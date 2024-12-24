/**--------------------------------------------------------------------------
 * @file     fixture_example.hpp
 * @author   Urban Willi
 * @date     Mai, 2024
 * @brief    Example of a test fixture for test setup and teardown.
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

#include <gtest/gtest.h>

extern "C"
{
    #include "bldc_controller_simulator/error.h"
    #include "bldc_controller_simulator/simulation.h"
    #include "bldc_controller_simulator/pid.h"
    #include "bldc_controller_simulator/log.h"
    #include "bldc_controller_simulator/custom_typedefs.h"
}


class FixtureIntegration : public ::testing::Test
{
protected:
    FixtureIntegration();
    ~FixtureIntegration();

    controllerIo *controller_io;
    motorQuantities motor_quant;
    pwmMosfets bridge_signals;
    hallSensors hall_sensors;
    bool sto_active_low = true;
    bool enable;
    double sim_time;
    double sim_duration;
    int scenario;
    int status;
};

/**
 * @brief Setup function, called each time before every test case is executed.
 */
FixtureIntegration::FixtureIntegration(/* args */)
{
    // Set up what you want to set up every time you use this fixture.
    log_init();

    sim_time = 0.0;
    sim_duration = 1.0;

    controller_io = new controllerIo();
}

/**
 * @brief Teardown function, called each time after every test case is executed.
 */
FixtureIntegration::~FixtureIntegration()
{
    // Clean up after yourself, memory etc.
    log_close();
}
