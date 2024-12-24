/**--------------------------------------------------------------------------
 * @file     hall_sensors.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Taking as input the angle of the motor, calculate values of the three Hall sensors.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/utils.h"
#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/custom_typedefs.h"

#include <stdbool.h>



void calculate_hall_signals(const double theta_el, hallSensors *hall);