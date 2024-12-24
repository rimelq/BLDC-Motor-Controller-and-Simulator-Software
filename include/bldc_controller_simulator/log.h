/**--------------------------------------------------------------------------
 * @file     log.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Organize logging to files (csv and maybe text)
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include "bldc_controller_simulator/simulation.h"
#include "bldc_controller_simulator/params.h"

#include <stdio.h>
#include <locale.h>


void log_init();

void log_close();

void log_sim_steps(const double numeric_approx[][RK_ODE_SYSTEM_SIZE], const phaseVoltages *phase_voltages);

void log_student_io(const pwmMosfets *bridge, const bool sto_active_low, const motorQuantities *m_quant, const hallSensors *halls, const bool enable, const double sim_time);

void log_mode_change(const double sim_time, const char* mode, const char* detailed_message);