/**--------------------------------------------------------------------------
 * @file     callbacks.h
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Handle GTK signals from the GUI (Button clicks, etc)
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

#include "bldc_controller_simulator/bldc_controller_simulator_window.h"
#include "bldc_controller_simulator/error.h"
#include "bldc_controller_simulator/students_code.h"

#ifdef _WIN32
    __declspec(dllexport) void on_start_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win);
    __declspec(dllexport) void on_stop_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win);
    __declspec(dllexport) void on_target_scale_changed(GtkScale *s, BldcControllerSimulatorWindow *win);
    __declspec(dllexport) void on_sim_duration_changed(GtkEntry *e, BldcControllerSimulatorWindow *win);
    __declspec(dllexport) void on_sim_duration_enter(GtkEntry *e, BldcControllerSimulatorWindow *win);
#else
    void on_start_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win);
    void on_stop_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win);
    void on_target_scale_changed(GtkScale *s, BldcControllerSimulatorWindow *win);
    void on_sim_duration_changed(GtkEntry *e, BldcControllerSimulatorWindow *win);
    void on_sim_duration_enter(GtkEntry *e, BldcControllerSimulatorWindow *win);
#endif

bool filter_entry_text(BldcControllerSimulatorWindow *win);