#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "MessageLib.h"
#include "LinkedList.h"
#include "Signal.h"
#include "Common.h"

struct Message_T{
    void* pvParam;
    uint32_t uiMessage;
};

typedef struct Message_IF_T{
    MessageSt_T *ptMessageSt;
    
    Error_E (*SendMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터
    Error_E (*PostMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터

} Message_IF_T;

typedef struct {
    Message_T *ptMeesage;

    LinkedList_IF_T LinkedList_IF;  
    pthread_t tThead;

    Signal_T *ptSignal;
    LinkedList_IF_T *ptLinkedListIF;

    pfnMessageHandler fnMessageHandler;
} MessageSt_T;


static void SF_MessageThread(MessageSt_T *ptMessageSt){
    LinkedList_IF_T *ptLinkedList_IF;
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
    
    Error_E eError = ERROR_NONE;

    if(ptMessageSt != NULL){
        LinkedList_T *ptInsertedList;
        LinkedList_IF_T *ptLinkedListIF = ptMessageSt->LinkedList_IF;

        Message_T *ptMessage;

        ptMessage = (*Message_T)calloc(1UL, sizeof(Message_T));

        if(ptMessage !=NULL){
            
            ptMessage->pvParam = pvParam;
            ptMessage->uiMessage = uiMessage;
        
        }

        (void)SignalLock(ptMessageSt->ptSignal);

        ptInsertedList = ptLinkedListIF->InsertList(ptLinkedListIF->ptListSt, "post", ptMessage, sizeof(ptMessage));

        if(ptInsertedList != NULL){
            (void)SignalBareWakeup(ptMessageSt->ptSignal)
        }

        (void)SignalUnlock(ptMessageSt->ptSignal);
        

        free(ptMessage);
    }else{
        eError = ERROR_BAD_PARAMETER;
    }
    
    return eError;
}

Message_IF_T* CreateMessage(pfnMessageHandler fnMessageHandler){
    Message_IF_T *ptRetIF;
    Error_E eError;
    ptRetIF = (Message_IF_T*)malloc(sizeof(Message_IF_T));
    
    if(ptRetIF != NULL){
        MessageSt_T *ptMessageSt;
        ptMessageSt = (MessageSt_T*)malloc(sizeof(ptMessageSt));

        ptMessageSt->ptSignal = CreateSignal();
        ptMessageSt->ptLinkedListIF = CreateList();

        if((ptMessageSt->ptSignal != NULL) && (ptMessageSt->ptLinkedListIF != NULL)){
            ptMessageSt->fnMessageHandler = fnMessageHandler;
            
            ptRetIF->SendMessage = SF_SendMessage;
            ptRetIF->PostMessage = SF_PostMessage;

            eError = SF_CreateMessageThread(ptMessageSt);
        }else{
            eError = ERROR_INSUFFICIENT_RESOURCE;
        }
    }else{
        eError = ERROR_INSUFFICIENT_RESOURCE;
    }

    if(eError != ERROR_NONE){
        DestroyMessage(ptRetIF);
        ptRetIF = NULL;
    }

    return ptRetIF;

}

Error_E DestroyMessage(Message_IF_T *ptMeesageIF){

}