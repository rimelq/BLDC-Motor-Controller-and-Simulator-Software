/**--------------------------------------------------------------------------
 * @file     students_code.h
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    The trapezoidal controller loop and call of simulation. Students will edit this file.
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

#include <gtk/gtk.h>

#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/simulation.h"
#include "bldc_controller_simulator/pid.h"
#include "bldc_controller_simulator/log.h"
#include "bldc_controller_simulator/custom_typedefs.h"
#include "bldc_controller_simulator/bldc_controller_simulator_window.h"

enum motor_states {
    STO_Mode, //Safe torque off 
    DEGRADED_Mode, 
    RUNNING_Mode,
};

void *run_trapezoidal_controller(void *arg);
int update_values_gui(void* data);
int set_initial_values_controller_io(BldcControllerSimulatorWindow *win);
void set_actual_rpm_label(BldcControllerSimulatorWindow *win, double raw_val);
void sim_cleanup();


void reaction_rpm_change(double time, double* target_rpm_parameter, double* actual_rpm_motor_parameter);
double calculate_hall_triggered_RPM(double sim_time, hallSensors *hall);
void monitor_and_reduce_rpm(controllerIo *controller_io, motorQuantities *motor_quant, double sim_time);
void safe_to_resume(const double* temperature, int temp_size, double time);
void check_temp_over_145degrees_every_100ms(double time, const double *temperature);
enum motor_states get_motor_state();
