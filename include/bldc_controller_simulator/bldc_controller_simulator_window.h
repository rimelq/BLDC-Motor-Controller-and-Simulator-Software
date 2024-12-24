/**--------------------------------------------------------------------------
 * @file     bldc_controller_simulator_window.h
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Handle GTK window
 * 
 * 
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#pragma once

#include <gtk/gtk.h>
#include <pthread.h>

#include "bldc_controller_simulator/bldc_controller_simulator.h"
#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/custom_typedefs.h"

#define BLDC_CONTROLLER_SIMULATOR_WINDOW_TYPE (bldc_controller_simulator_window_get_type())

G_DECLARE_FINAL_TYPE(BldcControllerSimulatorWindow, bldc_controller_simulator_window, BLDC_CONTROLLER_SIMULATOR, WINDOW, GtkApplicationWindow)

struct _BldcControllerSimulatorWindow
{
    GtkApplicationWindow parent;

    GtkWidget *sim_time_level_bar;
    GtkWidget *sim_time_label;

    GtkWidget *actual_level_bar;
    GtkWidget *actual_value_label;

    GtkWidget *target_scale;
    GtkWidget *target_value_label;

    GtkWidget *sim_duration_entry;

    GtkWidget *start_button;
    GtkWidget *stop_button;

    GtkWidget *scenario_dropdown;

    GRegex *sim_time_regex;

    pthread_t  worker;

    int sim_mode;

    controllerIo controller_io; /* Obligatory locking of mutex inside! */
};

BldcControllerSimulatorWindow *bldc_controller_simulator_window_new(BldcControllerSimulator *app);
void bldc_controller_simulator_window_open(BldcControllerSimulatorWindow *win, GFile *file);
