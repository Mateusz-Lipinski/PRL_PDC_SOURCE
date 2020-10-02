#include "usart.h"
#include <stdio.h>
#include "state.h"
#include "usart_utils.h"
#include "ADC.h"
#include "spi.h"
#include "DAC.h"
#include "tim.h"
#include "PID.h"
#include "values.h"
#include <stdlib.h>

extern state State;

//Initialise temperature stabilisation setpoint
void TEMP_PID_Init()
{
    State.TEMP = 0;
    State.TEMP_PID_PWM = 0;
    State.TEMP_PID_DIR = 1;
    State.TEMP_PID_P_Error = 0;
    State.TEMP_PID_I_Error = 0;
    State.TEMP_PID_SetPoint = 35000000; //TODO: controlable setpoint
}

void TEMP_PID()
{

    State.TEMP = Temperature(); // Read current temperature

    if (State.TEMP > 40000000) // FOR TESTING: disable peltier if it gets too hot
    {                          //
        TIM1->CCR1 = 0;        //
        return;                //
    }                          //

    State.TEMP_PID_P_Error = (State.TEMP - State.TEMP_PID_SetPoint) / 1000;   // TODO: Controlable gain
    State.TEMP_PID_I_Error = State.TEMP_PID_I_Error + State.TEMP_PID_P_Error; // accumulate integral error

    int sat = -(State.TEMP_PID_P_Error / 50 + State.TEMP_PID_I_Error / 400); // TODO: Controlable gain & integration constant

    State.TEMP_PID_DIR = sat > 0; // keep output in 16b range
    sat = abs(sat);               //
    if (sat > 100)                //
    {                             //
        sat = 100;                //
    }                             //
    else if (sat < 0)             //
    {                             //
        sat = 0;                  //
    }                             //

    State.TEMP_PID_PWM = sat;                                              // set output
    TIM1->CCR1 = State.TEMP_PID_PWM;                                       // PWM
    HAL_GPIO_WritePin(PEL_DIR_GPIO_Port, PEL_DIR_Pin, State.TEMP_PID_DIR); // H-bridge polarisation

    if (abs(State.TEMP_PID_P_Error) < 200) // check if temperature is stable
    {
        if (State.TEMP_PID_LOCK_count < 10) // if it isnt stable for long enough, increase counter
        {
            State.TEMP_PID_LOCK_count++;
            State.TEMP_PID_LOCK = 0;
        }
        else // if it is stable for long enough, set flag
        {
            if (State.TEMP_PID_LOCK == 0)
            {
                SERIAL_WRITE_GREEN("Temperature locked\n");
            }
            State.TEMP_PID_LOCK = 1;
        }
    }
    else // if error rises, reset lock flag
    {
        State.TEMP_PID_LOCK_count = 0;
        if (State.TEMP_PID_LOCK == 1)
        {
            SERIAL_WRITE_RED("Temperature unlocked\n");
        }
        State.TEMP_PID_LOCK = 0;
    }
}
