find_package(Doxygen REQUIRED dot)

set(DOXYGEN_HTML_OUTPUT            ${PROJECT_BINARY_DIR}/docs/html)
set(DOXYGEN_GENERATE_HTML          YES)
set(DOXYGEN_GENERATE_LATEX NO)

set(DOXYGEN_HAVE_DOT               YES)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")

doxygen_add_docs(docs ${PROJECT_SOURCE_DIR} include/bldc_controller_simulator src)
