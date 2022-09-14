#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "MessageLib.h"
#include "LinkedList.h"
#include "Common.h"

typedef struct{
    void* pvParam;
    uint32_t uiMessage;
} Message_T;

typedef (*pfnMessageHandler)(Message_T *Message);

typedef struct Message_IF_T{
    MessageSt_T *ptMessageSt;
    
    Error_E (*SendMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터
    Error_E (*PostMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터

} Message_IF_T;

typedef struct {
    LinkedList_IF_T LinkedList_IF;  
    pthread_t tThead;
} MessageSt_T;


static void SF_MessageThread(MessageSt_T *ptMessageSt){
    LinkedList_IF_T *iLinkedList_IF;
}

static Error_E SF_CreateMessageThread(MessageSt_T *ptMessageSt){
    Error_E eRetVal = ERROR_NONE;
    void* pvParam;
    int iRetVal;

    iRetVal = pthread_create(&ptMessageSt->tThead, NULL, SF_MessageThread, ptMessageSt);// needed pthrad_t, threadCallbackFn, Parameters
    
    if(iRetVal == 0){ //normal

    }
    
    return eRetVal;
}

static Error_E SF_SendMessage(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam){

}

static Error_E SF_PostMessage(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam){
    
}

Message_IF_T* CreateMessage(pfnMessageHandler fnMessageHandler){
    Message_IF_T *iRet_IF;
    Error_E iRetError;
    iRet_IF = (Message_IF_T*)malloc(sizeof(Message_IF_T));

    if(iRet_IF != NULL){
        MessageSt_T *ptMessageSt;
        ptMessageSt = (MessageSt_T*)malloc(sizeof(ptMessageSt));

        iRetError = SF_CreateMessageThread(ptMessageSt);
        
    }

    iRet_IF->SendMessage = SF_SendMessage;
    iRet_IF->PostMessage = SF_PostMessage;

    return iRet_IF;

}