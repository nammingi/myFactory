#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Common.h"
#include "LinkedList.h"

struct LinkedList_T{
    void * pvData;
    char * pstrListName; 
    

    uint32_t iDataSize;

    LinkedList_T *ptPrev;
    LinkedList_T *ptNext;
};

struct LinkedListSt_T{
    LinkedList_T *ptHead;
    LinkedList_T *ptTail;
};

static Error_E SF_Init_LinkedList(LinkedListSt_T *ptListSt){
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

static BOOL_E SF_IsListTail(LinkedListSt_T *ptListSt, LinkedList_T *ptList){
    BOOL_E bRetVal = FALSE;

    bRetVal = (ptListSt->ptTail == ptList) ? TRUE : FALSE ;

    return bRetVal;
}


static LinkedList_T *SF_GetFisrtList(LinkedListSt_T *ptListSt){
    LinkedList_T *ptFisrtItem = NULL;

    if(SF_IsListTail(ptListSt, ptListSt->ptHead->ptNext) == FALSE){
        ptFisrtItem = ptListSt->ptHead->ptNext;
    }

    return ptFisrtItem;
}


static char* SF_GetListName(LinkedList_T *ptList){
    //char* pstrListName;
    if(ptList != NULL){
        return ptList->pstrListName;
    }
}


static Error_E SF_SetListName(LinkedList_T *ptList, const char* pcstrListName){
    Error_E RetError = ERROR_NONE;

    size_t iStrLen = strlen(pcstrListName);

    ptList->pstrListName = (char*)calloc(iStrLen, sizeof(char));

    (void)strcpy(ptList->pstrListName, pcstrListName);

    return RetError;
}

static Error_E SF_SetListData(LinkedList_T *ptList, void* pvData, uint32_t iDataSize){
    Error_E RetError = ERROR_NONE;
    
    ptList->pvData = calloc(1UL, sizeof(iDataSize));
    (void)memcpy(ptList->pvData, pvData, iDataSize);
    ptList->iDataSize = iDataSize;

    return RetError;
}

static void *SF_GetListData(LinkedList_T *ptList){
    void *pvData = NULL;

}
static uint32_t SF_GetListDataSize(LinkedList_T *ptList){
    uint32_t riDataSize = 0UL;

    riDataSize = ptList->iDataSize;

    return riDataSize;
}
static LinkedList_T *SF_InsertList(LinkedListSt_T *ptListSt, char* pstrListName, void* pvData, uint32_t iDataSize){
    //attach to before Tail node.
    Error_E rError = ERROR_NONE;

    LinkedList_T *ptNewList = NULL;
    ptNewList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));
    SF_SetListName(ptNewList, pstrListName);
    SF_SetListData(ptNewList, pvData, iDataSize);

    LinkedList_T *ptOldList = ptListSt->ptTail;
    ptOldList->ptPrev->ptNext = ptNewList;
    ptNewList->ptPrev         = ptOldList->ptPrev;
    ptOldList->ptPrev         = ptNewList;
    ptNewList->ptNext         = ptOldList;

    rError = SF_SetListName(ptNewList, pstrListName);

    rError = SF_SetListData(ptNewList, pvData, iDataSize);

    return ptNewList;
}

static LinkedList_T *SF_InsertListFirst(LinkedListSt_T *ptListSt, char* pstrListName, void* pvData, uint32_t iDataSize){
    
    Error_E rError = ERROR_NONE;

    LinkedList_T *ptNewFirstList;
    ptNewFirstList = (LinkedList_T *)calloc(1UL, sizeof(LinkedList_T));
    SF_SetListName(ptNewFirstList, pstrListName);
    SF_SetListData(ptNewFirstList, pvData, iDataSize);

    LinkedList_T *ptOldList = ptListSt->ptHead;
    ptOldList->ptNext->ptPrev       = ptNewFirstList;
    ptNewFirstList->ptPrev          = ptOldList;
    ptNewFirstList->ptNext          = ptOldList->ptNext;
    ptOldList->ptNext               = ptNewFirstList;
    

    rError = SF_SetListName(ptNewFirstList, pstrListName);

    rError = SF_SetListData(ptNewFirstList, pvData, iDataSize);
    
    return ptNewFirstList;
}

static Error_E SF_DeleteListData(LinkedList_T *ptList){
    Error_E rError = ERROR_NONE;

    ptList->pvData = NULL;
    ptList->iDataSize = 0UL;

    free(ptList->pvData);

    return rError;
}

static Error_E SF_DeleteListName(LinkedList_T *ptList){
    Error_E rError = ERROR_NONE;

    ptList->pstrListName = NULL;
    
    free(ptList->pstrListName);

    return rError;
}

static Error_E SF_DeleteList(LinkedList_T *ptList){
    Error_E rError = ERROR_NONE;

    ptList->ptPrev->ptNext = ptList->ptNext;
    ptList->ptNext->ptPrev = ptList->ptPrev;

    rError = SF_DeleteListData(ptList);

    rError = SF_DeleteListName(ptList);

    free(ptList);

    return rError;
}

static Error_E SF_TermList(LinkedListSt_T *ptListSt){
    
    Error_E rError = ERROR_NONE;

    ptListSt->ptHead = NULL;
    free(ptListSt->ptHead);

    ptListSt->ptTail = NULL;
    free(ptListSt->ptTail);

    return rError;
}

static Error_E SF_PrintListAll(LinkedListSt_T *ptListSt){
    Error_E rError = ERROR_NONE;

    LinkedList_T *ptList;
    void *pvIntData;

    ptList = ptListSt->ptHead;
    pvIntData = (int*)calloc(1UL, sizeof(int));

    int* data;
    while(SF_IsListTail(ptListSt, ptList) == FALSE){

        printf("check\n");

        //(void)memcpy(pvIntData,ptList->pvData,sizeof(int));
        memset(pvIntData, data, sizeof(int));
        printf(">> %d \n", pvIntData);
        ptList = ptList->ptNext;
    }
    printf(">> End of list.\n");
    free(pvIntData);
    
    return rError;

}
LinkedList_IF_T *CreateList(void){
    Error_E iError;
    LinkedList_IF_T *ptLinkedList_IF_T;
    ptLinkedList_IF_T = (LinkedList_IF_T *)calloc(1UL, sizeof(LinkedList_IF_T));
    
    ptLinkedList_IF_T->ptListSt = (LinkedListSt_T *)calloc(1UL, sizeof(LinkedListSt_T));

    iError = SF_Init_LinkedList(ptLinkedList_IF_T->ptListSt);

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
    ptLinkedList_IF_T->PrintListAll     = SF_PrintListAll;

    

    return ptLinkedList_IF_T;
}