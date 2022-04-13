#include "bsp.h"



extern int32_t  uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);

static volatile uint32_t timer_tick = 0;


ISR_HANDLER(TIM4_ISR, _TIM4_OVR_UIF_VECTOR_) 
{
  timer_tick++;

  sfr_TIM4.SR.UIF = 0;
  return;
}




bool bspInit(void)
{
  DISABLE_INTERRUPTS();

  //-- CPU Clock
  //
  sfr_CLK.CKDIVR.HSIDIV = 0; // HSI 16Mhz / 1 = 16Mhz
  while(sfr_CLK.ICKR.HSIRDY != 1);

  //-- 1ms Timer
  //   16Mhz/64 * 250 = 1000us
  //
  sfr_TIM4.CR1.CEN  = 0;
  sfr_TIM4.CNTR.byte= 0;
  sfr_TIM4.CR1.ARPE = 1;
  sfr_TIM4.CR1.OPM  = 0;
  sfr_TIM4.CR1.URS  = 0;
  
  sfr_TIM4.PSCR.PSC = 6;  // 2^6 = 64
  sfr_TIM4.ARR.ARR  = 249;// 16Mhz/64 * 250 = 1000us = 1ms
  
  sfr_TIM4.EGR.byte= 0;   
  sfr_TIM4.IER.UIE = 1;   // Update interrupt enabled
  sfr_TIM4.CR1.CEN = 1;   // Counter Enable

  ENABLE_INTERRUPTS();

  return true;
}

#if 0
void delay(uint32_t ms)
{
  while (ms--)
      NOP();
}
#else
void delay(uint32_t ms)
{
  uint32_t pre_time = millis();

  while(millis()-pre_time < ms)
  {    
  }
}
#endif

uint32_t millis(void)
{
  uint32_t ret;

  DISABLE_INTERRUPTS();
  ret = timer_tick;
  ENABLE_INTERRUPTS();

  return ret;
}

void logPrintf(const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[128];


  len = vsprintf(print_buffer, fmt, arg);
  va_end (arg);

  uartWrite(_DEF_UART1, (uint8_t *)print_buffer, len);
}

void assert_failed(uint8_t* file, uint32_t line)
{
  (void)file;
  (void)line;
}



