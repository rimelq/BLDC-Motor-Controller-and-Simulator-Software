# Testing

## Testing with Google Test

Googletest is a well-known testing framework written in C++. It supports mocking and has many advanced features. Mocking however relies on c++ polymorphic classes, a feature not available in C. Using gmock is possible but cumbersome and creates a lot of additional testing code.

### Overview

The files found in the folder are an example of an integration test (unit tests are written the same way).

In Googletest, test cases are organized in test suites. Test suites typically contain test cases that logically belong together. Test files must be added to the cmake file.

An important concept are test fixtures. They are defined as classes and replace test suites. A test fixture contains setup and teardown methods, which are called before and after execution of *individual* tests. Fixtures are used
to reuse code for several test cases, while maintaining independance.

### Install googletest

#### Ubuntu 22.04

1) run:

    ```sudo apt install libgtest-dev```

#### WSL

Follow instructions of Ubuntu in the WSL virtualized machine.

#### Windows

1) In MSYS2:

    ```pacman -S mingw-w64-ucrt-x86_64-gtest```

2) In command prompt, compile with cmake as usual. (Make sure to close and open a new command prompt after installation of gtest is complete in MSYS2).

### Run tests

test_gtest target built with CMake. Run tests by executing the target `test_gtest` in `build/tests/` in Ubuntu or `test_gtest.exe` in `build\tests\` in Windows. Go to the tests folder with cd before launching.

## Coverage with gcov

Please refer to the [README-COVERAGE.md](README-COVERAGE.md) for instructions on generating an html coverage report.


