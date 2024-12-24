/**--------------------------------------------------------------------------
 * @file     custom_typedefs.h
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    One central location for all custom struct type definitions.
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

#include <stdbool.h>
#include <gtk/gtk.h>

typedef struct HallSensors
{
    bool h1;
    bool h2;
    bool h3;
} hallSensors;

typedef struct PwmMosfets
{
    bool gate1;
    bool gate2;
    bool gate3;
    bool gate4;
    bool gate5;
    bool gate6;

    int pwm; /* Defined to be in range [-255, 255] */
} pwmMosfets;

typedef struct PhaseVoltages
{
    double v_a;
    double v_b;
    double v_c;
} phaseVoltages;

typedef struct MotorQuantities
{
    double target_rpm; /* [1 / min] */
    double actual_rpm; /* [1 / min] */
    double torque;     /* [Nm] */
    double temp;       /* [deg C] */
} motorQuantities;

typedef struct ControllerIo
{
    GMutex *mutex;

    motorQuantities motor_quant;
    hallSensors hall_sensors;
    pwmMosfets pwm_mosfets;
    double sim_duration;
    double sim_time;
    bool is_enabled;
    bool sto_active_low;
    int scenario;
} controllerIo;
