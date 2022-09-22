#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h> 
#include "Common.h"
#include "MessageLib.h"


int32_t fnMessageHandler(const Message_T *ptMessage)
{
    printf("[Debug] This is msg handler\n%u\n", ptMessage->uiMessage);

    return 0;
}

int main()
{
    Error_E eError = ERROR_NONE;

    pfnMessageHandler pfnMessageHandler = fnMessageHandler;

    printf("[Main Started]\n");
    
    Message_IF_T *ptMessageIF;

    ptMessageIF = CreateMessage(pfnMessageHandler);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 1111, NULL);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 2222, NULL);

    ptMessageIF->SendMessage(ptMessageIF->ptMessageSt, 4444, NULL);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 3333, NULL);

    ptMessageIF->PostMessage(ptMessageIF->ptMessageSt, 5555, NULL);

    
    printf("[All MSG Inserted]\n");

    sleep(1);

    (void)DestroyMessage(ptMessageIF);
    
    printf("[EXIT]\n");

    return eError;

}