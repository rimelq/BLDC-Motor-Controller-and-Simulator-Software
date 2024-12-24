set(GRESOURCE_C     bldc_controller_simulator.gresource.c)
set(GRESOURCE_XML   bldc_controller_simulator.gresource.xml)

add_custom_command(
    OUTPUT ${GRESOURCE_C}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES}
    ARGS 
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        --generate-source
        ${GRESOURCE_XML}
    VERBATIM 
    DEPENDS ${GRESOURCE_XML} bldc_controller_simulator_window.ui
)

add_custom_target(bldc_controller_simulator_resource DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    PROPERTIES GENERATED TRUE
)