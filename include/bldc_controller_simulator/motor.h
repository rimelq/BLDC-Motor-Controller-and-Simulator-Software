/**--------------------------------------------------------------------------
 * @file     motor.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Define physical constants of the motor and other motor properties.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/inverter.h"
#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/utils.h"
#include "bldc_controller_simulator/params.h"

#include <stdbool.h>


// Runge-Kutta function declaration
int rk_f_vec(const double y_vec[], double f_y_vec[], const double t_load, const phaseVoltages *phase_voltages);

// Other motor function declarations
double back_emf_unit_function(const double theta_el);
double calculate_electric_torque(const double theta_el, const double i_a, const double i_b, const double i_c);