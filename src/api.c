#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"
#include "cJSON.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL)
        return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Add this near the top of api.c:
char* json_escape(const char* input) {
    size_t len = strlen(input);
    char* output = malloc(len * 2 + 3); // allocate enough space
    char* p = output;
    *p++ = '"';
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '"')      { *p++ = '\\'; *p++ = '"'; }
        else if (input[i] == '\\'){ *p++ = '\\'; *p++ = '\\'; }
        else if (input[i] == '\n'){ *p++ = '\\'; *p++ = 'n'; }
        else if (input[i] == '\r'){ *p++ = '\\'; *p++ = 'r'; }
        else if (input[i] == '\t'){ *p++ = '\\'; *p++ = 't'; }
        else                     { *p++ = input[i]; }
    }
    *p++ = '"';
    *p = 0;
    return output;
}

char* get_llm_comment(const char* api_key, const char* api_url, const char* code) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        char post_data[16384];
        char* code_escaped = json_escape(code);
        snprintf(post_data, sizeof(post_data),
            "{"
              "\"model\":\"gpt-3.5-turbo\","
              "\"messages\":["
                "{\"role\":\"system\",\"content\":\"You are a helpful code comment generator. For the following function, write a short descriptive comment that explains its purpose.\"},"
                "{\"role\":\"user\",\"content\":%s}"
              "],"
              "\"max_tokens\":100"
            "}", code_escaped);
        free(code_escaped);

        struct curl_slist *headers = NULL;
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, api_url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        // DEBUG: print raw API response
        printf("DEBUG: Raw API response:\n%s\n", chunk.memory);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }

        // Parse JSON response with cJSON (no change needed here)
        cJSON *json = cJSON_Parse(chunk.memory);
        const cJSON *choices = cJSON_GetObjectItemCaseSensitive(json, "choices");
        if (cJSON_IsArray(choices)) {
            cJSON *first = cJSON_GetArrayItem(choices, 0);
            cJSON *msg = cJSON_GetObjectItemCaseSensitive(first, "message");
            cJSON *content = cJSON_GetObjectItemCaseSensitive(msg, "content");
            if (cJSON_IsString(content) && (content->valuestring != NULL)) {
                char* comment = strdup(content->valuestring);
                cJSON_Delete(json);
                free(chunk.memory);
                return comment;
            }
        }
        cJSON_Delete(json);
        free(chunk.memory);
    }
    return NULL;
}
