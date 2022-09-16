#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "Common.h"

#include "Signal.h"
#include "MessageLib.h"
#include "LinkedList.h"

struct Message_T{
    void* pvParam;
    uint32_t uiMessage;
};

struct MessageSt_T{
    Message_T *ptMeesage;

    pthread_t tThead;

    Signal_T *ptSignal;
    LinkedList_IF_T *ptLinkedListIF;

    pfnMessageHandler fnMessageHandler;
};


static Error_E SF_ProcessPostMessage(MessageSt_T *ptMessageSt, Message_T *ptMessage){

}
static void SF_MessageThread(MessageSt_T *ptMessageSt){
    Error_E eError = ERROR_NONE;

    LinkedList_IF_T *ptLinkedList_IF;
    LinkedListSt_T *ptListSt;
    LinkedList_T *ptList;
    

    ptLinkedList_IF = ptMessageSt->ptLinkedListIF;
    ptListSt = ptLinkedList_IF->ptListSt;

    (void)SignalLock(ptMessageSt->ptSignal);

    
    if(ptLinkedList_IF->GetListCount(ptListSt) == 0){
        SignalBareWait(ptMessageSt->ptSignal);
    }

    
    // 리스트에서 맨 앞 리스트 가져옴
    ptList = ptLinkedList_IF->GetFirstList(ptListSt);

    // 가져온 리스트에 대해 리스트네임 가져옴
    if(ptList != NULL){
        char* pstrListName;

        pstrListName = ptLinkedList_IF->GetListName(ptList);
        // 가져온 리스트 네임 비교
        /*
        if(strcmp(pstrListName, "post") == 0){

            Message_T *tMessage = (tMessage*)LinkedList_IF_T->GetListData(ptList);
            SF_ProcessPostMessage(ptMessageSt, tMeesage);
        }else{
            
        }*/

    }
    

    // 네임에 맞게 프로세싱

    // 언락?
    
    
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
        LinkedList_IF_T *ptLinkedListIF = ptMessageSt->ptLinkedListIF;

        Message_T *ptMessage;

        ptMessage = (Message_T*)calloc(1UL, sizeof(Message_T));

        if(ptMessage !=NULL){
            
            ptMessage->pvParam = pvParam;
            ptMessage->uiMessage = uiMessage;
        
        }

        (void)SignalLock(ptMessageSt->ptSignal);

        ptInsertedList = ptLinkedListIF->InsertList(ptLinkedListIF->ptListSt, "post", ptMessage, sizeof(ptMessage));

        if(ptInsertedList != NULL){
            (void)SignalBareWakeup(ptMessageSt->ptSignal);
        }

        (void)SignalUnLock(ptMessageSt->ptSignal);
        

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