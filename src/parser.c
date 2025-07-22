#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

char** extract_functions(const char* src, int* count) {
    *count = 0;
    char** results = malloc(sizeof(char*) * 128); // up to 128 functions
    const char* cursor = src;
    char* start;
    while ((start = strstr(cursor, "{"))) {
        // Find function signature
        const char* sig = cursor;
        // Go back to previous semicolon or brace
        while (sig > src && *sig != '}' && *sig != ';' && *sig != '\n') sig--;
        if (*sig != '\n') sig++;
        size_t len = start - sig + 1;
        // Now find end of function (matching brace)
        int depth = 1;
        const char* end = start + 1;
        while (*end && depth > 0) {
            if (*end == '{') depth++;
            if (*end == '}') depth--;
            end++;
        }
        len = end - sig;
        char* func = malloc(len + 1);
        strncpy(func, sig, len);
        func[len] = 0;
        results[*count] = func;
        (*count)++;
        cursor = end;
        if (*count >= 128) break;
    }
    return results;
}
