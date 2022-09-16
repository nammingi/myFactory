#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Common.h"
#include "MessageLib.h"

int32_t fnMessageHandler(Message_T *ptMessage){
    printf(">> This is msg handler\n");
}

int main(){
    Error_E eError = ERROR_NONE;

    pfnMessageHandler fnMessageHandler = fnMessageHandler;

    printf("started\n");
    Message_IF_T *ptMessageIF;
    ptMessageIF = CreateMessage(fnMessageHandler);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 1234, NULL);
    
    printf("done\n");

    sleep(2);

    eError = DestroyMessage(ptMessageIF);
    
    return eError;

}