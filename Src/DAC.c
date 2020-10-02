#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "spi.h"
#include "DAC.h"
#include "usart_utils.h"
#include "main.h"

extern state State;

// Write 24b to DAC over SPI
void DAC_SPI_WRITE_24(uint32_t data)
{
    uint8_t txbuffer[3] = {
        (uint8_t)((data & 0xff0000) >> 16),
        (uint8_t)((data & 0xff00) >> 8),
        (uint8_t)((data & 0xff))};
    HAL_SPI_Transmit(&hspi1, txbuffer, 3, HAL_MAX_DELAY);
}

// Reset DAC
void DAC_reset()
{
    HAL_GPIO_WritePin(DAC_nSYNC_GPIO_Port, DAC_nSYNC_Pin, 1);
    HAL_GPIO_WritePin(DAC_nLOAD_GPIO_Port, DAC_nLOAD_Pin, 1);
    HAL_GPIO_WritePin(DAC_nRESET_GPIO_Port, DAC_nRESET_Pin, 0);
    HAL_Delay(10);
    HAL_GPIO_WritePin(DAC_nRESET_GPIO_Port, DAC_nRESET_Pin, 1);
}

// Write command to DAC over SPI
void DAC_cmd(uint32_t data)
{
    HAL_GPIO_WritePin(DAC_nSYNC_GPIO_Port, DAC_nSYNC_Pin, 0);
    DAC_SPI_WRITE_24(data);
    HAL_GPIO_WritePin(DAC_nSYNC_GPIO_Port, DAC_nSYNC_Pin, 1);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DAC_nLOAD_GPIO_Port, DAC_nLOAD_Pin, 0);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DAC_nLOAD_GPIO_Port, DAC_nLOAD_Pin, 1);
}

//Change DAC values in loop for simple voltage meter check
void DAC_test()
{
    for (int i = 0; i <= 0xffff; i += 0xffff / 5)
    {
        DAC_cmd(DAC_CH(0) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(1) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(2) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(3) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(4) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(5) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(6) + DAC_WRITE + (uint32_t)(i));
        DAC_cmd(DAC_CH(7) + DAC_WRITE + (uint32_t)(i));
        SERIAL_WRITE("0x%04x\n", i);
        HAL_Delay(1000);
    }
};

// Change all dac values for testing
void DAC_test_val(uint32_t val)
{
    DAC_cmd(DAC_CH(0) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(1) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(2) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(3) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(4) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(5) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(6) + DAC_WRITE + val);
    DAC_cmd(DAC_CH(7) + DAC_WRITE + val);
};