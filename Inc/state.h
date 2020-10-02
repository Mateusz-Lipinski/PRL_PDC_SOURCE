#ifndef __STATE
#define __STATE

#define FilterLength 16
typedef struct state // Global state struct to share between files
{
    uint64_t uptime;       // Seconds from reset
    uint8_t timer_1s_flag; // Flag set every 1s

    uint8_t error;                               // TODO: error handling?
    uint8_t cmd;                                 // Command code to execute synchronically in main loop
    uint8_t cmdLoop;                             // Loop command every 1s flag
    uint8_t UART_RX_BUFF[UART_RX_BUFFER_LENGTH]; // Buffer storing UART received data

    uint8_t ADC_Channel;                         // Current ADC channel
    uint8_t ADC_Updated;                         // Every ADC value updated flag
    uint32_t ADC_Values[16];                     // Array storing ADC raw values
    uint32_t ADC_ValuesBuffer[FilterLength][16]; // FIR filter register matrix
    uint32_t ADC_ValuesFiltered[16];             // Filter output values
    uint8_t ADC_Filter_Iteration;                // Filter iteration count, used to determine if filter contains initial zeros
    uint8_t ADC_Filter_Valid;                    // filter doesnt contain initial zeros flag

    //PHASE PIDS

    int PD1_Slope;        // 1 when increasing phase shift increases detector output voltage , or -1 otherwise
    int PD2_Slope;        // 1 when increasing phase shift increases detector output voltage , or -1 otherwise
    int PD3_Slope;        // 1 when increasing phase shift increases detector output voltage , or -1 otherwise
    int PD4_Slope;        // 1 when increasing phase shift increases detector output voltage , or -1 otherwise
    int PD1_PD2_SetPoint; // 704 phase stabilisation point
    int PD1_PD2_Offset;   // 704 initial offset (coarse control value) defined for setpoint
    int64_t PD1_PD2_MSE;  // MEAN SQUARE ERROR from the middle of the characteristics
    int PD3_PD4_SetPoint; // 352 phase stabilisation point
    int PD3_PD4_Offset;   // 352 initial offset (coarse control value) defined for setpoint
    int64_t PD3_PD4_MSE;  // MEAN SQUARE ERROR from the middle of the characteristics

    int PID_352_Output_Fine;   // PID state values:
    int PID_352_Output_Coarse; //
    int PID_352_P_error;       //
    int PID_352_I_error;       //
    int PID_352_LOCK;          // 352 pid locked

    int PID_704_Output_Fine;   //
    int PID_704_Output_Coarse; //
    int PID_704_P_error;       //
    int PID_704_I_error;       //
    int PID_704_LOCK;          // 704 pid locked

    //TEMPERATURE PID:
    int TEMP;                // Current temperature [*Ce-3]
    int TEMP_PID_SetPoint;   // Temperature setpoint in [*Ce-3]
    int TEMP_PID_P_Error;    // PID state values:
    int TEMP_PID_I_Error;    // PID state values:
    int TEMP_PID_PWM;        // output pulse width
    int TEMP_PID_DIR;        // Heating/Cooling flag
    int TEMP_PID_LOCK;       // Temperature locked flag
    int TEMP_PID_LOCK_count; // Counter to determine for how long temperature is stable

    //POWER PIDS:
    // uint32_t POWER_PID_352_SetPoint;
    // uint32_t POWER_PID_352_Output;
    // int64_t POWER_PID_352_P;
    // int64_t POWER_PID_352_I;
    // int64_t POWER_PID_352_T;
    // int64_t POWER_PID_352_P_error;
    // int64_t POWER_PID_352_I_error;
    // int8_t POWER_PID_352_LOCK;
    // uint32_t POWER_PID_704_Output;
    // uint32_t POWER_PID_704_OutputFINE;
    // int64_t POWER_PID_704_P;
    // int64_t POWER_PID_704_I;
    // int64_t POWER_PID_704_T;
    // int64_t POWER_PID_704_P_error;
    // int64_t POWER_PID_704_I_error;
    // int8_t POWER_PID_704_LOCK;

} state;

#endif /* __STATE */
