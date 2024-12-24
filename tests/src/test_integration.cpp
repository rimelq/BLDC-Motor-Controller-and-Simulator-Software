/**--------------------------------------------------------------------------
 * @file     test_integration_simulationStructs.cpp
 * @author   Urban Willi
 * @date     Mai, 2024
 * @brief    Example Integration test for simulation part of application.
 * 
 * @note     Example Integration test checking if struct values have changed with iteration of run simulation test / 1 microcontroller step.
 *
 *
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include <gtest/gtest.h>
#include "FixtureIntegration.hpp"

extern "C"
{
    #include "bldc_controller_simulator/error.h"
    #include "bldc_controller_simulator/simulation.h"
    #include "bldc_controller_simulator/pid.h"
    #include "bldc_controller_simulator/log.h"
    #include "bldc_controller_simulator/custom_typedefs.h"
    #include "bldc_controller_simulator/students_code.h"
}

// Example Integration test checking if struct values have changed with iteration of run simulation test / 1 microcontroller step.


/**
 * @brief Test without test fixture
 * 
 * This is a test case without a test fixture. First parameter is the test suite name, second parameter is the test name.
*/
TEST(TestIntegration, TestLogChangesForTemperature)
{
    double over_heat[3] = {200.0, 200.0, 300.0};
    double time = 0.0;
    char mode[] = "STO_Mode";
    char detailed_message[] = "sever overheating";

    log_init();

    check_temp_over_145degrees_every_100ms(time, over_heat);

    log_close();

    FILE* file = fopen("logs/mode_changes.csv", "r");

    ASSERT_NE(file, nullptr);

    char file_line[256];
    fgets(file_line, sizeof(file_line), file);

    char reference_line[256];
    snprintf(reference_line, 256, "%f: Enter %s due to %s\n", time, mode, detailed_message);

    EXPECT_STREQ(file_line, reference_line);

    fclose(file);
}


/**
 * @brief Test without test fixture
 * 
 * This is a test case without a test fixture. First parameter is the test suite name, second parameter is the test name.
*/
TEST(TestIntegration, TestLogChanges_RPM)
{
    double target_rpm[2] = {1500, 2000};
    double actual_rpm_motor[2] = {0.0, 0.0};
    double time = 0.2; 
    char mode[] = "STO_Mode";
    char detailed_message[] = "no reaction on actual rpm after target rpm change";

    log_init();
    
    for (int i = 0; i < 200005; i++)
    {
        reaction_rpm_change(time, target_rpm, actual_rpm_motor);
    }

    log_close();

    FILE* file = fopen("logs/mode_changes.csv", "r");

    ASSERT_NE(file, nullptr);

    char file_line[256];
    fgets(file_line, sizeof(file_line), file);

    char reference_line[256];
    snprintf(reference_line, 256, "%f: Enter %s due to %s\n", time, mode, detailed_message);

    //EXPECT_EQ(1, 1);
    EXPECT_STREQ(file_line, reference_line);

    fclose(file);
}



/**
 * @brief Test with test fixture
 * 
 * @note Macro name is TEST_F with an F for fixture.
 * 
 * This is a test case with a test fixture. First parameter is the test *fixture name*, second parameter is the test name.
*/
/*
TEST_F(FixtureIntegration, TestStructChanges) // Tests in different test suites (or using different fixtures) can have the same name.
{
    // Run initial step of the simulation
    status = run_simulation_step(&bridge_signals, sto_active_low, scenario, 0, &motor_quant, &hall_sensors, &enable);
    ASSERT_EQ(status, SUCCESS);

    // Run two steps of the simulation
    for (int i = 0; i < 2; i++) {
        if (sim_time == 0.000001) {
            motor_quant.temp = 32000;
        }
        else {
            motor_quant.temp = 50;
        }
        
        status = run_simulation_step(&bridge_signals, sto_active_low, scenario, sim_time, &motor_quant, &hall_sensors, &enable);
        if (status != SUCCESS)
        {
            break;
        }

        sim_time += RK_NUMBER_STEPS * RK_STEP_SIZE;
        printf("sim_time: %f\n", sim_time);
        printf("motor_temp: %f\n", motor_temp[0]);
    }

    EXPECT_EQ(get_motor_state(), STO_Mode);
    // ...
}
*/