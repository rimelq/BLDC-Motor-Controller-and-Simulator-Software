# Coverage report

## Install lcov or gcovr

### Ubuntu 22.04

`sudo apt install -y lcov`

### WSL

Inside WSL command prompt, follow Ubuntu instructions.

### Windows

1) If python is not yet installed: In MSYS2 command prompt:
    ```
    pacman -S mingw-w64-ucrt-x86_64-python-pip
    pacman -S mingw-w64-ucrt-x86_64-python-lxml
    ```
2) In a command prompt:
    `pip install gcovr`

## Enable branch coverage

### Ubuntu, WSL

Copy the `.lcovrc` file found in the `lcov_config` folder to your home directory.

### Windows

No worries, turned on by default in gcovr.

## Obtain coverage report

### Ubuntu, WSL

1) Build as usual with cmake. 

    `cmake -D CMAKE_BUILD_TYPE=DEBUG -S . -B build && cmake --build build`  

2) Cd to build folder
3) `make`
4) `make bldc_controller_simulator_coverage`
5) Open the file `build/bldc_controller_simulator_coverage/index.html` with a browser.

Repeat steps 1) to 4) for an updated version of the report, then make sure to reload the file in the browser.

### Windows

1) Build as usual with cmake. 

    ```
    cmake -D CMAKE_BUILD_TYPE=DEBUG -G "Ninja" -S . -B build
    cmake --build build
    ```

2) Cd to build folder
3) `ninja .\bldc_controller_simulator_coverage`
4) Open the file `build\bldc_controller_simulator_coverage\index.html` with a browser.

Repeat steps 1) to 3) for an updated version of the report, then make sure to reload the file in the browser.


## Troubleshooting

### My tests don't show up in the report

Make sure you included all necessary files in the add_executable function of your test target (Files under test and the test file itself.)

### It's not working in MacOS or I cannot find lcov package

I have not tested on MacOS.
Try installing gcovr. Lcov and gcovr do essentially the same thing. Try either and use the corresponding setup command in the makefile. (`setup_target_for_coverage_*`)