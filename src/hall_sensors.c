/**--------------------------------------------------------------------------
 * @file     hall_sensors.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Taking as input the angle of the motor, calculate values of the three Hall sensors.
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/hall_sensors.h"

void calculate_hall_signals(const double theta_el, hallSensors *hall)
{
    double theta_deg = reduce_revolutions_to_mechanical_angle_rad2deg(theta_el);

    if (theta_deg < 60)
    {
        hall->h1 = false;
        hall->h2 = false;
        hall->h3 = true;
    }

    else if (theta_deg < 120)
    {
        hall->h1 = true;
        hall->h2 = false;
        hall->h3 = true;
    }

    else if (theta_deg < 180)
    {
        hall->h1 = true;
        hall->h2 = false;
        hall->h3 = false;
    }

    else if (theta_deg < 240)
    {
        hall->h1 = true;
        hall->h2 = true;
        hall->h3 = false;
    }

    else if (theta_deg < 300)
    {
        hall->h1 = false;
        hall->h2 = true;
        hall->h3 = false;
    }

    else
    {
        hall->h1 = false;
        hall->h2 = true;
        hall->h3 = true;
    }
}