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
    Error_E eError = ERROR_NONE;

    if(ptListSt != NULL)
    {
        ptListSt->ptHead = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));
        if(ptListSt->ptHead != NULL)
        {
            ptListSt->ptHead->ptPrev = ptListSt->ptHead;
            ptListSt->ptHead->ptNext = ptListSt->ptTail;
        }
        
        ptListSt->ptTail = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));
        if(ptListSt->ptHead != NULL)
        {
            ptListSt->ptTail->ptPrev = ptListSt->ptHead;
            ptListSt->ptTail->ptNext = ptListSt->ptTail;
        }
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

static BOOL_E SF_IsListTail(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);

    BOOL_E bRetVal = FALSE;

    if((ptListSt != NULL) && (ptList != NULL))
    {
        bRetVal = (ptListSt->ptTail == ptList) ? TRUE : FALSE ;
    }

    return bRetVal;
}

static BOOL_E SF_IsListHead(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    BOOL_E bRetVal = FALSE;

    if((ptListSt != NULL) && (ptList != NULL))
    {
        bRetVal = (ptListSt->ptHead == ptList) ? TRUE : FALSE ;
    }

    return bRetVal;
}

static LinkedList_T *SF_GetFirstList(const LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    LinkedList_T *ptFisrtItem = NULL;
    if(ptListSt != NULL)
    {
        if(SF_IsListTail(ptListSt, ptListSt->ptHead->ptNext) == FALSE)
        {
            ptFisrtItem = ptListSt->ptHead->ptNext;
        }
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

    Error_E eError = ERROR_NONE;

    if(ptList != NULL)
    {
        uint64_t iStrLen = strlen(pcstrListName);

        ptList->pstrListName = (char*)calloc(iStrLen, sizeof(char));

        (void)strcpy(ptList->pstrListName, pcstrListName);
    }
    else
    {
        eError = ERROR_NONE;
    }

    return eError;
}

static Error_E SF_SetListData(LinkedList_T *ptList, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);

    Error_E eError = ERROR_NONE;

    if(ptList != NULL)
    {
        ptList->pvData = calloc(1UL, sizeof(iDataSize));

        if(ptList->pvData != NULL)
        {
            (void)memcpy(ptList->pvData, pvData, iDataSize);
        }

        ptList->iDataSize = iDataSize;
    }
    else
    {
        eError = ERROR_NONE;
    }

    return eError;
}

static void *SF_GetListData(LinkedList_T *ptList)
{
    Error_E eError = ERROR_NONE;

    CheckDebugPoint(__func__);

    if(ptList != NULL)
    {
        return ptList->pvData;
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
        PrintError(eError, __func__);
    }
}

static uint32_t SF_GetListDataSize(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);

    if(ptList != NULL)
    {
        uint32_t iDataSize = 0UL;

        iDataSize = ptList->iDataSize;

        return iDataSize;
    }
    else
    {
        return 0;
    }
}

static Error_E SF_DeleteListData(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if(ptList != NULL)
    {
        if(ptList->pvData != NULL)
        {
            if(ptList->iDataSize > 0UL)
            {
                free(ptList->pvData);

                ptList->pvData = NULL;
                
                ptList->iDataSize = 0UL;
            }
            else
            {
                ptList->pvData = NULL;
            }
        }
        else
        {
            eError = ERROR_INSUFFICIENT_RESOURCE;
        }
    }
    else
    {
        eError = ERROR_NONE;
    }
    return eError;
}

static Error_E SF_DeleteListName(LinkedList_T *ptList)
{
    CheckDebugPoint(__func__);
    Error_E eError = ERROR_NONE;

    if(ptList != NULL)
    {
        if(ptList->pstrListName != NULL)
        {
            free(ptList->pstrListName);

            ptList->pstrListName = NULL;
        }
        else
        {
            eError = ERROR_INSUFFICIENT_RESOURCE;
        }
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
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
        ptList = NULL;
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

static LinkedList_T *SF_InsertList(LinkedListSt_T *ptListSt, const char* pcstrListName, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);
    //attach to before Tail node.
    LinkedList_T *ptNewList = NULL;

    if(ptListSt != NULL)
    {
        Error_E eError = ERROR_NONE;

        ptNewList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));

        if(ptNewList != NULL)
        {
            LinkedList_T *ptOldList = ptListSt->ptTail;

            ptOldList->ptPrev->ptNext = ptNewList;
            ptNewList->ptPrev         = ptOldList->ptPrev;
            ptOldList->ptPrev         = ptNewList;
            ptNewList->ptNext         = ptOldList;
            
            if(pcstrListName != NULL)
            {
                eError = SF_SetListName(ptNewList, pcstrListName);
            }
            else
            {
                eError = ERROR_BAD_PARAMETER;
                (void)SF_DeleteList(ptNewList);
                ptNewList = NULL;
            }

            if(eError == ERROR_NONE)
            {
                if(pvData != NULL)
                {
                    eError = SF_SetListData(ptNewList, pvData, iDataSize);
                }
                else
                {
                    eError = ERROR_BAD_PARAMETER;
                    (void)SF_DeleteList(ptNewList);
                    ptNewList = NULL;
                }
            }
        }
       
    }

    return ptNewList;
}

static LinkedList_T *SF_InsertListFirst(LinkedListSt_T *ptListSt, const char* pstrListName, void* pvData, uint32_t iDataSize)
{
    CheckDebugPoint(__func__);

    LinkedList_T *ptNewFirstList = NULL;

    if(ptListSt != NULL)
    {
        Error_E eError = ERROR_NONE;

        ptNewFirstList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));

        if(ptNewFirstList != NULL)
        {
            LinkedList_T *ptOldList = ptListSt->ptHead;

            ptOldList->ptNext->ptPrev       = ptNewFirstList;
            ptNewFirstList->ptPrev          = ptOldList;
            ptNewFirstList->ptNext          = ptOldList->ptNext;
            ptOldList->ptNext               = ptNewFirstList;

            if(pstrListName != NULL)
            {
                eError = SF_SetListName(ptNewFirstList, pstrListName);
            }
            else
            {
                eError = ERROR_BAD_PARAMETER;
                (void)SF_DeleteList(ptNewFirstList);
                ptNewFirstList = NULL;
            }
            if(eError == ERROR_NONE)
            {
                if(pvData != NULL)
                {
                    eError = SF_SetListData(ptNewFirstList, pvData, iDataSize);
                }
                else
                {
                    eError = ERROR_BAD_PARAMETER;
                    (void)SF_DeleteList(ptNewFirstList);
                    ptNewFirstList = NULL;
                }
            }    
        }
    }
    return ptNewFirstList;
}

static int SF_GetListCount(LinkedListSt_T *ptListSt)
{
    CheckDebugPoint(__func__);
    int32_t iRetVal = 0;

    LinkedList_T *ptList = NULL;

    if(ptListSt != NULL)
    {
        ptList = ptListSt->ptHead->ptNext;

        if(ptList != NULL)
        {
            if(SF_IsListTail(ptListSt, ptList) == TRUE)
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

                dprintf("[Debug] ListCount == %d\n",iRetVal);

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
        free(ptListSt->ptHead);
        ptListSt->ptHead = NULL;

        free(ptListSt->ptTail);
        ptListSt->ptTail = NULL;
    }
    else
    {
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

static LinkedList_T *SF_GetNextList(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList)
{   
    CheckDebugPoint(__func__);

    LinkedList_T *ptRetList = NULL;
    if((ptListSt != NULL) && (ptList != NULL))
    {
        if(SF_IsListTail(ptListSt, ptList->ptNext) == FALSE)
        {
            ptRetList = ptList->ptNext;
        }
    }
    return ptRetList;
}

static LinkedList_T *SF_GetPrevList(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList)
{   
    CheckDebugPoint(__func__);

    LinkedList_T *ptRetList = NULL;
    if((ptListSt != NULL) && (ptList != NULL))
    {
        if(SF_IsListHead(ptListSt, ptList->ptPrev) == FALSE)
        {
            ptRetList = ptList->ptPrev;
        }
    }
    return ptRetList;
}



LinkedList_IF_T *CreateList(void)
{
    CheckDebugPoint(__func__);

    LinkedList_IF_T *ptLinkedList_IF_T = NULL;

    ptLinkedList_IF_T = (LinkedList_IF_T *)calloc(1UL, sizeof(LinkedList_IF_T));
    
    if(ptLinkedList_IF_T != NULL)
    {
        ptLinkedList_IF_T->ptListSt = (LinkedListSt_T *)calloc(1UL, sizeof(LinkedListSt_T));
        
        if(ptLinkedList_IF_T->ptListSt != NULL)
        {
            Error_E eError;

            eError = SF_Init_LinkedList(ptLinkedList_IF_T->ptListSt);
            //Modify below functions
            if(eError == ERROR_NONE)
            {
                ptLinkedList_IF_T->InsertList       = SF_InsertList;
                ptLinkedList_IF_T->InsertListFirst  = SF_InsertListFirst;
                ptLinkedList_IF_T->GetFirstList     = SF_GetFirstList;
                ptLinkedList_IF_T->SetListData      = SF_SetListData;
                ptLinkedList_IF_T->DeleteList       = SF_DeleteList;
                ptLinkedList_IF_T->TermList         = SF_TermList;
                ptLinkedList_IF_T->GetListName      = SF_GetListName;
                ptLinkedList_IF_T->SetListName      = SF_SetListName;
                ptLinkedList_IF_T->GetListData      = SF_GetListData;
                ptLinkedList_IF_T->GetListDataSize  = SF_GetListDataSize;
                ptLinkedList_IF_T->GetListCount     = SF_GetListCount;
                ptLinkedList_IF_T->isListTail       = SF_IsListTail;
                ptLinkedList_IF_T->isListHead       = SF_IsListHead;
                ptLinkedList_IF_T->GetNextList      = SF_GetNextList;
                ptLinkedList_IF_T->GetPrevList      = SF_GetPrevList;
            }
        }
        else
        {
            free(ptLinkedList_IF_T->ptListSt);
            free(ptLinkedList_IF_T);
            ptLinkedList_IF_T = NULL;
        }
    }
    else
    {
        free(ptLinkedList_IF_T);
        ptLinkedList_IF_T = NULL;
    }

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
            eError = SF_TermList(ptListIF->ptListSt);

            if(eError == ERROR_NONE)
            {
                free(ptListIF->ptListSt);
            }
        }
    }

    free(ptListIF);

    return eError;
}