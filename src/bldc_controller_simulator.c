/**--------------------------------------------------------------------------
 * @file     bldc_controller_simulator.c
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

#include "bldc_controller_simulator/bldc_controller_simulator.h"
#include "bldc_controller_simulator/bldc_controller_simulator_window.h"


struct _BldcControllerSimulator
{
    GtkApplication parent;
};

G_DEFINE_TYPE(BldcControllerSimulator, bldc_controller_simulator, GTK_TYPE_APPLICATION)

static void bldc_controller_simulator_init(BldcControllerSimulator *app)
{
    (void)app;
    /* Initialise stuff before window is shown ... */
}

static void bldc_controller_simulator_activate(GApplication *app)
{
    BldcControllerSimulatorWindow *win;

    win = bldc_controller_simulator_window_new(BLDC_CONTROLLER_SIMULATOR_APP(app));
    gtk_window_present(GTK_WINDOW(win));
}

static void bldc_controller_simulator_class_init(BldcControllerSimulatorClass *class)
{
    G_APPLICATION_CLASS(class)->activate = bldc_controller_simulator_activate;
}

BldcControllerSimulator * bldc_controller_simulator_new(void)
{
    return g_object_new(BLDC_CONTROLLER_SIMULATOR_TYPE, "application-id", "org.gtk.bldccontrollersimulator", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
