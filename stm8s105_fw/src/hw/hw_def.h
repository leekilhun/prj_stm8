#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_


#include "def.h"
#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V220331R1"
#define _DEF_BOARD_NAME           "STM8S105C4"





#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    8
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    2
#define      HW_CLI_LINE_BUF_MAX    64


#endif