#ifndef MESSAGELIB_H_
#define MESSAGELIB_H_

typedef struct Message_T Message_T;
typedef struct MessageSt_T MessageSt_T;

typedef int32_t (*pfnMessageHandler)(Message_T *ptMessage);

typedef struct Message_IF_T{
    MessageSt_T *ptMessageSt;
    
    Error_E (*SendMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터
    Error_E (*PostMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터

} Message_IF_T;

Message_IF_T* CreateMessage(pfnMessageHandler fnMessageHandler);
Error_E DestroyMessage(Message_IF_T *ptMessageIF);
#endif