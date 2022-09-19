#include <pthread.h>

#include "Common.h"
#include "Signal.h"

struct Signal_T {
    BOOL_E bSignaled;

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

    ptSignal->bSignaled = FALSE;

    if(ptSignal != NULL){
        int iRetVal = NULL; 
        
        while((ptSignal->bSignaled==FALSE) && (iRetVal == 0)){
            iRetVal = pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);
        }

        ptSignal->bSignaled=FALSE;  

        if(iRetVal != 0){
            eError = ERROR_THREAD_ERROR;
        }
    }else{
        eError = ERROR_BAD_PARAMETER;
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
    if(ptSignal != NULL){
        (void)pthread_mutex_lock(&ptSignal->tMutex);
        printf("locked and wait....");
        eError = SF_SignalWait(ptSignal);
        printf("being..\n");
        (void)pthread_mutex_unlock(&ptSignal->tMutex);
    }else{
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}

Error_E SignalWakeup(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;

    int iRetVal = NULL;

    if(ptSignal != NULL){
        (void)pthread_mutex_lock(&ptSignal->tMutex);

        iRetVal = pthread_cond_signal(&ptSignal->tCond);

        if(iRetVal != 0){
            eError = ERROR_SIGNAL;
        }
        ptSignal->bSignaled = TRUE;

        (void)pthread_mutex_unlock(&ptSignal->tMutex);

    }else{
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

Error_E SignalBareWait(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    
    if(ptSignal != NULL){
        eError = SF_SignalWait(ptSignal);
    }else{
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}

Error_E SignalBareWakeup(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    int iRetVal = NULL;

    if(ptSignal != NULL){
        iRetVal = pthread_cond_signal(&ptSignal->tCond);

        if(iRetVal != 0){
            eError = ERROR_SIGNAL;
        }
        ptSignal->bSignaled = TRUE;

    }else{
        eError = ERROR_BAD_PARAMETER;
    }
    return eError;
}

Signal_T *CreateSignal(void){
    Signal_T *ptSignal;

    Error_E eError;
    ptSignal = (Signal_T*)calloc(1UL, sizeof(Signal_T));

    eError = SF_InitCond(ptSignal);

    eError = SF_InitMutex(ptSignal);

    return ptSignal;

}

Error_E DestroySignal(Signal_T *ptSignal){
    Error_E eError = ERROR_NONE;
    
    if(ptSignal != NULL){

        (void)pthread_cond_destroy(&ptSignal->tCond);
        (void)pthread_mutex_destroy(&ptSignal->tMutex);

        free(ptSignal);

    }else{
        eError = ERROR_BAD_PARAMETER;
    }

    return eError;
}