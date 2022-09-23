#ifndef MESSAGELIB_H_
#define MESSAGELIB_H_

#define POST_OPT_REMOVE_DUPLICATED   0x00000001U
#define POST_OPT_PUSH_FIRST          0x00000002U

typedef struct
{
    void* pvParam;
    uint32_t uiMessage;
} Message_T;
typedef struct MessageSt_T MessageSt_T;

typedef int32_t (*pfnMessageHandler)(const Message_T *ptMessage);

typedef struct Message_IF_T{
    MessageSt_T *ptMessageSt;
    
    Error_E (*SendMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터
    Error_E (*PostMessage)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void* pvParam); //함수포인터
    Error_E (*PostMessageEX)(MessageSt_T *ptMessageSt, uint32_t uiMessage, void *pvParam, uint32_t uiOption);

} Message_IF_T;

Message_IF_T* CreateMessage(pfnMessageHandler fnMessageHandler);
Error_E DestroyMessage(Message_IF_T *ptMessageIF);
#endif