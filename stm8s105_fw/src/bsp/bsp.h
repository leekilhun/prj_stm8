#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"
#include "device/include/STM8S105K4.h"




bool bspInit(void);
bool bspDeInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);


void logPrintf(const char *fmt, ...);



ISR_HANDLER(TIM4_ISR, _TIM4_OVR_UIF_VECTOR_);
ISR_HANDLER(UART2_RXD, _UART2_R_RXNE_VECTOR_);


#ifdef __cplusplus
}
#endif

#endif