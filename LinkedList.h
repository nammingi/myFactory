#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#define REMOVE_DUPLICATED 0x00000001U
#define PUSH_FIRST 0x000002U

typedef struct LinkedList_T LinkedList_T;
typedef struct LinkedListSt_T LinkedListSt_T;

typedef struct {
    LinkedListSt_T *ptListSt;

    LinkedList_T *(*InsertList)(LinkedListSt_T *ptList, const char* pstrListName, void* pvData, uint32_t iDataSize);
    LinkedList_T *(*InsertListFirst)(LinkedListSt_T *ptListSt, const char* pstrListName, void* pvData, uint32_t iDataSize);
    LinkedList_T *(*GetFirstList)(const LinkedListSt_T *ptListSt);
    Error_E (*SetListData)(LinkedList_T *ptList, void* pvData, uint32_t iDataSize);
    Error_E (*DeleteList)(LinkedList_T *ptList);
    Error_E (*TermList)(LinkedListSt_T *ptListSt);
    char*   (*GetListName)(LinkedList_T *ptList);
    Error_E (*SetListName)(LinkedList_T *ptList, const char* pctstrListName);
    void *(*GetListData)(LinkedList_T *ptList);
    uint32_t(*GetListDataSize)(LinkedList_T *ptList);
    int (*GetListCount)(LinkedListSt_T *ptListSt);
    Error_E (*PrintListAll)(LinkedListSt_T *ptListSt);
    BOOL_E (*isListTail)(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList);
    BOOL_E (*isListHead)(const LinkedListSt_T *ptListSt, const LinkedList_T *ptList);
    LinkedList_T *(*GetNextList)(const LinkedListSt *ptListSt, const LinkedList_T *ptList);
    LinkedList_T *(*GetPrevList)(const LinkedListSt *ptListSt, const LinkedList_T *ptList);

} LinkedList_IF_T;

LinkedList_IF_T *CreateList(void);
Error_E DestroyList(LinkedList_IF_T *ptListIF);
#endif