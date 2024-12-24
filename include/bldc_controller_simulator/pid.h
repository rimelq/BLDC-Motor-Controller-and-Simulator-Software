/**--------------------------------------------------------------------------
 * @file     pid.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    PID controller for bldc speed control.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/params.h"

#include <math.h>


int run_pid(const double target_value, const double actual_value, int *control_variable);