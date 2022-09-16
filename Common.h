#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

typedef enum{
    ERROR_NONE=0,
    ERROR_BAD_PARAMETER,
    ERROR_INSUFFICIENT_RESOURCE,
    ERROR_THREAD_ERROR,

} Error_E;

typedef enum{
    FALSE = 0,
    TRUE
} BOOL_E;


void PrintError(Error_E eError, char *pstrFunc);
#endif
