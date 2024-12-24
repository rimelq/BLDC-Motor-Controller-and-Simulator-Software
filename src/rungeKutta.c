/**--------------------------------------------------------------------------
 * @file     rungeKutta.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    4th order Runge-Kutta algorithm for a system of ODE. Implementation for ODEs without explicit dependence on t, only y(t).
 * 
 * INTERFACE:   - A constant RK_ODE_SYSTEM_SIZE must be defined (number of ODEs in the system)
 *              - A function "int rk_f_vec(const double y_vec[], double f_y_vec[], const double t_load, const phaseVoltages *phase_voltages);"
 *                must be defined. This function implements the system of ODE in the form d y_i / dx = ... for each entry in the vector y_vec.
 *              
 *              Additionnal arguments t_load and phase_voltages added for ease of use, might need to be edited out of this module if used for
 *              other system of ODEs.
 * 
 *              The definitions asked for in this interface are in a separate file (motor.h), to keep this one generic and easy to test.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/rungeKutta.h"

/* Private Function Declarations */
int rk_iterate(double numeric_approx[][RK_ODE_SYSTEM_SIZE], const int y_row, const double t_load, const phaseVoltages *phase_voltages);


int rungeKutta(double numeric_approx[][RK_ODE_SYSTEM_SIZE], const double torque_load, const phaseVoltages *phase_voltages)
{
    int status = SUCCESS;

    for (int i=0; i<RK_NUMBER_STEPS; i++)
    {
        status = rk_iterate(numeric_approx, i, torque_load, phase_voltages);

        if (status != SUCCESS)
        {
            break;
        }
    }
    return status;
}

int rk_iterate(double numeric_approx[][RK_ODE_SYSTEM_SIZE], const int y_row, const double t_load, const phaseVoltages *phase_voltages)
{
    int status = SUCCESS;
    double f_y_vec[RK_ODE_SYSTEM_SIZE];
    double k1_vec[RK_ODE_SYSTEM_SIZE];
    double k2_vec[RK_ODE_SYSTEM_SIZE];
    double k3_vec[RK_ODE_SYSTEM_SIZE];
    double k4_vec[RK_ODE_SYSTEM_SIZE];
    double y_pluskn_vec[RK_ODE_SYSTEM_SIZE];

    /* k1_vec */
    for (int i = 0; i < RK_ODE_SYSTEM_SIZE; i++)
    {
        y_pluskn_vec[i] = numeric_approx[y_row][i]; /* For k1_vec */
    }

    status = rk_f_vec(y_pluskn_vec, f_y_vec, t_load, phase_voltages);
    if (status != SUCCESS)
    {
        return status;
    }

    for (int i = 0; i < RK_ODE_SYSTEM_SIZE; i++)
    {
        k1_vec[i] = RK_STEP_SIZE * f_y_vec[i];
        y_pluskn_vec[i] = numeric_approx[y_row][i] + 0.5 * k1_vec[i]; /* For k2_vec */
    }

    /* k2_vec */
    status = rk_f_vec(y_pluskn_vec, f_y_vec, t_load, phase_voltages);
    if (status != SUCCESS)
    {
        return status;
    }

    for (int i = 0; i < RK_ODE_SYSTEM_SIZE; i++)
    {
        k2_vec[i] = RK_STEP_SIZE * f_y_vec[i];
        y_pluskn_vec[i] = numeric_approx[y_row][i] + 0.5 * k2_vec[i]; /* For k3_vec */
    }

    /* k3_vec */
    status = rk_f_vec(y_pluskn_vec, f_y_vec, t_load, phase_voltages);
    if (status != SUCCESS)
    {
        return status;
    }

    for (int i = 0; i < RK_ODE_SYSTEM_SIZE; i++)
    {
        k3_vec[i] = RK_STEP_SIZE * f_y_vec[i];
        y_pluskn_vec[i] = numeric_approx[y_row][i] + k3_vec[i]; /* For k4_vec */
    }

    /* k4_vec */
    status = rk_f_vec(y_pluskn_vec, f_y_vec, t_load, phase_voltages);
    if (status != SUCCESS)
    {
        return status;
    }

    for (int i = 0; i < RK_ODE_SYSTEM_SIZE; i++)
    {
        k4_vec[i] = RK_STEP_SIZE * f_y_vec[i];

        numeric_approx[y_row+1][i] = numeric_approx[y_row][i] + (k1_vec[i] + 2 * k2_vec[i] + 2 * k3_vec[i] + k4_vec[i]) / 6; /* Y_nplus1_vec */
    }

    return status;
}