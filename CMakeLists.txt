cmake_minimum_required(VERSION 3.14...3.29)

project(bldc_controller_simulator VERSION 1.2
                                  DESCRIPTION "BLDC motor controller and simulator for course SW Architecture EPFL"
                                  LANGUAGES C CXX) # CXX for googletest

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

if (NOT APPLE)
    # Gcov
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
        if(WIN32)
            setup_target_for_coverage_gcovr_html(NAME ${PROJECT_NAME}_coverage
                                                    EXECUTABLE test_gtest
                                                    EXCLUDE "/usr/include/*" "tests/*")
        else()
            setup_target_for_coverage_lcov(NAME ${PROJECT_NAME}_coverage
                                            EXECUTABLE test_gtest
                                            EXCLUDE "/usr/include/*" "tests/*")
        endif()
    endif()
endif()

# Find GTK library
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Add cmake folder for helper modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Only if this is the main project (skip if added through add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_C_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(CTest)

    find_package(Doxygen)

    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# Add subdirectories (which contain CMake files)
add_subdirectory(gtk_ui_builder)
add_subdirectory(src)
