find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14) # C++14 is minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)

# we need to turn off optimization for non-skewed coverage reports
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

if (WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

add_executable(test_gtest   src/test_main.cpp
                            src/test_unit.cpp
                            src/test_integration.cpp
                            ${PROJECT_SOURCE_DIR}/src/rungeKutta.c
                            ${PROJECT_SOURCE_DIR}/src/simulation.c
                            ${PROJECT_SOURCE_DIR}/src/pid.c
                            ${PROJECT_SOURCE_DIR}/src/log.c
                            ${PROJECT_SOURCE_DIR}/src/fault_injector.c
                            ${PROJECT_SOURCE_DIR}/src/motor.c
                            ${PROJECT_SOURCE_DIR}/src/hall_sensors.c
                            ${PROJECT_SOURCE_DIR}/src/inverter.c
                            ${PROJECT_SOURCE_DIR}/src/utils.c
                            ${PROJECT_SOURCE_DIR}/src/students_code.c
                            )

target_include_directories(test_gtest   PUBLIC  ${PROJECT_SOURCE_DIR}/tests/include
                                                ${PROJECT_SOURCE_DIR}/include)

#set_target_properties(test_gtest PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(test_gtest    GTest::gtest_main
                                    PkgConfig::GTK4)

set_target_properties(test_gtest PROPERTIES COMPILE_FLAGS "--coverage")
set_target_properties(test_gtest PROPERTIES LINK_FLAGS "--coverage")

if(NOT APPLE)
    target_compile_options(test_gtest PRIVATE -fprofile-arcs -ftest-coverage) # gcov compile flags
    target_link_libraries(test_gtest    -lgcov -coverage) # gcov link flags
endif ()

include(GoogleTest)
# gtest_discover_tests(test_gtest)