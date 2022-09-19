#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "Common.h"

#include "Signal.h"
#include "MessageLib.h"
#include "LinkedList.h"



struct MessageSt_T
{
    Message_T *ptMessage;

    pthread_t tThead;

    Signal_T *ptSignal;
    LinkedList_IF_T *ptLinkedListIF;

    pfnMessageHandler fnMessageHandler;
};


static Error_E SF_ProcessPostMessage(MessageSt_T *ptMessageSt, Message_T *ptMessage)
{
    Error_E eError = ERROR_NONE;
    
    if((ptMessageSt != NULL) && (ptMessage != NULL))
    {
        if(ptMessageSt->fnMessageHandler != NULL)
        {
            
            (void)ptMessageSt->fnMessageHandler(ptMessage);
        }
        else
        {   
            eError = ERROR_BAD_PARAMETER;
        }
    }
    else
    {
        
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}
static void SF_MessageThread(MessageSt_T *ptMessageSt)
{
    Error_E eError = ERROR_NONE;

    LinkedList_IF_T *ptLinkedList_IF;
    LinkedListSt_T *ptListSt;
    LinkedList_T *ptList;

    ptLinkedList_IF = ptMessageSt->ptLinkedListIF;
    ptListSt = ptLinkedList_IF->ptListSt;
    
    (void)SignalLock(ptMessageSt->ptSignal);
    
    if(ptLinkedList_IF->GetListCount(ptListSt) == 0)
    {
        SignalBareWait(ptMessageSt->ptSignal);
    }
    
    // 리스트에서 맨 앞 리스트 가져옴
    ptList = ptLinkedList_IF->GetFirstList(ptListSt);

    // 가져온 리스트에 대해 리스트네임 가져옴
    if(ptList != NULL)
    {
        char* pstrListName;

        pstrListName = ptLinkedList_IF->GetListName(ptList);
        // 가져온 리스트 네임 비교
        
        if(strcmp(pstrListName, "post") == 0)
        {
            Message_T *tMessage = ptLinkedList_IF->GetListData(ptList);
            
            SF_ProcessPostMessage(ptMessageSt, tMessage);
            
        }
        else
        {
            
        }

    }

    // 언락?
    
    
}

static Error_E SF_CreateMessageThread(MessageSt_T *ptMessageSt)
{
    
    Error_E eRetVal = ERROR_NONE;
    void* pvParam;
    int iRetVal;

    iRetVal = pthread_create(&ptMessageSt->tThead, NULL, SF_MessageThread, ptMessageSt);// needed pthrad_t, threadCallbackFn, Parameters
    
    if(iRetVal == 0)
    { //normal
        //
    }
    
    return eRetVal;
}

static Error_E SF_DestroyMessageThread(MessageSt_T *ptMessageSt)
{
    Error_E eError = ERROR_NONE;

    if(ptMessageSt != NULL)
    {

        int iRetVal = NULL; 

        (void)SignalWakeup(ptMessageSt->ptSignal); //JP

        iRetVal = pthread_join(ptMessageSt->tThead, NULL);

        if(iRetVal != 0)
        {
            eError = ERROR_THREAD_ERROR;
        }
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}

static Error_E SF_SendMessage(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam)
{

}

static Error_E SF_PostMessage(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam)
{
    Error_E eError = ERROR_NONE;

    if(ptMessageSt != NULL)
    {
        LinkedList_T *ptInsertedList;
        LinkedList_IF_T *ptLinkedListIF = ptMessageSt->ptLinkedListIF;

        Message_T *ptMessage;

        ptMessage = (Message_T*)calloc(1UL, sizeof(Message_T));

        if(ptMessage !=NULL)
        {   
            ptMessage->pvParam = pvParam;
            ptMessage->uiMessage = uiMessage;
        }

        (void)SignalLock(ptMessageSt->ptSignal);

        ptInsertedList = ptLinkedListIF->InsertList(ptLinkedListIF->ptListSt, "post", ptMessage, sizeof(Message_T));

        if(ptInsertedList != NULL)
        {
            eError = SignalBareWakeup(ptMessageSt->ptSignal);
        }
        else
        {
            eError = ERROR_INSUFFICIENT_RESOURCE;
        }

        (void)SignalUnLock(ptMessageSt->ptSignal);


        free(ptMessage);
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
        PrintError(eError, __func__);
    }
    
    return eError;
}

Message_IF_T* CreateMessage(pfnMessageHandler fnMessageHandler)
{
    
    Message_IF_T *ptRetIF;
    
    ptRetIF = (Message_IF_T*)calloc(1UL, sizeof(Message_IF_T));
    
    if(ptRetIF != NULL)
    {

        Error_E eError;

        ptRetIF->ptMessageSt = (MessageSt_T*)calloc(1UL, sizeof(MessageSt_T));

        if(ptRetIF->ptMessageSt != NULL)
        {

            MessageSt_T *ptMessageSt = ptRetIF->ptMessageSt;

            ptMessageSt->ptSignal = CreateSignal();
            ptMessageSt->ptLinkedListIF = CreateList();

            if((ptMessageSt->ptSignal != NULL) && (ptMessageSt->ptLinkedListIF != NULL))
            {

                ptMessageSt->fnMessageHandler = fnMessageHandler;
                
                ptRetIF->SendMessage = SF_SendMessage;
                ptRetIF->PostMessage = SF_PostMessage;

                eError = SF_CreateMessageThread(ptRetIF->ptMessageSt);
            
            }
            else
            {
                eError = ERROR_INSUFFICIENT_RESOURCE;
            }
        }
        else
        {
            eError = ERROR_INSUFFICIENT_RESOURCE;
        }

        if(eError != ERROR_NONE)
        {
            PrintError(eError, __func__);
            DestroyMessage(ptRetIF);
            ptRetIF = NULL;
        }
    }

    return ptRetIF;

}

Error_E DestroyMessage(Message_IF_T *ptMessageIF)
{
    Error_E eError = ERROR_NONE;

    if(ptMessageIF != NULL)
    {
        if(ptMessageIF->ptMessageSt != NULL)
        {
            MessageSt_T *ptMessageSt = ptMessageIF->ptMessageSt;
            
            eError = SF_DestroyMessageThread(ptMessageSt);

            DestroySignal(ptMessageSt->ptSignal);
            
            DestroyList(ptMessageSt->ptLinkedListIF);
            
            free(ptMessageIF->ptMessageSt);
        }
        //free(ptMessageIF);

    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;

}