#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#include "M_Message.h"


M_Message_IF_T *M_CreateMessage(uint32_t a){

    M_Message_IF_T *ptRet_IF;

    ptRet_IF = (M_Message_IF_T*)calloc(1UL, sizeof(M_Message_IF_T));

    if(ptRet_IF != NULL){

        M_Error_E eError;

        ptRet_IF->ptMeesageSt = (M_MessageSt_T*)calloc(1UL, sizeof(M_MessageSt_T));

        if(ptRet_IF->ptMeesageSt != NULL){

            M_MessageSt_T *ptMessageSt = ptRet_IF->ptMeesageSt;

            ptMessageSt->ptSignal = M_CreateSignal();

        }
    }


    return ptRet_IF;
}