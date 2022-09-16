#include <pthread.h>

#include "Common.h"
#include "Signal.h"

struct Signal_T {
    pthread_cond_t tCond;
    pthread_mutex_t tMutex;
};


static Error_E SF_InitCond(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal;
    pthread_condattr_t tCondAttr;

    iRetVal = pthread_condattr_init(&tCondAttr);

    if(iRetVal != 0){
        pthread_cond_init(&ptSignal->tCond,&tCondAttr);
    }
    pthread_condattr_destroy(&tCondAttr);
    return eError;

}

static Error_E SF_InitMutex(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    pthread_mutexattr_t tMutexAttr;
    pthread_mutexattr_init(&tMutexAttr);
    pthread_mutex_init(&ptSignal->tMutex, &tMutexAttr);
    return eError;
}

static Error_E SF_SignalWait(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL; 
    iRetVal = pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);
    if(iRetVal != 0){
        eError = ERROR_THREAD_ERROR;
    }
    return eError;
}



Error_E SignalLock(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL;
    iRetVal = pthread_mutex_lock(&ptSignal->tMutex);
    if(iRetVal != 0){
        eError = ERROR_THREAD_ERROR;
    }
    return eError;
}

Error_E SignalUnLock(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL;
    iRetVal = pthread_mutex_unlock(&ptSignal->tMutex);
    if(iRetVal != 0){
        eError = ERROR_THREAD_ERROR;
    }
    return eError;
}

Error_E SignalWait(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    pthread_mutex_lock(&ptSignal->tMutex);
    eError = SF_SignalWait(ptSignal);
    pthread_mutex_unlock(&ptSignal->tMutex);
    return eError;
}

Error_E SignalWakeup(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    pthread_mutex_lock(&ptSignal->tMutex);

    pthread_cond_signal(&ptSignal->tCond);

    pthread_mutex_unlock(&ptSignal->tMutex);
    return eError;
}

Error_E SignalBareWait(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL;
    iRetVal = pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);
    if(iRetVal != 0){
        eError = ERROR_THREAD_ERROR;
    }
    

    return eError;
}

Error_E SignalBareWakeup(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL;
    iRetVal = pthread_cond_signal(&ptSignal->tCond);
    if(iRetVal != 0){
        eError = ERROR_THREAD_ERROR;
    }
    return eError;
}

Signal_T *CreateSignal(void){
    Signal_T *ptSignal;

    Error_E iError;
    ptSignal = (Signal_T*)calloc(1UL, sizeof(Signal_T));

    iError = SF_InitCond(ptSignal);

    iError = SF_InitMutex(ptSignal);

    return ptSignal;

}