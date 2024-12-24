/**--------------------------------------------------------------------------
 * @file     test_main.cpp
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Boilerplate gtest file calling other tests. Needed for gcov.
 * 
 * @note     This files contains no tests! Please create separate test suites.
 *
 *
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

#include "GlobalSetup.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    GlobalSetup* AddGlobalTestEnvironment(GlobalSetup* env);
    int ret = RUN_ALL_TESTS();
    return ret;
}