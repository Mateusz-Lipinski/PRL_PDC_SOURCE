#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "usart_utils.h"

extern state State;

void UART_PARSE(uint8_t buffer[UART_RX_BUFFER_LENGTH])
{
    SERIAL_WRITE(" => ");
    if (strcmp(buffer, "blink") == 0)
    {
        State.cmd = cmd_blink;
    }
    else if (strcmp(buffer, "adc id") == 0)
    {
        State.cmd = cmd_adc_id;
    }
    else if (strcmp(buffer, "adc reset") == 0)
    {
        State.cmd = cmd_adc_reset;
    }
    else if (strcmp(buffer, "adc debug") == 0)
    {
        State.cmd = cmd_adc_debug;
    }
    else if (strcmp(buffer, "adc data") == 0)
    {
        State.cmd = cmd_adc_data;
    }
    else if (strcmp(buffer, "adc values") == 0)
    {
        SERIAL_WRITE("\n");
        State.cmd = cmd_adc_values;
    }
    else if (strcmp(buffer, "temp") == 0)
    {
        State.cmd = cmd_temperature;
    }
    else
    {
        State.cmd = cmd_undefined;
    }
}