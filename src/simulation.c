/**--------------------------------------------------------------------------
 * @file     simulation.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Simulate MOSFET bridge and 3-phase BLDC
 *
 *
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/simulation.h"
#include "bldc_controller_simulator/utils.h"
#include "bldc_controller_simulator/log.h"

#include "stdbool.h"
#include "stdlib.h"

int run_simulation_step(const pwmMosfets *bridge, const bool sto_active_low, const int scenario, const double sim_time, motorQuantities *m_quant, hallSensors *halls, bool *enable)
{
    int status = SUCCESS;

    static bool first_step = true;
    static double motor_simulated[RK_NUMBER_STEPS + 1][RK_ODE_SYSTEM_SIZE];

    double delta_angle;

    static double torque_load = 0.0; /* [Nm] Maxon 323217 max cont. torque load 43.7mNm*/

    phaseVoltages phase_voltages;

    /* Set up initial conditions only, no simulation in first step */
    if (first_step)
    {
        motor_simulated[0][0] = 0 * PI / 180;
        motor_simulated[0][1] = 0.0;

        calculate_hall_signals(motor_simulated[RK_NUMBER_STEPS][0] * MOTOR_P / 2, halls);

        first_step = false;
        return status;
    }

    status = run_mosfet_bridge(bridge, &phase_voltages, MOTOR_SUPPLY_VOLTAGE, sto_active_low);

    if (status != SUCCESS)
    {
        return status;
    }

    status = rungeKutta(motor_simulated, torque_load, &phase_voltages);

    if (status != SUCCESS)
    {
        return status;
    }

    temperature_simulation(m_quant);

    if (scenario == 1)
    {
        fault1(sim_time, m_quant);
    }

    if (scenario == 2)
    {
        fault2(m_quant);
    }

    /* Write simulation output */
    calculate_hall_signals(motor_simulated[RK_NUMBER_STEPS][0] * MOTOR_P / 2, halls);
    m_quant->actual_rpm = motor_simulated[RK_NUMBER_STEPS][1] * 30 / PI;
    m_quant->torque = calculate_electric_torque(motor_simulated[RK_NUMBER_STEPS][0], motor_simulated[RK_NUMBER_STEPS][2], motor_simulated[RK_NUMBER_STEPS][3], motor_simulated[RK_NUMBER_STEPS][4]);

    if (scenario == 3)
    {
        fault3(m_quant);
    }

    if (scenario == 4)
    {
        fault4(halls, sim_time);
    }

    if (LOG_SIM_STEPS)
    {
        log_sim_steps(motor_simulated, &phase_voltages);
    }

    /* Check change of motor angle in one simulation step */
    delta_angle = fabs(motor_simulated[RK_NUMBER_STEPS][0] - motor_simulated[0][0]);

    if (delta_angle > SIM_ACCEPTABLE_DELTA_ANGLE)
    {
        printf("WARNING: Motor did more than defined SIM_ACCEPTABLE_DELTA_ANGLE in one calculation step!\n");
    }

    // Set last values as new initial conditions for the next step
    motor_simulated[0][0] = motor_simulated[RK_NUMBER_STEPS][0];
    motor_simulated[0][1] = motor_simulated[RK_NUMBER_STEPS][1];
    motor_simulated[0][2] = motor_simulated[RK_NUMBER_STEPS][2];
    motor_simulated[0][3] = motor_simulated[RK_NUMBER_STEPS][3];
    motor_simulated[0][4] = motor_simulated[RK_NUMBER_STEPS][4];

    return status;
}

void temperature_simulation(motorQuantities *m_quant)
{
    static double previous_rpm = 0;
    double delta_rpm;

    delta_rpm = m_quant->actual_rpm - previous_rpm;

    m_quant->temp += SIM_TEMP_RPM_COEFF * m_quant->actual_rpm + SIM_TEMP_DELTA_COEFF * delta_rpm;

    previous_rpm = m_quant->actual_rpm;
}