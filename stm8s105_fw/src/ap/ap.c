#include "ap.h"





void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);  
}

void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);    
    }    

    /*
    if (uartAvailable(_DEF_UART1) > 0)
    {
      uint8_t rx_data;

      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "rx : 0x%X\n", rx_data);
    }
    */
    cliMain();
  }
}

