/**--------------------------------------------------------------------------
 * @file     fault_injector.c
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

#include "bldc_controller_simulator/fault_injector.h"

/**
 * @brief Invalid temperature
 */
void fault1(double sim_time, motorQuantities *m_quant)
{
    if (sim_time > 0.3)
    {
        m_quant->temp = 3265.0;
    }
}

/**
 * @brief rising_temperature
 */
void fault2(motorQuantities *m_quant)
{
    if (m_quant->temp < 210.0)
        m_quant->temp += 0.0003;
}

/**
 * @brief actual_rpm not changing
 */
void fault3(motorQuantities *m_quant)
{
    const double stuck_at = 4200.0;
    if (m_quant->actual_rpm > stuck_at)
        m_quant->actual_rpm = stuck_at;
}

/**
 * @brief actual_rpm >15% too fast
 */
void fault4(hallSensors *halls, double sim_time)
{
    if (sim_time > 0.3)
    {
        halls->h1 = TRUE;
        halls->h2 = FALSE;
        halls->h3 = FALSE;
    }
}