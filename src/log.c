/**--------------------------------------------------------------------------
 * @file     log.c
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

#include "bldc_controller_simulator/log.h"

static FILE *fp_sim_steps;
static FILE *fp_student_io;
static FILE *fp_mode_changes;

void log_init()
{
    system("mkdir -p logs");

    setlocale(LC_NUMERIC, "en_US");
    
    if (LOG_SIM_STEPS)
    {
        if (LOG_CREATE_FILE_WITH_TIMESTAMP)
        {
            /* TODO: Create timestamp in file name */
        }
        else
        {
            fp_sim_steps = fopen("logs/simulation_steps.csv", "w");
        }

        fputs("Theta_m;omega_m;i_a;i_b;i_c;v_a;v_b;v_c;step_size\n", fp_sim_steps);
    }
    if (LOG_STUDENT_IO)
    {
        if (LOG_CREATE_FILE_WITH_TIMESTAMP)
        {
            /* TODO: Create timestamp in file name */
        }
        else
        {
            fp_student_io = fopen("logs/student_input_output.csv", "w");
        }
        fputs("sim_time;target_rpm;actual_rpm;torque;temperature;hall_s1;hall_s2;hall_s3;bridge_s1;bridge_s2;bridge_s3;bridge_s4;bridge_s5;bridge_s6;bridge_pwm;enable;sto_active_low\n", fp_student_io);
    }
    if (LOG_MODE_CHANGES)
    {
        if (LOG_CREATE_FILE_WITH_TIMESTAMP)
        {
            /* TODO: Create timestamp in file name */
        }
        else
        {
            fp_mode_changes = fopen("logs/mode_changes.csv", "w");
            if (fp_mode_changes == NULL) {
                perror("Failed to open mode_changes.csv");
                return;
            }
        }
    }
}

void log_close()
{
    if (LOG_SIM_STEPS)
    {
        fclose(fp_sim_steps);
    }
    if (LOG_STUDENT_IO)
    {
        fclose(fp_student_io);
    }
    if (LOG_MODE_CHANGES)
    {
        fclose(fp_mode_changes);
    }
}

void log_sim_steps(const double numeric_approx[][RK_ODE_SYSTEM_SIZE], const phaseVoltages *phase_voltages)
{
    static int row = 0;

    for (; row < RK_NUMBER_STEPS + 1; row++)
    {
        for (int col = 0; col < RK_ODE_SYSTEM_SIZE; col++)
        {
            fprintf(fp_sim_steps, LOG_P_FLOAT ";", numeric_approx[row][col]);
        }

        fprintf(fp_sim_steps, LOG_P_FLOAT ";" LOG_P_FLOAT ";" LOG_P_FLOAT ";" LOG_P_FLOAT "\n", phase_voltages->v_a, phase_voltages->v_b, phase_voltages->v_c, RK_STEP_SIZE);
    }

    row = 1;
}

void log_student_io(const pwmMosfets *bridge, const bool sto_active_low, const motorQuantities *m_quant, const hallSensors *halls, const bool enable, const double sim_time)
{
    fprintf(fp_student_io, LOG_P_FLOAT ";" LOG_P_FLOAT ";" LOG_P_FLOAT ";" LOG_P_FLOAT ";" LOG_P_FLOAT ";%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i\n", sim_time, m_quant->target_rpm, m_quant->actual_rpm, m_quant->torque, m_quant->temp, halls->h1, halls->h2, halls->h3, bridge->gate1, bridge->gate2, bridge->gate3, bridge->gate4, bridge->gate5, bridge->gate6, bridge->pwm, enable, sto_active_low);
}

void log_mode_change(const double sim_time, const char* mode, const char* detailed_message)
{
    fprintf(fp_mode_changes, "%f: Enter %s due to %s\n", sim_time, mode, detailed_message);
}