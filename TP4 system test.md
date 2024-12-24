## System Test: Overheat Management and Mode Transitions

The following enhanced pseudocode () outlines a system test scenario for managing overheat conditions and transitioning between different modes (`RUNNING_Mode`, `DEGRADED_Mode`, `STO_Mode`) based on the specified technical safety requirements.

### Explanation

- **Initialization**: Set the target RPM to a typical acceptable `RUNNING_Mode` RPM (3000) and start the motor.
- **Initial Verification**: Ensure the system is in `RUNNING_Mode` after startup.
- **Transition to `DEGRADED_Mode`**: Inject a fault to simulate a temperature between 100 and 145 degrees Celsius of 105°C and verify the system transitions to `DEGRADED_Mode` if the temperature remains above 100°C for 100ms.
- **Transition to `STO_Mode`**: Maintain the high temperature and verify the system transitions to `STO_Mode` after 2 seconds in `DEGRADED_Mode`.
- **Return to `RUNNING_Mode` after cooling**: Simulate the temperature dropping to a temperature below 90 degrees Celsius of 85°C and verify the system returns to `RUNNING_Mode` after 200ms.
- **Immediate Transition to `STO_Mode` for excessive temperature**: Inject a fault to simulate a temperature  above 145 degrees Celsius of 150°C and verify the system transitions to `STO_Mode` if the temperature remains above 145°C for 100ms.
- **Persistence in `DEGRADED_Mode` and go to `STO_Mode`**: Wait 2 seconds to verify the system goes to `STO_Mode` after being in `DEGRADED_Mode` for 2 seconds.
- **Persistence in `STO_Mode` and return to `RUNNING_Mode`**: Wait 2 seconds to verify the system returns to `RUNNING_Mode` after being in `STO_Mode` for 2 seconds.
- **Log Recording**: Verify the logs to ensure all mode changes and reasons are documented.

This pseudocode demonstrates how the system handles transitions between different modes based on temperature conditions, ensuring compliance with the specified technical safety requirements.

### Pseudocode

```c
#include <gtest/gtest.h>


#define RUNNING_MODE_RPM 3000; //typical acceptable RUNNING_MODE RPM
#define STABILIZATION_TIME 50; //reasonable time for the simulation to stabilize
#define TEMPERATURE_ABOVE_100 105; //temperature between 100 and 145 degrees Celsius
#define TEMPERATURE_ABOVE_145 150; //temperature above 145 degrees Celsius
#define TEMPERATURE_BELOW_90 85; //temperature below 90 degrees Celsius


void test_overheat_and_mode_transitions(void) {
    // Initialization
    ASSERT_E(read_in_log_file(),"initial state");
    set_target_rpm(RUNNING_MODE_RPM);
    start_motor();
    wait(STABILIZATION_TIME);  // Initial wait to ensure system stabilizes

    // Initial Verification
    ASSERT_E(motor, RUNNING_MODE);
    
    unsigned long long sys_time = get_current_time();  // Get the current system time
    unsigned long long current_time = sys_time;
    
    //ISSUE 8
    // Fault Injection: Temperature rises above 100°C
    inject_temperature_fault(TEMPERATURE_ABOVE_100); //temperature between 100 and 145 degrees Celsius
    
    while ((current_time-sys_time) < 100000) { 
        ASSERT_E(motor, RUNNING_MODE);
    }  
    ASSERT_E(motor, DEGRADED_MODE); // Check if we entered the DEGRADED_MODE
    ASSERT_E(read_in_log_file(),"temperature above 100 degrees for more than 100ms");
    
    //ISSUE 12
    ASSERT_LE(get_target_rpm(), 4000); // Check if the target RPM is limited to 4000 RPM

    current_time = sys_time; // We get the time when entering DEGRADED_MODE
    while ((current_time-sys_time) < 200000)
    { 
        ASSERT_E(motor, RUNNING_MODE);
    }
    ASSERT_E(motor, STO_MODE);
    
    //ISSUE 10
    // Simulation: Temperature drops below 90°C
    inject_temperature_fault(TEMPERATURE_BELOW_90);  // Temperature at 85°C
    set_motor_mode ("DEGRADED_MODE");
    current_time = sys_time;
    while ((current_time-sys_time) < 200000) {
        ASSERT_E(motor, DEGRADED_MODE);
    }
    ASSERT_E(motor, RUNNING_MODE);
    ASSERT_E(read_in_log_file(),"safe temperature");

    //ISSUE 11 
    // Inject another fault: Temperature exceeds 145°C
    set_motor_mode ("RUNNING_MODE");
    inject_temperature_fault(TEMPERATURE_ABOVE_145);
    current_time = sys_time;
    while ((current_time-sys_time) < 100000) {
        ASSERT_E(motor, RUNNING_MODE);
    }
    ASSERT_E(motor, STO_MODE);
    ASSERT_E(read_in_log_file(),"sever overheating");

    //ISSUE 9
    // Verify persitance in DEGRADED_MODE
    current_time = sys_time;
    set_motor_mode ("DEGRADED_MODE");
    while ((current_time-sys_time) < 2000000) {
        ASSERT_E(motor, DEGRADED_MODE);
    }
    ASSERT_E(motor, STO_MODE);
    ASSERT_E(read_in_log_file(),"in degraded mode for too long");

    //ISSUE 18
    // Verify persistence in STO_Mode
    current_time = sys_time;
    set_motor_mode ("STO_MODE");
    while ((current_time-sys_time) < 2000000) {
        ASSERT_E(motor, STO_MODE);
    }
    ASSERT_E(motor, RUNNING_MODE);
    ASSERT_E(read_in_log_file(),"motor in STO_Mode for more than 2000ms");
    
    printf("Overheat and mode transition test completed successfully.\n");
}

int main() {
    test_overheat_and_mode_transitions();
    return 0;
}

// Implementations for the supposedely used functions

void inject_temperature_fault(int temp) {
    // Inject a temperature fault to simulate overheating
}

void wait(int milliseconds) {
    // Wait for the specified number of milliseconds
}

int get_temperature() {
    // Return a simulated temperature
}

const char* get_mode() {
    // Return the current mode of the system
}

int get_target_rpm() {
    // Return the target RPM
}

void start_motor() {
    // Start the motor
}

void set_target_rpm(int rpm) {
    // Set the target RPM
}

const char* read_in_log_file(void) {
    // reads and returns last line in log file
}

void set_motor_mode(const char* "MODE") {
    //sets the motor in the specified MODE
}