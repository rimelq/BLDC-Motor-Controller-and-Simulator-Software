/**--------------------------------------------------------------------------
 * @file     motor.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Define physical constants of the motor and other motor properties.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/motor.h"


double back_emf_unit_function(const double theta_el)
{
    double ret_value;
    const double phase_shift = 0; // -30 * PI / 180 - 120 * PI / 180; /* Easier to implement because only 2 parts non-constant. */
    double theta_deg = reduce_revolutions_to_mechanical_angle_rad2deg(theta_el + phase_shift);

    /* Back EMF model */
    if (theta_deg <= 120)
    {
        ret_value = 1.0;
    }

    else if (theta_deg < 180)
    {
        ret_value = -1 * theta_deg / 30.0 + 5.0;
    }

    else if (theta_deg <= 300)
    {
        ret_value = -1.0;
    }

    else
    {
        ret_value = theta_deg / 30.0 - 11;
    }

    return ret_value;
}

double calculate_electric_torque(const double theta_el, const double i_a, const double i_b, const double i_c)
{
    double t_elec =   i_a * MOTOR_KT * back_emf_unit_function(theta_el)
                    + i_b * MOTOR_KT * back_emf_unit_function(theta_el - 2*PI/3) 
                    + i_c * MOTOR_KT * back_emf_unit_function(theta_el + 2*PI/3);

    return t_elec;
}

/* TODO: Check formulas because Nm - rad/s cant be right */
int rk_f_vec(const double y_vec[], double f_y_vec[], const double t_load, const phaseVoltages *phase_voltages)
{
    double t_elec =   calculate_electric_torque(y_vec[0] * MOTOR_P / 2, y_vec[2], y_vec[3], y_vec[4]);

    f_y_vec[0] =  y_vec[1]; /* Theta_me */
    f_y_vec[1] = (t_elec - t_load - MOTOR_B * y_vec[1]) / MOTOR_J; /* omega_me (used in all formulas) */
    f_y_vec[2] = (phase_voltages->v_a - MOTOR_R * y_vec[2] - y_vec[1] / MOTOR_KV * back_emf_unit_function(y_vec[0] * MOTOR_P / 2 )) / MOTOR_L;         /* i_a */
    f_y_vec[3] = (phase_voltages->v_b - MOTOR_R * y_vec[3] - y_vec[1] / MOTOR_KV * back_emf_unit_function(y_vec[0] * MOTOR_P / 2 - 2*PI/3)) / MOTOR_L; /* i_b */
    f_y_vec[4] = (phase_voltages->v_c - MOTOR_R * y_vec[4] - y_vec[1] / MOTOR_KV * back_emf_unit_function(y_vec[0] * MOTOR_P / 2 + 2*PI/3)) / MOTOR_L; /* i_c */

    return SUCCESS;
}