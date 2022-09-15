#include <pthread.h>

#include "Common.h"
#include "Signal.h"

struct Signal_T {
    pthread_cond_t tCond;
    pthread_mutex_t tMutex;
};


static Error_E SF_InitCond(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    
    pthread_condattr_t tCondAttr;
    pthread_condattr_init(&tCondAttr);
    pthread_cond_init(&ptSignal->tCond,&tCondAttr);

    pthread_cond_destroy(&tCondAttr);
    return rError;

}

static Error_E SF_SignalWait(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);
    return rError;
}

Error_E SignalLock(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    pthread_mutex_lock(&ptSignal->tMutex);
    return rError;
}

Error_E SignalUnLock(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    pthread_mutex_unlock(&ptSignal->tMutex);
    return rError;
}

Error_E SignalWait(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    pthread_mutex_lock(&ptSignal->tMutex);

    SF_SignalWait(ptSignal);

    pthread_mutex_unlock(&ptSignal->tMutex);
    return rError;
}

Error_E SignalWakeup(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;
    pthread_mutex_lock(&ptSignal->tMutex);

    pthread_cond_signal(&ptSignal->tCond);

    pthread_mutex_unlock(&ptSignal->tMutex);
    return rError;
}

Error_E SignaBarelWait(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;

    pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);

    return rError;
}

Error_E SignalBareWakeup(Signal_T *ptSignal){
    Error_E rError = ERROR_NONE;

    pthread_cond_signal(&ptSignal->tCond);

    return rError;
}

static Error_E SF_InitMutex(Signal_T *ptSingal){
    Error_E rError = ERROR_NONE;
    pthread_mutexattr_t tMutexAttr;
    pthread_mutexattr_init(&tMutexAttr);
    pthread_mutex_init(&ptSignal->tMutex, &tMutexAttr);
    return rError;
}