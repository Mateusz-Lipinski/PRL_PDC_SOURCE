#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "usart_utils.h"
#include "ADC.h"
#include "spi.h"
#include "DAC.h"
#include "tim.h"
#include <stdlib.h>
#include "values.h"

extern state State;

//Determines optimal setpoints of Phase shifters, detectors slopes
void Calib352()
{
    SERIAL_WRITE("[CALIBRATION] ...")
    DAC_cmd(RF_PS_352_OFFSET + DAC_WRITE + 0);       // Alghoritm will loop through all values and check when both detectors will be nearest to the middle of the characteristics
    DAC_cmd(RF_PS_352_FINE + DAC_WRITE + 0x7fff);    // In optimal setpoint, fine control should be in middle-scale
    State.PD3_PD4_MSE = 16200000000;                 // Initialise with maximum value for comparison
    int64_t prevPhase3 = 0;                          //
    int64_t prevPhase4 = 0;                          // for comparison
    for (unsigned PS = 0; PS < 0xffff; PS += 0x0700) // loop through all values
    {
        DAC_cmd(RF_PS_352_OFFSET + DAC_WRITE + PS);                                                              // change phase shift and
        HAL_Delay(100);                                                                                          // wait for phase shift to settle
        int64_t Phase3 = (1800000 - (((uint64_t)State.ADC_Values[ADC_PD3_Phase_CH] * 1800000) / 0xffffff)) / 10; // calculate current phase
        int64_t Phase4 = (1800000 - (((uint64_t)State.ADC_Values[ADC_PD4_Phase_CH] * 1800000) / 0xffffff)) / 10; //

        int error3 = 90 - Phase3 / 1000;                // calculate mean squared error from the middle of the characteristics for both detectors
        int error4 = 90 - Phase4 / 1000;                //
        int MSE352 = error3 * error3 + error4 * error4; //
        // SERIAL_WRITE("PS: %u\n", PS);
        // SERIAL_WRITE("Phase3: %li\n", Phase1);
        // SERIAL_WRITE("Phase4: %li\n", Phase2);
        // SERIAL_WRITE("MSE: %i\n", MSE352);
        if (abs(MSE352) < abs(State.PD3_PD4_MSE))                                         // if error is lower than before, save setpoint parameters
        {                                                                                 //
            State.PD3_PD4_Offset = PS;                                                    //
            State.PD3_PD4_MSE = MSE352;                                                   //
            State.PD3_Slope = prevPhase3 < Phase3 ? 1 : -1;                               //
            State.PD4_Slope = prevPhase4 < Phase4 ? 1 : -1;                               //
            State.PD3_PD4_SetPoint = State.PD3_Slope * Phase3 + State.PD4_Slope * Phase4; // Setpoint as PD1 + PD2 with signs
        }
        prevPhase3 = Phase3; // save for comparison
        prevPhase4 = Phase4; //
    }
    SERIAL_WRITE("\b\b\bPD3+PD4 SETPOINT: %i\n", State.PD3_PD4_SetPoint);
    SERIAL_WRITE("[CALIBRATION] PD3 SLOPE: %i\n", State.PD3_Slope);
    SERIAL_WRITE("[CALIBRATION] PD4 SLOPE: %i\n", State.PD4_Slope);
    SERIAL_WRITE("[CALIBRATION] PS352 OFFSET: %i\n", State.PD3_PD4_Offset);
    DAC_cmd(RF_PS_352_OFFSET + DAC_WRITE + State.PD3_PD4_Offset); // set initial point of stabilisation
}

void PID352_Init()
{
    State.PID_352_Output_Fine = 0x7fff;                 // set initial point of stabilisation
    State.PID_352_Output_Coarse = State.PD3_PD4_Offset; //
    State.PID_352_P_error = 0;                          //
    State.PID_352_I_error = 0;                          //
    State.PID_352_LOCK = 0;                             //
};

void PID352() // TODO: PID parameters need to be changed externally, these are experimental
{

    State.PID_352_P_error = (State.PD3_Slope * Phase_u(ADC_PD3_Phase_CH)) + (State.PD4_Slope * Phase_u(ADC_PD4_Phase_CH)) - (State.PD3_PD4_SetPoint * 1000); // * 1000 because phase is measured in uDeg there for better resolution
    State.PID_352_I_error = State.PID_352_I_error + State.PID_352_P_error;                                                                                   // accumulate Integral error

    int sat = State.PID_352_Output_Fine - State.PID_352_P_error / 1000; //TODO: controlable gain, this is experimental

    sat = abs(sat);   // restrain output value to 16bit range
    if (sat > 0xffff) //
    {                 //
        sat = 0xffff; //
    }                 //
    else if (sat < 0) //
    {                 //
        sat = 0;      //
    }                 //

    State.PID_352_Output_Fine = sat;                                 //
    DAC_cmd(RF_PS_352_FINE + DAC_WRITE + State.PID_352_Output_Fine); // set fine output

    sat = State.PID_352_Output_Coarse - State.PID_352_I_error / 100000; //TODO: controlable integration (time) constant, this is experimental

    if (sat > 0xffff) // restrain output value to 16bit range
    {                 //
        sat = 0xffff; //
    }                 //
    else if (sat < 0) //
    {                 //
        sat = 0;      //
    }                 //

    State.PID_352_Output_Coarse = sat;                                   //
    DAC_cmd(RF_PS_352_OFFSET + DAC_WRITE + State.PID_352_Output_Coarse); // set coarse output
};
