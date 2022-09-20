#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Common.h"
#include "LinkedList.h"

struct LinkedList_T
{
    void * pvData;
    char * pstrListName; 
    

    uint32_t iDataSize;

    LinkedList_T *ptPrev;
    LinkedList_T *ptNext;
};

struct LinkedListSt_T
{
    LinkedList_T *ptHead;
    LinkedList_T *ptTail;
};

static Error_E SF_Init_LinkedList(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    //error refactoring needed

    Error_E Ret_Error = ERROR_NONE;

    ptListSt->ptHead = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));
    ptListSt->ptTail = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));

    ptListSt->ptHead->ptPrev = ptListSt->ptHead;
    ptListSt->ptHead->ptNext = ptListSt->ptTail;
    
    ptListSt->ptTail->ptPrev = ptListSt->ptHead;
    ptListSt->ptTail->ptNext = ptListSt->ptTail;

    return Ret_Error;
}

static BOOL_E SF_IsListTail(LinkedListSt_T *ptListSt, LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    BOOL_E bRetVal = FALSE;

    bRetVal = (ptListSt->ptTail == ptList) ? TRUE : FALSE ;

    return bRetVal;
}

static LinkedList_T *SF_GetFisrtList(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    LinkedList_T *ptFisrtItem = NULL;

    if(SF_IsListTail(ptListSt, ptListSt->ptHead->ptNext) == FALSE)
    {
        ptFisrtItem = ptListSt->ptHead->ptNext;
    }

    return ptFisrtItem;
}

static char* SF_GetListName(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    //char* pstrListName;
    if(ptList != NULL)
    {
        return ptList->pstrListName;
    }
}

static Error_E SF_SetListName(LinkedList_T *ptList, const char* pcstrListName)
{
    CheckDebugPoint(__func__);
    Error_E RetError = ERROR_NONE;

    size_t iStrLen = strlen(pcstrListName);

    ptList->pstrListName = (char*)calloc(iStrLen, sizeof(char));

    (void)strcpy(ptList->pstrListName, pcstrListName);

    return RetError;
}

static Error_E SF_SetListData(LinkedList_T *ptList, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);
    Error_E RetError = ERROR_NONE;
    
    ptList->pvData = calloc(1UL, sizeof(iDataSize));

    (void)memcpy(ptList->pvData, pvData, iDataSize);

    ptList->iDataSize = iDataSize;

    return RetError;
}

static void *SF_GetListData(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    if(ptList != NULL)
    {
        return ptList->pvData;
    }
}

static uint32_t SF_GetListDataSize(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    uint32_t iDataSize = 0UL;

    iDataSize = ptList->iDataSize;

    return iDataSize;
}

static LinkedList_T *SF_InsertList(LinkedListSt_T *ptListSt, char* pstrListName, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);
    //attach to before Tail node.
    Error_E eError = ERROR_NONE;

    LinkedList_T *ptNewList = NULL;

    ptNewList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));

    LinkedList_T *ptOldList = ptListSt->ptTail;

    ptOldList->ptPrev->ptNext = ptNewList;
    ptNewList->ptPrev         = ptOldList->ptPrev;
    ptOldList->ptPrev         = ptNewList;
    ptNewList->ptNext         = ptOldList;

    eError = SF_SetListName(ptNewList, pstrListName);

    eError = SF_SetListData(ptNewList, pvData, iDataSize);

    return ptNewList;
}

static LinkedList_T *SF_InsertListFirst(LinkedListSt_T *ptListSt, char* pstrListName, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    LinkedList_T *ptNewFirstList;
    
    ptNewFirstList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));

    LinkedList_T *ptOldList = ptListSt->ptHead;

    ptOldList->ptNext->ptPrev       = ptNewFirstList;
    ptNewFirstList->ptPrev          = ptOldList;
    ptNewFirstList->ptNext          = ptOldList->ptNext;
    ptOldList->ptNext               = ptNewFirstList;

    eError = SF_SetListName(ptNewFirstList, pstrListName);

    eError = SF_SetListData(ptNewFirstList, pvData, iDataSize);
    
    return ptNewFirstList;
}

static Error_E SF_DeleteListData(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    ptList->pvData = NULL;
    ptList->iDataSize = 0UL;

    free(ptList->pvData);

    return eError;
}

static Error_E SF_DeleteListName(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    ptList->pstrListName = NULL;
    
    free(ptList->pstrListName);

    return eError;
}

static Error_E SF_DeleteList(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if(ptList != NULL)
    {
        ptList->ptPrev->ptNext = ptList->ptNext;

        ptList->ptNext->ptPrev = ptList->ptPrev;

        eError = SF_DeleteListData(ptList);

        if(eError == ERROR_NONE)
        {
            eError = SF_DeleteListName(ptList);      

            if(eError == ERROR_NONE)
            {
                free(ptList);
            }
        }

    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}

static Error_E SF_DeleteListAll(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if(ptListSt != NULL)
    {
        LinkedList_T *ptList = ptListSt->ptHead->ptNext;

        while((SF_IsListTail(ptListSt, ptList)==FALSE) && (eError == ERROR_NONE))
        {
            eError = SF_DeleteList(ptList);
            ptList = ptList->ptNext;
        }
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

static int SF_GetListCount(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    int iRetVal = 0;
    LinkedList_T *ptList;
    if(ptListSt != NULL)
    {
        ptList = ptListSt->ptHead->ptNext;

        if(ptList != NULL)
        {
            if(SF_IsListTail(ptListSt, ptList) == TRUE )
            {
                return 0;
            }
            else
            {
                while(SF_IsListTail(ptListSt, ptList) == FALSE)
                {
                    ++iRetVal;
                    ptList = ptList->ptNext;
                }
                printf("[Debug point] in [%s], iRetVal == %d\n", __func__, iRetVal);
                return iRetVal;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

static Error_E SF_TermList(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    
    Error_E eError = ERROR_NONE;

    if(ptListSt != NULL)
    {

        ptListSt->ptHead = NULL;
        free(ptListSt->ptHead);

        ptListSt->ptTail = NULL;
        free(ptListSt->ptTail);

        //free(ptListSt);

    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

LinkedList_IF_T *CreateList(void)
{
    CheckDebugPoint(__func__);
    Error_E eError;
    LinkedList_IF_T *ptLinkedList_IF_T;
    ptLinkedList_IF_T = (LinkedList_IF_T *)calloc(1UL, sizeof(LinkedList_IF_T));
    
    ptLinkedList_IF_T->ptListSt = (LinkedListSt_T *)calloc(1UL, sizeof(LinkedListSt_T));

    eError = SF_Init_LinkedList(ptLinkedList_IF_T->ptListSt);

    //Modify below functions
    ptLinkedList_IF_T->InsertList       = SF_InsertList;
    ptLinkedList_IF_T->InsertListFisrt  = SF_InsertListFirst;
    ptLinkedList_IF_T->GetFirstList     = SF_GetFisrtList;
    ptLinkedList_IF_T->SetListData      = SF_SetListData;
    ptLinkedList_IF_T->DeleteList       = SF_DeleteList;
    ptLinkedList_IF_T->TermList         = SF_TermList;
    ptLinkedList_IF_T->GetListName      = SF_GetListName;
    ptLinkedList_IF_T->SetListName      = SF_SetListName;
    ptLinkedList_IF_T->GetListData      = SF_GetListData;
    ptLinkedList_IF_T->GetListDataSize  = SF_GetListDataSize;
    ptLinkedList_IF_T->GetListCount     = SF_GetListCount;

    return ptLinkedList_IF_T;
}

Error_E DestroyList(LinkedList_IF_T *ptListIF)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;
    if(ptListIF != NULL)
    {

        eError = SF_DeleteListAll(ptListIF->ptListSt);
        if(eError == ERROR_NONE)
        {
            SF_TermList(ptListIF->ptListSt);
        }
    }

    free(ptListIF);

    return eError;
}