/**--------------------------------------------------------------------------
 * @file     params.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    One centralized file for all parameters, motor, simulation, etc.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

/* Geometry */
#define PI 3.14159265359

/* Runge-Kutta */
#define RK_NUMBER_STEPS 1
#define RK_STEP_SIZE 0.000001
#define RK_ODE_SYSTEM_SIZE 5

/* PID */
#define PID_FREQ 1000000
#define PID_EPSILON 0.001
#define PID_MIN 0
#define PID_MAX 255

#define PID_ZIEGLER_KU 0.013 /* Values through Ziegler-Nicholson */
#define PID_ZIEGLER_TU 0.002

#define PID_P 0.6 * PID_ZIEGLER_KU 
#define PID_I 1.2 * PID_ZIEGLER_KU / PID_ZIEGLER_TU
#define PID_D 0.075 * PID_ZIEGLER_KU * PID_ZIEGLER_TU

// #define PID_P 0.1
// #define PID_I 0.0
// #define PID_D 0.0

/* Motor */
#define MOTOR_P  4                      /* [-]              Number of poles         Maxon 323217 */
#define MOTOR_J  0.000000554            /* [Kg m²]          Moment of Inertia       Maxon 323217 */
#define MOTOR_R  0.323 / 2              /* [Ohm]            Stator Resistance       Maxon 323217 */
#define MOTOR_L  0.0000283 / 2          /* [H]              Stator Inductance       Maxon 323217 */
#define MOTOR_B  0.0001                 /* [-]              Damping Constant        Estimation   */
#define MOTOR_KV 94.980818              /* [rad / (V sec)]  Back EMF Constant       Maxon 323217 */
#define MOTOR_KT 0.0105                 /* [Nm / A]         Motor Torque Constant   Maxon 323217 */

#define MOTOR_SUPPLY_VOLTAGE 18         /* [V]  Supply Voltage of the Inverter      Maxon 323217 */

/* Simulation */
#define SIM_ACCEPTABLE_DELTA_ANGLE 0.6 * PI / 180 * 2 / MOTOR_P     /* 100 steps per electrical change */
#define SIM_TEMP_DELTA_COEFF 0.008
#define SIM_TEMP_RPM_COEFF 0.000000001

/* Logs */
#define LOG_CREATE_FILE_WITH_TIMESTAMP 0

#define LOG_STUDENT_IO 1
#define LOG_SIM_STEPS 1
#define LOG_MODE_CHANGES 1
#define LOG_P_FLOAT "%.6f"