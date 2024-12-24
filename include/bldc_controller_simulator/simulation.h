/**--------------------------------------------------------------------------
 * @file     simulation.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Simulate MOSFET bridge and 3-phase BLDC
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/rungeKutta.h"
#include "bldc_controller_simulator/inverter.h"
#include "bldc_controller_simulator/hall_sensors.h"
#include "bldc_controller_simulator/motor.h"
#include "bldc_controller_simulator/params.h"
#include "bldc_controller_simulator/custom_typedefs.h"
#include "bldc_controller_simulator/fault_injector.h"

#include <stdio.h>
#include <math.h>


int run_simulation_step(const pwmMosfets *bridge, const bool sto_active_low, const int scenario, const double sim_time, motorQuantities *m_quant, hallSensors *halls, bool *enable);
void temperature_simulation(motorQuantities *m_quant);