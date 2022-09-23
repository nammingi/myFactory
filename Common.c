#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "Common.h"

#define _DEBUG_

#define BADKEY -1
#define FUNC_SF_POSTMESSAGE 1
#define FUNC_SF_SENDMESSAGE 2
#define FUNC_SF_POSTMESSAGEEX 3

#define NKEYS (sizeof(LookUpTable)/sizeof(_KV_T))

typedef struct {
    char *key;
    int val;
} _KV_T;

static _KV_T LookUpTable[] = {
    {"SF_PostMessage" , FUNC_SF_POSTMESSAGE },
    {"SF_SendMessage" , FUNC_SF_SENDMESSAGE },
    {"SF_PostMessageEX" , FUNC_SF_POSTMESSAGEEX }    
};

static int SF_GetValueFromKey(const char *key)
{
    int i;
    for (i=0; i < NKEYS; i++) {
        _KV_T *ptKVTable = &LookUpTable[i];
        if (strcmp(ptKVTable->key, key) == 0)
            return ptKVTable->val;
    }
    return BADKEY;
}


#ifdef _DEBUG_

inline void CheckDebugPoint(const char* _func_)
{
    if(_func_ != NULL)
    {
        pthread_t ulThreadID;
        ulThreadID = pthread_self();

        char *_color_s = NULL;
        char *_color_rs = COLOR_RESET;
        
        switch(SF_GetValueFromKey(_func_))
        {
            case FUNC_SF_POSTMESSAGE:
                _color_s = COLOR_RED;
                break;

            case FUNC_SF_SENDMESSAGE:
                _color_s = COLOR_BLUE;
                break;
            case FUNC_SF_POSTMESSAGEEX:
                _color_s = COLOR_GREEN;
                break;

            default:
                _color_s = COLOR_RESET;
        }
        printf("%s[Debug] Function [%-20s] has called. / ThreadID = [%ul]%s\n",_color_s,_func_, ulThreadID,_color_rs);
    }
    else
    {
        printf("!!!! [Debug point Error] !!!!\n");
    }
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