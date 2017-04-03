#ifndef FSM_H_
#define FSM_H_

#include <UART.h>
#include "stm32f0xx.h"
#include "Buffer.h"

Buffer inputBuffer;

/* Public function prototypes -----------------------------------------------*/

extern void FSM_Init();
extern void FSM(void *dummy);

#endif // FSM_H_

