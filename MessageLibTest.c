#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Common.h"
#include "MessageLib.h"

int32_t fnMessageHandler(Message_T *ptMessage)
{
    printf(">> This is msg handler\n%u\n", ptMessage->uiMessage);
}

int main()
{
    Error_E eError = ERROR_NONE;

    pfnMessageHandler pfnMessageHandler = fnMessageHandler;

    printf("started\n");
    Message_IF_T *ptMessageIF;
    ptMessageIF = CreateMessage(pfnMessageHandler);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 1234, NULL);
    
    printf("done\n");

    sleep(1);

    eError = DestroyMessage(ptMessageIF);
    
    return eError;

}