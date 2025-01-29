#include "types.h"
#include "string.h"

int strcmp(const char* s1, const char* s2) {
    
    if(!s1 || !s2) {
        return s1 == s2 ? 0 : 1;
    }

    while(*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

unsigned long strlen(const char* str) {
    uint64 ret;

    for(ret = 0; str[ret]; ++ret) ;

    return ret;
}