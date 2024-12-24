/**--------------------------------------------------------------------------
 * @file     utils.h
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

#pragma once

#include "bldc_controller_simulator/params.h"

#include <math.h>

double reduce_revolutions_to_mechanical_angle_rad2deg(const double theta);

double reduce_revolutions_to_mechanical_angle(const double theta);