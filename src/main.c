#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "api.h"
#include "parser.h"

// Helper to get env vars from .env (simple version)
char* getenv_value(const char* key) {
    FILE* env = fopen("../.env", "r");
    static char value[512];
    char line[600];
    if (!env) return NULL;
    while (fgets(line, sizeof(line), env)) {
        char* eq = strchr(line, '=');
        if (!eq) continue;
        *eq = 0;
        if (strcmp(line, key) == 0) {
            strncpy(value, eq + 1, sizeof(value));
            value[strcspn(value, "\n")] = 0; // remove newline
            fclose(env);
            return value;
        }
    }
    fclose(env);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    // char* api_key = getenv_value("OPENAI_API_KEY");
    // char* api_url = getenv_value("OPENAI_API_URL");
    // Add this at the top

    // ...inside main:
    char* api_key = getenv("OPENAI_API_KEY");
    char* api_url = getenv("OPENAI_API_URL");

    if (!api_key || !api_url) {
        fprintf(stderr, "API Key or URL not set in .env\n");
        return 1;
    }

    char* src_code = read_file(argv[1]);
    if (!src_code) {
        fprintf(stderr, "Failed to read input file.\n");
        return 1;
    }

    int count = 0;
    char** functions = extract_functions(src_code, &count);

    FILE* outf = fopen("output_commented.c", "w");
    if (!outf) {
        fprintf(stderr, "Failed to open output file.\n");
        free(src_code);
        return 1;
    }

    // For simplicity, just process and print for now:
    for (int i = 0; i < count; i++) {
        char* comment = get_llm_comment(api_key, api_url, functions[i]);
        printf("DEBUG: Comment for function %d: %s\n", i+1, comment ? comment : "(null)");
        if (comment) {
            fprintf(outf, "// %s\n%s\n\n", comment, functions[i]);
            free(comment);
        } else {
            fprintf(outf, "%s\n\n", functions[i]);
        }
        free(functions[i]);
    }

    free(functions);
    free(src_code);
    fclose(outf);

    printf("Done! Output written to output_commented.c\n");
    return 0;
}
