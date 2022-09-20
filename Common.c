#include <stdio.h>

#include "Common.h"

inline void CheckDebugPoint(char* _func_)
{
    printf("[Debug point] Function [%s] has called.\n",_func_);
}


void PrintError(Error_E eError, char *pstrFunc)
{
    if(eError)
    {
        switch(eError)
        {
            case ERROR_NONE:
                printf("ERROR_NONE, at func[%s]\n", pstrFunc);
                break;
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
        printf(">> Print error occured : [%d]ERROR_BAD_PARAMETER, at func[%s]\n", eError, pstrFunc);
    }
}