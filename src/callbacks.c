/**--------------------------------------------------------------------------
 * @file     callbacks.c
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

#include "bldc_controller_simulator/callbacks.h"

void on_start_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win)
{
  (void)b;

  printf("Start button clicked.\n");
  const gchar *current_label = gtk_button_get_label(GTK_BUTTON(win->start_button));

  // Focus change
  if (win->sim_mode == MODE_START_SIM)
  {
    gtk_widget_grab_focus(win->stop_button);
  }
  else if (win->sim_mode == MODE_STOP_SIM)
  {
    gtk_widget_grab_focus(win->start_button);
  }

  // Mode Start: Start Simulation in separate thread
  if (win->sim_mode == MODE_START_SIM)
  {
    int ret;
    int scenario;

    win->sim_mode = MODE_STOP_SIM;

    set_initial_values_controller_io(win);

    scenario = gtk_drop_down_get_selected(GTK_DROP_DOWN(win->scenario_dropdown));
    
    g_mutex_lock(win->controller_io.mutex);
    win->controller_io.scenario = scenario;
    g_mutex_unlock(win->controller_io.mutex);

    ret = pthread_create(&(win->worker), NULL, run_trapezoidal_controller, win);

    if (ret != SUCCESS)
    {
      printf("Thread creation failed with error code: %i\n", ret);
    }
    else
    {
      printf("Worker thread created.\n");
    }

    /* Set simulation duration to non-editable */
    gtk_editable_set_editable(GTK_EDITABLE(win->sim_duration_entry), FALSE);

    gtk_button_set_label(GTK_BUTTON(win->start_button), "Stop Simulation");
  }

  // Mode Stop: Cancel thread if running
  else if (win->sim_mode == MODE_STOP_SIM)
  {
    int ret;
    bool is_enabled;

    win->sim_mode = MODE_START_SIM;

    /* TODO: handle freeze last thing printed signalled to cancel ... */

    ret = pthread_cancel(win->worker);

    if (ret != SUCCESS)
    {
      printf("Couldn't cancel thread\n");
    }
    else
    {
      printf("Signalled thread to cancel.\n");
    }

    g_mutex_lock(win->controller_io.mutex);
    is_enabled = win->controller_io.is_enabled;
    g_mutex_unlock(win->controller_io.mutex);

    if (is_enabled == FALSE)
    {
      on_stop_button_clicked(NULL, win);
    }

    /* Set simulation duration back to editable */
    gtk_editable_set_editable(GTK_EDITABLE(win->sim_duration_entry), TRUE);

    gtk_button_set_label(GTK_BUTTON(win->start_button), "Start Simulation");
    gtk_level_bar_set_value(GTK_LEVEL_BAR(win->sim_time_level_bar), 0.0);
  }
}

void on_stop_button_clicked(GtkButton *b, BldcControllerSimulatorWindow *win)
{
  (void)b;

  bool is_enabled;

  g_mutex_lock(win->controller_io.mutex);
  is_enabled = !(win->controller_io.is_enabled);
  win->controller_io.is_enabled = is_enabled;
  g_mutex_unlock(win->controller_io.mutex);

  if (is_enabled)
  {
    gtk_button_set_label(GTK_BUTTON(win->stop_button), "Stop Motor");
  }
  else
  {
    gtk_button_set_label(GTK_BUTTON(win->stop_button), "Start Motor");
  }

  printf("Stop button clicked: Motor is stopped: %i\n", !is_enabled);
}

void on_target_scale_changed(GtkScale *s, BldcControllerSimulatorWindow *win)
{
  double current_value;
  double rounded_value;
  gchar *val_str;

  current_value = gtk_range_get_value(GTK_RANGE(s));
  rounded_value = round(current_value / 10.0) * 10.0;

  val_str = g_strdup_printf("%.0f", rounded_value);

  gtk_label_set_text(GTK_LABEL(win->target_value_label), val_str);

  g_mutex_lock(win->controller_io.mutex);
  win->controller_io.motor_quant.target_rpm = rounded_value;
  g_mutex_unlock(win->controller_io.mutex);
}

void on_sim_duration_changed(GtkEntry *e, BldcControllerSimulatorWindow *win)
{
  (void)e;

  bool is_valid;

  // g_idle_add(filter_entry_text, win);

  is_valid = filter_entry_text(win);

  if (!is_valid)
  {
    int text_len;

    text_len = gtk_entry_get_text_length(GTK_ENTRY(win->sim_duration_entry));

    if (text_len > 0)
    {
      gtk_editable_delete_text(GTK_EDITABLE(win->sim_duration_entry), text_len - 1, -1);
    }
  }
}

void on_sim_duration_enter(GtkEntry *e, BldcControllerSimulatorWindow *win)
{
  (void)e;

  gtk_widget_grab_focus(win->start_button);
}

void on_scenario_dropdown_changed(GtkDropDown *d, BldcControllerSimulatorWindow *win)
{

}

bool filter_entry_text(BldcControllerSimulatorWindow *win)
{
  bool is_valid = false;

  const gchar *text = gtk_editable_get_text(GTK_EDITABLE(win->sim_duration_entry));

  is_valid = g_regex_match(win->sim_time_regex, text, 0, NULL);

  return is_valid;
}