#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct LinkedList_T LinkedList_T;
typedef struct LinkedListSt_T LinkedListSt_T;

typedef struct {
    LinkedListSt_T *ptListSt;

    LinkedList_T *(*InsertList)(LinkedListSt_T *ptList, char* pstrListName, void* pvData, uint32_t iDataSize);
    LinkedList_T *(*InsertListFisrt)(LinkedListSt_T *ptListSt, char* pstrListName, void* pvData, uint32_t iDataSize);
    LinkedList_T *(*GetFirstList)(LinkedListSt_T *ptListSt);
    Error_E (*SetListData)(LinkedList_T *ptList, void* pvData, uint32_t iDataSize);
    Error_E (*DeleteList)(LinkedList_T *ptList);
    Error_E (*TermList)(LinkedListSt_T *ptListSt);
    char*   (*GetListName)(LinkedList_T *ptList);
    Error_E (*SetListName)(LinkedList_T *ptList, const char* pctstrListName);
    //Error_E (*DeleteListName)(LinkedList_T *ptList);
    //Error_E (*DeleteListData)(LinkedList_IF_T *ptList);

    void *(*GetListData)(LinkedList_T *ptList);
    uint32_t(*GetListDataSize)(LinkedList_T *ptList);
    int (*GetListCount)(LinkedListSt_T *ptListSt);
    Error_E (*PrintListAll)(LinkedListSt_T *ptListSt);

} LinkedList_IF_T;

LinkedList_IF_T *CreateList(void);
Error_E DestroyList(LinkedList_IF_T *ptListIF);
#endif