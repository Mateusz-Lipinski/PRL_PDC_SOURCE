#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "spi.h"
#include "ADC.h"
#include "usart_utils.h"
#include "main.h"

extern state State;

// Write 8b to ADC over SPI and return 8b output
uint8_t ADC_SPI_WRITE_8(uint8_t data)
{
    uint8_t txbuffer[1] = {data};
    uint8_t rxbuffer[1] = {0};
    HAL_SPI_TransmitReceive(&hspi2, txbuffer, rxbuffer, 1, HAL_MAX_DELAY);
    uint8_t answer = rxbuffer[0];
    return answer;
}
// Write 16b to ADC over SPI and return 16b output
uint8_t ADC_SPI_WRITE_16(uint16_t data)
{
    uint8_t txbuffer[2] = {
        (uint8_t)((data & 0xff00) >> 8),
        (uint8_t)((data & 0x00ff))};
    uint8_t rxbuffer[2] = {0};
    HAL_SPI_TransmitReceive(&hspi2, txbuffer, rxbuffer, 2, HAL_MAX_DELAY);
    uint8_t answer = rxbuffer[0];
    return answer;
}

// Read 16b from ADC over SPI
uint16_t ADC_SPI_READ_16()
{
    uint8_t rxbuffer[2] = {0};
    HAL_SPI_Receive(&hspi2, rxbuffer, 2, HAL_MAX_DELAY);
    uint16_t answer = (((uint16_t)rxbuffer[0]) << 8) | ((uint16_t)rxbuffer[1]);
    return answer;
}

// Read 24b from ADC over SPI
uint32_t ADC_SPI_READ_24()
{
    uint8_t rxbuffer[3] = {0};
    HAL_SPI_Receive(&hspi2, rxbuffer, 3, HAL_MAX_DELAY);
    uint32_t answer = (((uint32_t)rxbuffer[0]) << 16) | (((uint32_t)rxbuffer[1]) << 8) | ((uint32_t)rxbuffer[2]);
    return answer;
}

// Read 8b from ADC over SPI
uint8_t ADC_SPI_READ_8()
{
    uint8_t rxbuffer[1] = {0};
    HAL_SPI_Receive(&hspi2, rxbuffer, 1, HAL_MAX_DELAY);
    uint8_t answer = rxbuffer[0];
    return answer;
}

// Send command to ADC
uint8_t ADC_CMD(uint8_t read, uint8_t addr)
{
    uint8_t command = read | addr;
    ADC_SPI_WRITE_8(command);
    return command;
}

// Get ADC chip ID
uint16_t ADC_ID()
{
    ADC_DATA_READY_EXTI_deInit();
    HAL_SPI_MspInit(&hspi2);
    ADC_CMD(ADC_READ, ADC_ID_REG);
    uint16_t answer = ADC_SPI_READ_16();
    HAL_SPI_MspDeInit(&hspi2);
    ADC_DATA_READY_EXTI_Init();
    return answer;
}

// Read ADC data register
uint32_t ADC_DATA()
{
    ADC_CMD(ADC_READ, ADC_DATA_REG);
    uint32_t answer = ADC_SPI_READ_24();
    return answer;
}

// ASYNC ADC update. Use in ADC Data Ready inturrupt
void ADC_update()
{

    ADC_DATA_READY_EXTI_deInit(); // ADC SPI pins are used both to communicate and as data ready EXTI.
    HAL_SPI_MspInit(&hspi2);      // Switch to SPI
    if (State.ADC_Channel == ADC_DEFAULT_CH)
    {
        HAL_GPIO_WritePin(ADC_nSYNC_GPIO_Port, ADC_nSYNC_Pin, 0); // Initial start conversions
        HAL_Delay(1);                                             //
        HAL_GPIO_WritePin(ADC_nSYNC_GPIO_Port, ADC_nSYNC_Pin, 1); //
        State.ADC_Channel = 0;
    }
    else
    {
        ADC_CMD(ADC_READ, ADC_STATUS_REG);                                 // Read current channel
        State.ADC_Channel = ADC_SPI_READ_8() & 0xf;                        //
        ADC_CMD(ADC_READ, ADC_DATA_REG);                                   //  Read data
        State.ADC_Values[State.ADC_Channel] = (uint32_t)ADC_SPI_READ_24(); //
    }
    if (State.ADC_Channel == 15)
    {
        State.ADC_Updated = 1; // Set flag when every channel has been read
    }
    HAL_SPI_MspDeInit(&hspi2);  // ADC SPI pins are used both to communicate and as data ready EXTI.
    ADC_DATA_READY_EXTI_Init(); // Switch to EXTI
}

void ADC_config(ADC_SPS sps)
{
    ADC_DATA_READY_EXTI_deInit(); // ADC SPI pins are used both to communicate and as data ready EXTI.
    HAL_SPI_MspInit(&hspi2);      // Switch to SPI
    for (int i = 0; i < 16; i++)
    {
        ADC_CMD(ADC_WRITE, ADC_CHx_REG(i));                       // Configure channel multiplexer
        ADC_SPI_WRITE_16(ADC_CH_EN + ADC_AINPOS(i) + ADC_AINNEG); //
    }

    ADC_CMD(ADC_WRITE, ADC_SETUPCONx_REG(0));                                         // Set ADC config
    ADC_SPI_WRITE_16(ADC_SETUP_BI_UNIPOLAR0 + ADC_SETUP_REF_BUF + ADC_SETUP_AIN_BUF); // Input as unipolar & buffers enabled
    ADC_CMD(ADC_WRITE, ADC_FILTCONx_REG(0));                                          //
    ADC_SPI_WRITE_16(sps);                                                            // Conversion rate
    State.ADC_Channel = ADC_DEFAULT_CH;                                               // Init current channel
    HAL_SPI_MspDeInit(&hspi2);                                                        // ADC SPI pins are used both to communicate and as data ready EXTI.
    ADC_DATA_READY_EXTI_Init();                                                       // Switch to EXTI
}

void ADC_reset() // Reset ADC by pulsing clocks without data
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ADC_MOSI_Pin | ADC_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(ADC_MOSI_GPIO_Port, ADC_MOSI_Pin, 1);
    for (int i = 0; i < 200; i++)
    {
        HAL_GPIO_TogglePin(ADC_SCK_GPIO_Port, ADC_SCK_Pin);
        HAL_Delay(1);
    }
    HAL_SPI_MspInit(&hspi2);
};
