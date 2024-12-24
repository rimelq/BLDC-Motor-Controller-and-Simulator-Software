if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
elseif (APPLE)
    set(COMPILE_FOR_OSX TRUE)
elseif (UNIX)
    set(COMPILE_FOR_LINUX TRUE)
endif ()

add_executable(bldc_controller_simulator main.c
                                         hall_sensors.c
                                         inverter.c
                                         motor.c
                                         pid.c
                                         rungeKutta.c
                                         simulation.c
                                         utils.c
                                         log.c
                                         bldc_controller_simulator.c
                                         bldc_controller_simulator_window.c
                                         callbacks.c
                                         students_code.c
                                         fault_injector.c
                                         ${CMAKE_BINARY_DIR}/gtk_ui_builder/bldc_controller_simulator.gresource.c)

target_include_directories(bldc_controller_simulator PUBLIC ../include)
target_link_libraries(bldc_controller_simulator PRIVATE m PkgConfig::GTK4)
add_dependencies(bldc_controller_simulator bldc_controller_simulator_resource)

if (COMPILE_FOR_LINUX)
    target_link_options(bldc_controller_simulator PRIVATE -export-dynamic)
endif ()

if (COMPILE_FOR_OSX)
    # Macos: No target_link_options needed
endif ()

# Warning levels
if (MSVC)
    target_compile_options(bldc_controller_simulator PRIVATE /W4 /WX)
else ()
    target_compile_options(bldc_controller_simulator PRIVATE -Wall -Wextra -std=c11 -Wpedantic)
endif ()