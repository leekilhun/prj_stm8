#include "uart.h"
#include "qbuffer.h"



#define UART_RX_Q_BUF_MAX     64



typedef struct 
{
  bool     is_open;
  uint32_t baud;

  UART2_t *p_uart;
  uint8_t rx_q_buf[UART_RX_Q_BUF_MAX];
  qbuffer_t rx_q;
} uart_tbl_t;


static uart_tbl_t uart_tbl[UART_MAX_CH];




bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 57600;

    qbufferCreate(&uart_tbl[i].rx_q, uart_tbl[i].rx_q_buf, UART_RX_Q_BUF_MAX);
  }

  return true;
}

bool uartDeInit(void)
{  
  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  uint32_t baud_div;


  switch(ch)
  {
    case _DEF_UART1:
      uart_tbl[ch].p_uart = &sfr_UART2;
      uart_tbl[ch].baud = baud;
      
      baud_div = (160000005/baud)/10;


      sfr_PORTD.DDR.DDR5 = 1; // Output
      sfr_PORTD.CR1.C15  = 1; // Push-pull

      sfr_PORTD.DDR.DDR6 = 0; // Input
      sfr_PORTD.CR1.C16  = 1; // Pull-up

      sfr_UART2.CR1.byte = 0x00;
      sfr_UART2.CR2.byte = 0x00;
      sfr_UART2.CR3.byte = 0x00;

      sfr_UART2.BRR1.UART_DIV  = (baud_div >> 4) & 0xFF;
      sfr_UART2.BRR2.UART_DIV  = (baud_div >> 0) & 0x0F;
      sfr_UART2.BRR2.UART_DIV |= (baud_div >> 8) & 0xF0;

      sfr_UART2.CR1.M    = 0;    // 1 Start bit, 8 Data bits, n Stop bit
      sfr_UART2.CR1.PCEN = 0;    // Parity control disabled      
      sfr_UART2.CR2.TEN  = 1;    // Transmitter enable
      sfr_UART2.CR2.REN  = 1;    // Receiver enable      
      sfr_UART2.CR3.STOP = 0;    // 1 Stop bit      
      
      sfr_UART2.CR2.RIEN = 1;   // Receiver interrupt enable

      uart_tbl[ch].is_open = true;
      ret = true;
      break;
  }

  return ret;
}

bool uartIsOpen(uint8_t ch)
{
  return uart_tbl[ch].is_open;
}

bool uartClose(uint8_t ch)
{
  UART2_t *p_uart = uart_tbl[ch].p_uart;

  p_uart->CR2.TEN = 0;    // Transmitter disable
  p_uart->CR2.REN = 0;    // Receiver disable  
  uart_tbl[ch].is_open = false;
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  return qbufferAvailable(&uart_tbl[ch].rx_q);
}

bool uartFlush(uint8_t ch)
{
  qbufferFlush(&uart_tbl[ch].rx_q);
  return true;
}

void uartPutch(uint8_t ch, uint8_t data)
{
  uartWrite(ch, &data, 1);
}

uint8_t uartGetch(uint8_t ch)
{
  uint8_t ret = 0;

  while(1)
  {
    if (uartAvailable(ch) > 0)
    {
      ret = uartRead(ch);
      break;
    }
  }

  return ret;
}

int32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  int32_t ret = 0;
  uint32_t pre_time;
  uint8_t sent_i;
  UART2_t *p_uart = uart_tbl[ch].p_uart;

  sent_i = 0;
  pre_time = millis();
  while(sent_i < length)
  {
    if (sfr_UART2.SR.TXE)
    {
      sfr_UART2.DR.byte = p_data[sent_i];
      sent_i++;      
    }

    if (millis()-pre_time > 100)
    {
      break;
    }
  }

  ret = sent_i;

  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret;

  qbufferRead(&uart_tbl[ch].rx_q, &ret, 1);

  return ret;
}



int32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[128];  
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsprintf(buf, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);
  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  return uart_tbl[ch].baud;
}




ISR_HANDLER(UART2_RXD, _UART2_R_RXNE_VECTOR_)
{
  if (sfr_UART2.SR.RXNE == 1)
  {
    uint8_t rx_data;

    rx_data = sfr_UART2.DR.byte;
    qbufferWrite(&uart_tbl[_DEF_UART1].rx_q, &rx_data, 1);
    sfr_UART2.SR.RXNE = 0;
  }
}