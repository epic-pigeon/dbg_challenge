#ifndef DBG_CHALLENGE_UTIL_H
#define DBG_CHALLENGE_UTIL_H

#include "stdarg.h"

int prompt_option(int noptions, ...);
int prompt_option_array(int noptions, char **options);
char* prompt_line(char* str, int max_len);
void* ptr_array_of(int len, ...);

#endif //DBG_CHALLENGE_UTIL_H
