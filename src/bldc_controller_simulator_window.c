/**--------------------------------------------------------------------------
 * @file     bldc_controller_simulator_window.c
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

#include "bldc_controller_simulator/bldc_controller_simulator_window.h"

G_DEFINE_TYPE(BldcControllerSimulatorWindow, bldc_controller_simulator_window, GTK_TYPE_APPLICATION_WINDOW)

static void bldc_controller_simulator_window_init(BldcControllerSimulatorWindow *win)
{
  gtk_widget_init_template(GTK_WIDGET(win));

  win->sim_time_regex = g_regex_new("^[0-9]*\\.[0-9]*$|^[1-9][0-9]*$", 0, 0, NULL);

  if (win->sim_time_regex == NULL)
  {
    exit(REGEX_COMPILATION_FAILED);
  }

  win->controller_io.mutex = malloc(sizeof(*(win->controller_io.mutex)));
  g_mutex_init(win->controller_io.mutex);

  win->sim_mode = MODE_START_SIM;
  g_mutex_lock(win->controller_io.mutex);
  win->controller_io.motor_quant.target_rpm = 0.0;
  win->controller_io.is_enabled = TRUE;
  g_mutex_unlock(win->controller_io.mutex);
}

static void bldc_controller_simulator_window_dispose(GObject *gobject)
{
  BldcControllerSimulatorWindow *win;

  win = BLDC_CONTROLLER_SIMULATOR_WINDOW (gobject);
  free(win->controller_io.mutex);

  // gtk_widget_dispose_template(GTK_WIDGET(gobject), GTK_TYPE_APPLICATION_WINDOW); /* TODO: Why implicit declaration? */
  G_OBJECT_CLASS(bldc_controller_simulator_window_parent_class)->dispose(gobject);
}

static void bldc_controller_simulator_window_class_init(BldcControllerSimulatorWindowClass *class)
{
  G_OBJECT_CLASS(class)->dispose = bldc_controller_simulator_window_dispose;

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/gtk/bldc_controller_simulator/bldc_controller_simulator_window.ui"); /* Not file path, is a resource path. */

  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, sim_duration_entry);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, sim_time_level_bar);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, sim_time_label);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, actual_level_bar);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, actual_value_label);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, target_scale);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, target_value_label);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, start_button);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, stop_button);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BldcControllerSimulatorWindow, scenario_dropdown);
}

BldcControllerSimulatorWindow *bldc_controller_simulator_window_new(BldcControllerSimulator *app)
{
  return g_object_new(BLDC_CONTROLLER_SIMULATOR_WINDOW_TYPE, "application", app, NULL);
}
