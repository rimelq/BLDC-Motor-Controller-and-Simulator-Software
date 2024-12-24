/**--------------------------------------------------------------------------
 * @file     utils.c
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Small utility functions, geometric and others
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/utils.h"

double reduce_revolutions_to_mechanical_angle_rad2deg(const double theta)
{
    double theta_deg = fabs(theta * 180.0 / PI);

    if (theta_deg > 360)
    {
        int completed_revolutions = theta_deg / 360.0;

        theta_deg = theta_deg - completed_revolutions * 360.0;
    }

    if (theta < 0)
    {
        theta_deg = 360.0 - theta_deg;
    }

    return theta_deg;
}

double reduce_revolutions_to_mechanical_angle(const double theta)
{
    return reduce_revolutions_to_mechanical_angle_rad2deg(theta) * PI / 180.0;
}