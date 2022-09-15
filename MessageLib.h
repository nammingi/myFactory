#ifndef MESSAGELIB_H_
#define MESSAGELIB_H_

typedef struct Message_T Message_T;

typedef int32_t (*pfnMessageHandler)(Message_T *ptMessage);

#endif