# BLDC Controller and Simulator

This will be the basis for the practical exercices for the Software Architecture Course at EPFL.

## Build

```cmake -D CMAKE_BUILD_TYPE=DEBUG -S . -B build && cmake --build build``` from the top project folder.

## Run

Open a terminal to the folder bldc_controller_simulator and then enter the following command:
```./build/src/bldc_controller_simulator```

## Cppcheck for MISRA C compliance

The open source version was used, therefore only **MISRA C 2012 - amendment #2** is included.

Launch the check from the top project folder with the following command:

```cppcheck --cppcheck-build-dir=build --enable=all --suppress=missingIncludeSystem --checkers-report=build/cppcheck/checkers_report.txt --inconclusive --language=c -I include src```

Note: Project must be built before check with above command, as it only checks source files which are changed in the build folder.

Note: Unmatched suppression: missingIncludeSystem shown but it has the intended effect of removing warnings of stdlibs not found.

## Testing with CUnit

test_c_unit target built with CMake. Run tests by executing the target ./build/tests/test_c_unit

https://cunit.sourceforge.net/


## Notes

CMake structure built to example: https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project


## Sources

The mathematical model is taken from the following papers:

S. Mondal, A. Mitra and M. Chattopadhyay, "Mathematical modeling and simulation of Brushless DC motor with ideal Back EMF for a precision speed control," 2015 IEEE International Conference on Electrical, Computer and Communication Technologies (ICECCT), Coimbatore, India, 2015, pp. 1-5, doi: 10.1109/ICECCT.2015.7225944. keywords: {MATLAB;Silicon;Sensors;Load modeling;Shafts;Switches;Brushless DC motor;Back EMF;Hall sensors;PI controller;electromagnetic torque},

https://core.ac.uk/download/pdf/53188902.pdf

https://koreascience.kr/article/JAKO201117148820110.pdf

https://www.matec-conferences.org/articles/matecconf/pdf/2017/53/matecconf_icmite2017_00172.pdf

https://www.ijert.org/research/mathematical-modeling-of-brushless-dc-motor-and-its-speed-control-using-pi-controller-IJERTV8IS050446.pdf

https://www.codesansar.com/numerical-methods/ordinary-differential-equation-using-runge-kutta-rk-method-using-c-programming.htm

## TP3 Implementation

Ordinateur preté pendant une pèartie du semestre: plusieurs commits de l'élève pedro.condesalvarelli@epfl.ch ont été fait depuis le git de farouk.himmiche@epfl.ch