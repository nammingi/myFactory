#ifndef SIGNAL_H_
#define SIGNAL_H_


typedef struct Signal_T Signal_T;

Error_E SignalLock(Signal_T *ptSignal);
Error_E SignalUnLock(Signal_T *ptSignal);
Error_E SignalWait(Signal_T *ptSignal);
Error_E SignalWakeup(Signal_T *ptSignal);
Error_E SignalBareWait(Signal_T *ptSignal);
Error_E SignalBareWakeup(Signal_T *ptSignal);

Signal_T *CreateSignal(void);
Error_E DestroySignal(Signal_T *ptSignal);

#endif