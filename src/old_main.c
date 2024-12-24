/**--------------------------------------------------------------------------
 * @file     main.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Main loop, template for student exercices (with simulator / GUI)
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/simulation.h"
#include "bldc_controller_simulator/pid.h"
#include "bldc_controller_simulator/log.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main()
{
    int status = EXIT_SUCCESS;
    double sim_time = 0; /* [s] */

    pwmMosfets bridge_signals;
    motorQuantities motor_quant;
    hallSensors hall_sensors;

    const bool STO_ACTIVE_LOW = true;
    bool enable = true; /* signal from GUI, handled by simulation */

    motor_quant.actual_rpm = 0.0;
    motor_quant.target_rpm = 4000.0;

    log_init();

    status = run_simulation_step(&bridge_signals, STO_ACTIVE_LOW, &motor_quant, &hall_sensors, &enable);
    if (status != SUCCESS)
    {
        return status;
    }

    while (sim_time < 1.0)
    {
        // if (sim_time < 0.8)
        // {
        //     motor_quant.target_rpm += 0.003;
        // }

        status = run_pid(motor_quant.target_rpm, motor_quant.actual_rpm, &bridge_signals.pwm);
        if (status != SUCCESS)
        {
            return status;
        }

        // bridge_signals.pwm = 255; /* Open loop response. TODO: Remove this line */

        status = run_inverter_mucontroller(&hall_sensors, &bridge_signals, STO_ACTIVE_LOW);
        if (status != SUCCESS)
        {
            return status;
        }

        status = run_simulation_step(&bridge_signals, STO_ACTIVE_LOW, &motor_quant, &hall_sensors, &enable);
        if (status != SUCCESS)
        {
            return status;
        }

        if (LOG_STUDENT_IO)
        {
            log_student_io(&bridge_signals, STO_ACTIVE_LOW, &motor_quant, &hall_sensors, enable, sim_time);
        }

        sim_time += RK_NUMBER_STEPS * RK_STEP_SIZE;
    }

    printf("\nElapsed time: %f\n", sim_time);

    log_close();

    return status;
}
