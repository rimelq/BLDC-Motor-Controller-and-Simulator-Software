/**--------------------------------------------------------------------------
 * @file     bldc_controller_simulator.h
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Handle GTK application
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include <gtk/gtk.h>


#define BLDC_CONTROLLER_SIMULATOR_TYPE (bldc_controller_simulator_get_type())

G_DECLARE_FINAL_TYPE(BldcControllerSimulator, bldc_controller_simulator, BLDC_CONTROLLER_SIMULATOR, APP, GtkApplication)


BldcControllerSimulator * bldc_controller_simulator_new(void);