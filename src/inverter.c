/**--------------------------------------------------------------------------
 * @file     inverter.c
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

#include "bldc_controller_simulator/inverter.h"

/**
 * input: 6 PWM signals represented as integers from 0-255 (duty cycle, plus choose which ones active)
 *        Supply voltage
 *        STO
 *
 * output: Phase Voltage Va,b,c for the three motor phases.
 */

int run_mosfet_bridge(const pwmMosfets *bridge, phaseVoltages *phase_voltages, const double supply_voltage, const bool sto_active_low)
{
    if (sto_active_low == false)
    {
        phase_voltages->v_a = 0;
        phase_voltages->v_b = 0;
        phase_voltages->v_c = 0;

        return SUCCESS;
    }

    double mean_output_voltage = supply_voltage / 2 * bridge->pwm / 255.0;

    /* Following Digikey two electrical cycles equal one mechanical cycle */
    phase_voltages->v_a = (bridge->gate1 - bridge->gate2) * mean_output_voltage;
    phase_voltages->v_b = (bridge->gate3 - bridge->gate4) * mean_output_voltage;
    phase_voltages->v_c = (bridge->gate5 - bridge->gate6) * mean_output_voltage;

    return SUCCESS;
}

int run_inverter_mucontroller(const hallSensors *hall, pwmMosfets *bridge, const bool sto_active_low)
{
    int status = SUCCESS;
    if (sto_active_low == false)
    {
        bridge->gate1 = false;
        bridge->gate2 = false;
        bridge->gate3 = false;
        bridge->gate4 = false;
        bridge->gate5 = false;
        bridge->gate6 = false;

        return SUCCESS;
    }

    int s1 = hall->h1 << 2;
    int s2 = hall->h2 << 1;
    int s3 = hall->h3;

    int hall_state = s1 | s2 | s3;

    /* Reverse direction of rotation*/
    if ((bridge->pwm < 0))
    {
        hall_state = (~hall_state) & 7;
    }

    bridge->gate1 = false;
    bridge->gate2 = false;
    bridge->gate3 = false;
    bridge->gate4 = false;
    bridge->gate5 = false;
    bridge->gate6 = false;

    switch (hall_state)
    {
    case 1:
        bridge->gate1 = true;
        bridge->gate4 = true;
        break;
    case 2:
        bridge->gate2 = true;
        bridge->gate5 = true;
        break;
    case 3:
        bridge->gate4 = true;
        bridge->gate5 = true;
        break;
    case 4:
        bridge->gate3 = true;
        bridge->gate6 = true;
        break;
    case 5:
        bridge->gate1 = true;
        bridge->gate6 = true;
        break;
    case 6:
        bridge->gate2 = true;
        bridge->gate3 = true;
        break;
    default:
        status = INVALID_HALL_SIGNAL;
        break;
    }

    return status;
}
