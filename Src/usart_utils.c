#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "usart_utils.h"

extern state State;
extern uint8_t UART_RX_BUFF[UART_RX_BUFFER_LENGTH];
extern uint8_t COMMAND;

void UART_PARSE(uint8_t buffer[UART_RX_BUFFER_LENGTH])
{
    SERIAL_WRITE(" => ");
    if (strcmp(buffer, "blink") == 0)
    {
        COMMAND = cmd_blink;
    }
    else if (strcmp(buffer, "adc id") == 0)
    {
        COMMAND = cmd_adc_id;
    }
    else if (strcmp(buffer, "adc reset") == 0)
    {
        COMMAND = cmd_adc_reset;
    }
    else if (strcmp(buffer, "adc debug") == 0)
    {
        COMMAND = cmd_adc_debug;
    }
    else if (strcmp(buffer, "adc data") == 0)
    {
        COMMAND = cmd_adc_data;
    }
    else
    {
        COMMAND = cmd_undefined;
    }
}