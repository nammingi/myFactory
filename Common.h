#ifndef COMMON_H_
#define COMMON_H_


#define COLOR_RED	"\033[38;2;170;0;0m"
#define COLOR_GREEN	"\033[38;2;0;170;0m"
#define COLOR_BLUE	"\033[38;2;0;0;170m"
#define COLOR_RESET	"\033[0m"



#include <stdbool.h>

typedef enum{
    ERROR_NONE=0,
    ERROR_BAD_PARAMETER,
    ERROR_INSUFFICIENT_RESOURCE,
    ERROR_THREAD_ERROR,
    ERROR_SIGNAL,
    

} Error_E;

typedef enum{
    FALSE = 0,
    TRUE
} BOOL_E;

void CheckDebugPoint(const char* _func_);
void PrintError(const Error_E eError, const char *pstrFunc);
#endif
