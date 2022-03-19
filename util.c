#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int prompt_option_array(int noptions, char **options) {
    printf("Choose an option:\n");
    for (int i = 0; i < noptions; i++) {
        printf("[ %d ] %s\n", i + 1, options[i]);
    }
    char line[51];
    while (1) {
        prompt_line(line, 51);
        int input = -1;
        if (sscanf(line, "%d", &input) == 1 && 1 <= input && input <= noptions) {
            return input-1;
        }
        printf("Please input a valid option.\n");
    }
}

int prompt_option(int noptions, ...) {
    va_list args;
    va_start(args, noptions);
    char** arr = malloc(noptions * sizeof(char*));
    for (int i = 0; i < noptions; i++) {
        arr[i] = va_arg(args, char*);
    }
    int res = prompt_option_array(noptions, arr);
    free(arr);
    va_end(args);
    return res;
}

char* prompt_line(char* str, int max_len) {
    char* res = fgets(str, max_len, stdin);
    return res;
}

void* ptr_array_of(int len, ...) {
    va_list args;
    va_start(args, len);
    void** res = malloc(len * sizeof(void*));
    for (int i = 0; i < len; i++) {
        res[i] = va_arg(args, void*);
    }
    va_end(args);
    return res;
}
