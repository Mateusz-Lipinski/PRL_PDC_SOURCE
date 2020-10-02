#ifndef __ADC_H
#define __ADC_H
#include "main.h"
#include "gpio.h"

#define ADC_DATA_READY !HAL_GPIO_ReadPin(ADC_MISO_GPIO_Port, ADC_MISO_Pin)

#define ADC_WRITE 0                                      // commands:
#define ADC_READ 0x40                                    //
#define ADC_CH_EN 0x8000                                 //
#define ADC_AINPOS(i) ((0 <= i <= 15) ? (i << 5) : NULL) // Inputs addresses
#define ADC_AINNEG 0b10110                               // GND input address (by design)
#define ADC_SETUP_BI_UNIPOLAR0 0                         // Set inputs as unipolar

#define ADC_SETUP_REF_BUF 0xC00                                       // Registers addreses
#define ADC_SETUP_AIN_BUF 0x300                                       //
#define ADC_STATUS_REG 0x00                                           //
#define ADC_MODE_REG 0x01                                             //
#define ADC_INTERFACE_REG 0x02                                        //
#define ADC_CHECK_REG 0x03                                            //
#define ADC_DATA_REG 0x04                                             //
#define ADC_GPIO_REG 0x06                                             //
#define ADC_ID_REG 0x07                                               //
#define ADC_CHx_REG(i) ((0 <= i <= 15) ? (0x10 + i) : NULL)           //
#define ADC_SETUPCONx_REG(i) ((0 <= i && i <= 7) ? (0x20 + i) : NULL) //
#define ADC_FILTCONx_REG(i) ((0 <= i && i <= 7) ? (0x28 + i) : NULL)  //
#define ADC_OFFSETx_REG(i) ((0 <= i && i <= 7) ? (0x30 + i) : NULL)   //
#define ADC_GAINx_REG(i) ((0 <= i && i <= 7) ? (0x38 + i) : NULL)     //

//Channel definitions:
#define ADC_PD0_Power_CH 12  // MO352 vs FREQDIV MO704(/2)
#define ADC_PD0_Phase_CH 11  // MO352 vs MO704(/2)
#define ADC_PD1_Power_CH 5   // MO704 vs AMP704
#define ADC_PD1_Phase_CH 6   // MO704 vs AMP704
#define ADC_PD2_Power_CH 1   // MO704 vs STUB704
#define ADC_PD2_Phase_CH 0   // MO704 vs STUB704
#define ADC_PD3_Power_CH 13  // MO352 vs AMP352
#define ADC_PD3_Phase_CH 14  // MO352 vs AMP352
#define ADC_PD4_Power_CH 3   // MO352 vs STUB352
#define ADC_PD4_Phase_CH 2   // MO352 vs STUB352
#define ADC_PWD0_Power_CH 4  // MO704P
#define ADC_PWD1_Power_CH 10 // AMP704P
#define ADC_PWD2_Power_CH 9  // AMP352P
#define ADC_PWD3_Power_CH 15 // MO352P
#define ADC_T_CH 7
#define ADC_RH_CH 8
#define ADC_DEFAULT_CH 16

#define RF_REL_POWER_MO352_MOFREQDIV352 12 // MO352 vs FREQDIV MO704(/2)
#define RF_PHASE_MO352_MOFREQDIV352 11     // MO352 vs MO704(/2)
#define RF_REL_POWER_MO704_AMP704 5        // MO704 vs AMP704
#define RF_PHASE_MO704_AMP704 6            // MO704 vs AMP704
#define RF_REL_POWER_MO704_STUB704 1       // MO704 vs STUB704
#define RF_PHASE_MO704_STUB704 0           // MO704 vs STUB704
#define RF_REL_POWER_MO352_AMP352 13       // MO352 vs AMP352
#define RF_PHASE_MO352_AMP352 14           // MO352 vs AMP352
#define RF_REL_POWER_MO352_STUB352         // MO352 vs STUB352
#define RF_PHASE_MO352_STUB352 2           // MO352 vs STUB352
#define RF_POWER_MO704 4                   // MO704P
#define RF_POWER_AMP704 10                 // AMP704P
#define RF_POWER_AMP352 9                  // AMP352P
#define RF_POWER_MO352 15                  // MO352P

typedef enum // Sampling rate definitions:
{
    ADC_SPS_31250 = 0, //  31250 Sps
    ADC_SPS_15625 = 6, //  15625 Sps
    ADC_SPS_10417 = 7, //  10417 Sps
    ADC_SPS_5208 = 8,  //  5208 Sps
    ADC_SPS_2597 = 9,  //  2597 Sps
    ADC_SPS_1007 = 10, //  1007 Sps
    ADC_SPS_503 = 11,  //  503.8 Sps
    ADC_SPS_381 = 12,  //  381 Sps
    ADC_SPS_200 = 13,  //  200.3 Sps
    ADC_SPS_100 = 14,  //  100.5 Sps
    ADC_SPS_59 = 15,   //  59.52 Sps
    ADC_SPS_49 = 16,   //  49.68 Sps
    ADC_SPS_20 = 17,   //  20.01 Sps
    ADC_SPS_16 = 18,   //  16.63 Sps
    ADC_SPS_10 = 19,   //  10 Sps
    ADC_SPS_5 = 20,    //  5 Sps
    ADC_SPS_2_5 = 21,  //  2.5 Sps
    ADC_SPS_1_25 = 22, //  1.25 Sps
} ADC_SPS;

void ADC_reset();
void ADC_config(ADC_SPS);
void ADC_update();
uint8_t ADC_SPI_READ_8();
uint16_t ADC_SPI_READ_16();
uint32_t ADC_SPI_READ_24();
uint8_t ADC_SPI_WRITE_8(uint8_t data);
uint16_t ADC_ID();
void ADC_DEBUG();
uint32_t ADC_DATA();
uint8_t ADC_CMD(uint8_t read, uint8_t addr);
#endif /* __ADC_H */