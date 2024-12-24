/**--------------------------------------------------------------------------
 * @file     rungeKutta.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    4th order Runge-Kutta algorithm for a system of ODE.
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

#pragma once

#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/motor.h"
#include "bldc_controller_simulator/inverter.h"
#include "bldc_controller_simulator/params.h"


int rungeKutta(double numeric_approx[][RK_ODE_SYSTEM_SIZE], const double torque_load, const phaseVoltages *phase_voltages);
