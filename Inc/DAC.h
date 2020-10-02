#ifndef __DAC_H
#define __DAC_H
#include "main.h"
#include "gpio.h"

#define DAC_CH(i) ((0 <= i && i < 8) ? i << 16 : 0) //Channel addresses
#define DAC_WRITE 0x100000                          //C3:C0: 0 0 0 1 Write to Input Register n where n = 1 to 8, depending on the DAC selected from the address bits in Table 11 (dependent on LDAC)
#define DAC_UPDATE 0x20000                          //C3:C0: 0 0 1 0 Update DAC Register n with contents of Input Register n
#define DAC_WRITE_AND_UPDATE 0x300000               //C3:C0: 0 0 1 1 Write to and update DAC Channel n
#define DAC_POWER 0x400000                          //C3:C0: 0 1 0 0 Power down/power up the DAC
#define DAC_HW_LDAC_MASK 0x500000                   //C3:C0: 0 1 0 1 Hardware LDAC mask register
#define DAC_SOFT_RESET 0x600000                     //C3:C0: 0 1 1 0 Software reset (power-on reset)
#define DAC_REF_GAIN_REG 0x700000                   //C3:C0: 0 1 1 1 Internal reference and gain setup register
#define DAC_DAISY_CHAIN_REG 0x800000                //C3:C0: 1 0 0 0 Set up the DCEN register (daisy-chain enable)
#define DAC_READBACK_REG 0x900000                   //C3:C0: 1 0 0 1 Set up the readback register (readback enable)
#define DAC_WRITE_AND_UPDATE_ALL 0xa00000           //C3:C0: 1 0 1 0 Update all channels of the input register simultaneously with the input data
#define DAC_UPDATE_ALL 0xb00000                     //C3:C0: 1 0 1 1 Update all channels of the DAC register and input register simultaneously with the input data

#define RF_PS_704_OFFSET DAC_CH(6)  // DAC channels by function
#define RF_PS_704_FINE DAC_CH(7)    //
#define RF_PS_352_OFFSET DAC_CH(4)  //
#define RF_PS_352_FINE DAC_CH(5)    //
#define RF_ATT_704_OFFSET DAC_CH(0) //
#define RF_ATT_704_FINE DAC_CH(1)   //
#define RF_ATT_352_OFFSET DAC_CH(3) //
#define RF_ATT_352_FINE DAC_CH(2)   //

void DAC_cmd(uint32_t data);
void DAC_reset();
void DAC_SPI_WRITE_24(uint32_t data);
void DAC_test();
void DAC_test_val(uint32_t);

#endif /* __DAC_H */