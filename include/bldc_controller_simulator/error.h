/**--------------------------------------------------------------------------
 * @file     error.h
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    One place for error code defines
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#define SUCCESS 0
#define GENERIC_ERROR 1

/* Define file-specific with comment as title and add number range */

/* inverter.h   Range 100 - 199 */
#define INVALID_HALL_SIGNAL 100

/* GTK  Range 500 - 599 */
#define REGEX_COMPILATION_FAILED 501

/* Window Mode 600 -699 */
#define MODE_START_SIM 601
#define MODE_STOP_SIM 602