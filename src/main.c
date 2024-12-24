/**--------------------------------------------------------------------------
 * @file     main.c
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Main loop, start GTK app
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include <gtk/gtk.h>

#include "bldc_controller_simulator/bldc_controller_simulator.h"

int main (int argc, char *argv[])
{
  return g_application_run (G_APPLICATION(bldc_controller_simulator_new()), argc, argv);
}