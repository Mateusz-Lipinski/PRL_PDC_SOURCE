#ifndef __DAC_H
#define __DAC_H
#include "main.h"
#include "gpio.h"

#define DAC_CH(i) ((0 <= i <= 7) ? i << 16 : NULL)
#define DAC_WRITE 0x100000                //C3:C0: 0 0 0 1 Write to Input Register n where n = 1 to 8, depending on the DAC selected from the address bits in Table 11 (dependent on LDAC)
#define DAC_UPDATE 0x20000                //C3:C0: 0 0 1 0 Update DAC Register n with contents of Input Register n
#define DAC_WRITE_AND_UPDATE 0x300000     //C3:C0: 0 0 1 1 Write to and update DAC Channel n
#define DAC_POWER 0x400000                //C3:C0: 0 1 0 0 Power down/power up the DAC
#define DAC_HW_LDAC_MASK 0x500000         //C3:C0: 0 1 0 1 Hardware LDAC mask register
#define DAC_SOFT_RESET 0x600000           //C3:C0: 0 1 1 0 Software reset (power-on reset)
#define DAC_REF_GAIN_REG 0x700000         //C3:C0: 0 1 1 1 Internal reference and gain setup register
#define DAC_DAISY_CHAIN_REG 0x800000      //C3:C0: 1 0 0 0 Set up the DCEN register (daisy-chain enable)
#define DAC_READBACK_REG 0x900000         //C3:C0: 1 0 0 1 Set up the readback register (readback enable)
#define DAC_WRITE_AND_UPDATE_ALL 0xa00000 //C3:C0: 1 0 1 0 Update all channels of the input register simultaneously with the input data
#define DAC_UPDATE_ALL 0xb00000           //C3:C0: 1 0 1 1 Update all channels of the DAC register and input register simultaneously with the input data

void DAC_SPI_WRITE_24(uint32_t data);
void DAC_TEST();
#endif /* __DAC_H */