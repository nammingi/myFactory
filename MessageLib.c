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
    BOOL_E bDestroyed;
    pthread_t tThead;

    Signal_T *ptSignal;

    LinkedList_IF_T *ptLinkedListIF;

    pfnMessageHandler fnMessageHandler;
};

typedef struct 
{
    Signal_T *ptSignal;
    Message_T *ptMessage;

} SendMessage_T;

static Error_E SF_ProcessPostMessage(MessageSt_T *ptMessageSt, Message_T *ptMessage)
{
    CheckDebugPoint(__func__);
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

static Error_E SF_ProcessSendMessage(MessageSt_T *ptMessageSt, SendMessage_T *ptSendMessage)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if((ptMessageSt != NULL) && (ptSendMessage != NULL))
    {
        if(ptMessageSt->fnMessageHandler != NULL)
        {
            ptMessageSt->fnMessageHandler(ptSendMessage->ptMessage);
        }
        else
        {
            eError = ERROR_BAD_PARAMETER;
        }
        (void)SignalWakeup(ptSendMessage->ptSignal);
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}
static void SF_MessageThread(MessageSt_T *ptMessageSt)
{
    CheckDebugPoint(__func__);
    if(ptMessageSt != NULL)
    {
        printf("[Debug point] MSG Thread, CALLED\n");
        const LinkedList_IF_T *ptLinkedList_IF;
        LinkedListSt_T *ptListSt;
        LinkedList_T *ptList;

        ptLinkedList_IF = ptMessageSt->ptLinkedListIF;
        ptListSt = ptLinkedList_IF->ptListSt;
        
        int whileCnt=0;
        while(ptMessageSt->bDestroyed == FALSE)
        {   
            printf("[Debug point] MSG Thread, While Syntax, %d times repeated.\n",++whileCnt);
            (void)SignalLock(ptMessageSt->ptSignal);
            printf("[Debug point] Msg Thread, While Syntax, Signal LOCKED\n");
            if(ptLinkedList_IF->GetListCount(ptListSt) == 0)
            {
                printf("[Debug point] Msg Thread, While Syntax, BareWaiting..\n");
                (void)SignalBareWait(ptMessageSt->ptSignal);
            }

            ptList = ptLinkedList_IF->GetFirstList(ptListSt);
            printf("[Debug point] Msg Thread, While Syntax, First List of %d List loaded. [Debug point] %d\n",ptLinkedList_IF->GetListCount(ptListSt), ptList);
            if(ptList != NULL)
            {
                char* pstrListName;

                pstrListName = ptLinkedList_IF->GetListName(ptList);
                printf("[Debug point] Msg Thread, While Syntax, Now ListName is [%s]\n",pstrListName);
                if(strcmp(pstrListName, "post") == 0)
                {
                    Message_T *tMessage = ptLinkedList_IF->GetListData(ptList);
                    
                    SF_ProcessPostMessage(ptMessageSt, tMessage);

                    (void)ptLinkedList_IF->DeleteList(ptList);
                }

                if(strcmp(pstrListName, "Send") == 0)
                {
                    Message_T *tMessage = ptLinkedList_IF->GetListData(ptList);

                    SF_ProcessSendMessage(ptMessageSt, tMessage);

                    (void)ptLinkedList_IF->DeleteList(ptList);
                }

                printf("[Debug point] Msg Thread, While Syntax, previous Signal UnLOCKING\n");
                (void)SignalUnLock(ptMessageSt->ptSignal);
                printf("[Debug point] MSG Thread, While Syntax, Signal UnLOCKED\n");
            
            }
        }
    }

    
    
}

static Error_E SF_CreateMessageThread(MessageSt_T *ptMessageSt)
{
    CheckDebugPoint(__func__);
    
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
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if(ptMessageSt != NULL)
    {
        int iRetVal = NULL; 

        (void)SignalWakeup(ptMessageSt->ptSignal); //JP

        iRetVal = pthread_join(ptMessageSt->tThead, NULL);

        ptMessageSt->bDestroyed = TRUE;

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
    CheckDebugPoint(__func__);
    int cnt=0;
    Error_E eError = ERROR_NONE;
    
    if(ptMessageSt != NULL)
    {
        SendMessage_T tSendMessage;
        LinkedList_T *ptInsertedList;
        LinkedList_IF_T *ptListIF = ptMessageSt->ptLinkedListIF;
        
        Message_T *ptMessage;
        ptMessage = (Message_T *)calloc(1UL, sizeof(Message_T));

        if(ptMessage != NULL)
        {
            ptMessage->uiMessage    = uiMessage;
            ptMessage->pvParam      = pvParam;
        }

        tSendMessage.ptMessage = ptMessage;
        tSendMessage.ptSignal = CreateSignal();

        (void)SignalLock(ptMessageSt->ptSignal);

        ptInsertedList = ptListIF->InsertListFisrt(ptListIF->ptListSt, "Send", &tSendMessage, sizeof(SendMessage_T));

        if(ptInsertedList != NULL)
        {
            printf("1\n");
            (void)SignalLock(tSendMessage.ptSignal);
            printf("2\n");
            (void)SignalBareWakeup(ptMessageSt->ptSignal);
            printf("3\n");
            (void)SignalUnLock(ptMessageSt->ptSignal);
            printf("4\n");
            (void)SignalBareWait(tSendMessage.ptSignal);
            printf("5\n");
            (void)SignalUnLock(tSendMessage.ptSignal);
            printf("6\n");
            (void)DestroySignal(tSendMessage.ptSignal);
            printf("7\n");
        }
        else
        {
            (void)SignalUnLock(ptMessageSt->ptSignal);

            (void)DestroySignal(tSendMessage.ptSignal);

            eError = ERROR_INSUFFICIENT_RESOURCE;
        }
        
    return eError;

    }
}

static Error_E SF_PostMessage(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam)
{
    CheckDebugPoint(__func__);
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
        printf("[Debug point] PostMSG, Signal LOCKED\n");

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
        printf("[Debug point] PostMSG, Signal UnLOCKED\n");

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
    CheckDebugPoint(__func__);
    
    Message_IF_T *ptRetIF;
    
    ptRetIF = (Message_IF_T*)calloc(1UL, sizeof(Message_IF_T));
    
    if(ptRetIF != NULL)
    {

        Error_E eError;

        ptRetIF->ptMessageSt = (MessageSt_T*)calloc(1UL, sizeof(MessageSt_T));

        if(ptRetIF->ptMessageSt != NULL)
        {

            MessageSt_T *ptMessageSt = ptRetIF->ptMessageSt;

            ptMessageSt->bDestroyed = FALSE;
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
    CheckDebugPoint(__func__);
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
        free(ptMessageIF);
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;

}