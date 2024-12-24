/**--------------------------------------------------------------------------
 * @file     fault_injector.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Change values to simulate different scenarios to test students safety code
 * 
 * 
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/custom_typedefs.h"

void fault1(double sim_time, motorQuantities *m_quant);
void fault2(motorQuantities *m_quant);
void fault3(motorQuantities *m_quant);
void fault4(hallSensors *halls, double sim_time);
