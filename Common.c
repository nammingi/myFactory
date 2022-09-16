#include <stdio.h>

#include "Common.h"

void PrintError(Error_E eError, char *pstrFunc){
    if(eError){
        switch(eError){
            case ERROR_NONE:
                printf("ERROR_NONE, at func[%s]\n", pstrFunc);
                break;
            case ERROR_BAD_PARAMETER:
                printf("ERROR_BAD_PARAMETER, at func[%s]\n", pstrFunc);
                break;
            case ERROR_INSUFFICIENT_RESOURCE:
                printf("ERROR_INSUFFICIENT_RESOURCE, at func[%s]\n", pstrFunc);
                break;
            
            default:
                printf("[Unkown Error], at func[%s]\n", pstrFunc);
                break;
        }
    }else{
        printf(" Print error occured : ERROR_BAD_PARAMETER, at func[%s]\n", pstrFunc);
    }
}