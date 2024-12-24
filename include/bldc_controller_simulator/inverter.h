/**--------------------------------------------------------------------------
 * @file     inverter.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Simulate MOSFET bridge circuit
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/hall_sensors.h"
#include "bldc_controller_simulator/custom_typedefs.h"

#include <stdbool.h>


int run_mosfet_bridge(const pwmMosfets *bridge, phaseVoltages *phase_voltages, const double supply_voltage, const bool sto_active_low);

int run_inverter_mucontroller(const hallSensors *hall, pwmMosfets *bridge, const bool sto_active_low);