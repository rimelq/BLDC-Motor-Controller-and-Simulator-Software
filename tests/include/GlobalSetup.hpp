/**--------------------------------------------------------------------------
 * @file     GlobalSetup.hpp
 * @author   Urban Willi
 * @date     Mai, 2024
 * @brief    Global Setup function, runs once for executable.
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


class GlobalSetup : public ::testing::Environment
{
public:
    ~GlobalSetup() override {}

    void SetUp() override {}
    void TearDown() override {}
};
