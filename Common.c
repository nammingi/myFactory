#include <stdio.h>

#include "Common.h"

#define _DEBUG_


#ifdef _DEBUG_

inline void CheckDebugPoint(const char* _func_)
{
    printf("[Debug] Function [%-20s] has called.\n",_func_);
}

#else
inline void CheckDebugPoint(const char* _func_)
{
}
#endif

void PrintError(const Error_E eError, const char *pstrFunc)
{
    if(eError)
    {
        switch(eError)
        {
            case ERROR_BAD_PARAMETER:
                printf("ERROR_BAD_PARAMETER, at func[%s]\n", pstrFunc);
                break;
            case ERROR_INSUFFICIENT_RESOURCE:
                printf("ERROR_INSUFFICIENT_RESOURCE, at func[%s]\n", pstrFunc);
                break;
            case ERROR_THREAD_ERROR:
                printf("ERROR_THREAD_ERROR, at func[%s]\n", pstrFunc);
                break;
            case ERROR_SIGNAL:
                printf("ERROR_SIGNAL, at func[%s]\n", pstrFunc);
                break;

            default:
                printf("[Unkown Error], at func[%s]\n", pstrFunc);
                break;
        }
    }
    else
    {
        printf("ERROR_NONE, at func[%s]\n", pstrFunc);
    }
}