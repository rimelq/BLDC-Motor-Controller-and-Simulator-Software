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

extern "C"
{
    #include "bldc_controller_simulator/error.h"
    #include "bldc_controller_simulator/simulation.h"
    #include "bldc_controller_simulator/pid.h"
    #include "bldc_controller_simulator/log.h"
    #include "bldc_controller_simulator/custom_typedefs.h"
    #include "bldc_controller_simulator/students_code.h"
}


/**
 * @brief Test without test fixture
 * 
 * This is a test case without a test fixture. First parameter is the test suite name, second parameter is the test name.
*/
TEST(UnitTest, TestLogChanges)
{
    double sim_time = 0.756;
    char mode[] = "STO_Mode";
    char detailed_message[] = "in degraded mode for too long";

    log_init();
    log_mode_change(sim_time, mode, detailed_message);
    log_close();

    FILE* file = fopen("logs/mode_changes.csv", "r");

    ASSERT_NE(file, nullptr);

    char file_line[256];
    fgets(file_line, sizeof(file_line), file);

    char reference_line[256];
    snprintf(reference_line, 256, "%f: Enter %s due to %s\n", sim_time, mode, detailed_message);

    EXPECT_STREQ(file_line, reference_line);

    fclose(file);
}

TEST(UnitTest, TestSafeToResume)
{
    double safe_temp[3] = {50.0, 50.0, 50.0};

    safe_to_resume(safe_temp, sizeof(safe_temp)/sizeof(safe_temp[0]), 0.0);

    EXPECT_EQ(get_motor_state(), RUNNING_Mode);
}

TEST(UnitTest, TestSeverOverHeat)
{
    double time = 0.0;
    double no_over_heat[3] = {90.0, 145.0, 30.0};
    double over_heat[3] = {146.0 , 146.0, 145.0};

    check_temp_over_145degrees_every_100ms(time, no_over_heat);

    EXPECT_NE(get_motor_state(), STO_Mode);

    check_temp_over_145degrees_every_100ms(time, over_heat);

    EXPECT_EQ(get_motor_state(), STO_Mode);
}