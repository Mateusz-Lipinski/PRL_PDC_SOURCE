#ifndef __values_H
#define __values_H
#include "main.h"

uint64_t Phase(uint8_t);         // Functions to calculate physical values from ADC channel
uint64_t Phase_u(uint8_t);       //
int64_t Relative_Power(uint8_t); //
int64_t RMS_Power(uint8_t);      //
int Temperature();               //
uint64_t Voltage(uint8_t);       //
uint64_t Voltage_nV(uint8_t);    //

#endif /* __values_H */
