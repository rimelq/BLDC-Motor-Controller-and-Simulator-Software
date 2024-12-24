/**--------------------------------------------------------------------------
 * @file     pid.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    PID controller for bldc speed control.
 * 
 * Adapted from: https://www.academia.edu/22900396/How_To_Write_PID_control_algorithm_using_C_language_TipsWell_1_
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/pid.h"

int run_pid(const double target_value, const double actual_value, int *control_variable)
{
    const int freq_threshold = 1 / RK_STEP_SIZE / PID_FREQ;

    static int freq_counter = freq_threshold;
    static double pre_error = 0;
    static double integral = 0;

    double error;
    double derivative;
    double output;

    if (freq_counter < freq_threshold - 1)
    {
        freq_counter ++;
        return SUCCESS;
    }

    freq_counter = 0;
    error = target_value - actual_value;

    if(fabs(error) > PID_EPSILON)
    {
        integral += error / PID_FREQ;
    }

    derivative = (error - pre_error) * PID_FREQ;

    output = PID_P * error + PID_I * integral + PID_D * derivative;

    /* Saturation Filter */
    if (output > PID_MAX)
    {
        output = PID_MAX;
    }
    else if (output < PID_MIN)
    {
        output = PID_MIN;
    }
    
    pre_error = error;

    *control_variable = round(output);

    return SUCCESS;
}