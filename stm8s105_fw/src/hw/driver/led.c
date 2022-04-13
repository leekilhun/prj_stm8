#include "led.h"


#ifdef _USE_HW_LED



typedef struct  
{
  PORT_t  *port;
  uint8_t  pin_num;
  uint8_t  on_state;
} led_pin_t;

static const led_pin_t pin_tbl[LED_MAX_CH] = 
{
  {&sfr_PORTE, 5, _DEF_LOW}
};




bool ledInit(void)
{
  for (int i=0; i<LED_MAX_CH; i++)
  {
    pin_tbl[i].port->DDR.byte |= (1<<pin_tbl[i].pin_num); // Output
    pin_tbl[i].port->CR1.byte |= (1<<pin_tbl[i].pin_num); // Push-pull
  }

  return true;
}

void ledOn(uint8_t ch)
{
  PORT_t *port = pin_tbl[ch].port;

  if (pin_tbl[ch].on_state == _DEF_HIGH)
  {
    port->ODR.byte |= (1<<pin_tbl[ch].pin_num);    
  }
  else
  {    
    port->ODR.byte &= ~(1<<pin_tbl[ch].pin_num);    
  }
}

void ledOff(uint8_t ch)
{
  PORT_t *port = pin_tbl[ch].port;

  if (pin_tbl[ch].on_state == _DEF_HIGH)
  {
    port->ODR.byte &= ~(1<<pin_tbl[ch].pin_num);    
  }
  else
  {    
    port->ODR.byte |= (1<<pin_tbl[ch].pin_num);  
  }
}

void ledToggle(uint8_t ch)
{
  PORT_t *port = pin_tbl[ch].port;

  port->ODR.byte ^= (1<<pin_tbl[ch].pin_num);  
}

#endif