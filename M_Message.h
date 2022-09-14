#ifndef M_MESSAGE_H_
#define M_MESSAGE_H_

typedef enum{
    ERROR_NONE=0,
    ERROR_BAD_PARAMETER,
    ERROR_BAD_REQUEST,
} M_Error_E;


typedef struct {
    uint32_t Message;
} M_Message_T;

typedef struct {
    pthread_cond_t *pThreadCond; 
    pthread_mutex_t *pThreadMutex;
}M_Signal_T;

typedef struct {
    pthread_t tHead;
    M_Signal_T *ptSignal;


} M_MessageSt_T;

typedef struct{ 
    M_MessageSt_T *ptMeesageSt;

    M_Error_E (*SendMessage)(  );
    M_Error_E (*PostMessage)(uint32_t Message);

} M_Message_IF_T;


M_Message_IF_T *M_CreateMessage(uint32_t a);
M_Error_E M_DestroyMessage();



#endif