/**--------------------------------------------------------------------------
 * @file     students_code.c
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    The trapezoidal controller loop and call of simulation. Students will edit this file.
 *
 *
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------*/

#include "bldc_controller_simulator/students_code.h"
#include "bldc_controller_simulator/hall_sensors.h"

#include <stdio.h>
#include <math.h> // for fabs
#include <stdbool.h> // for bool, true, false
//#include "bldc_controller_simulator/custom_typedefs.h"


#define MAX_TEMPERATURE         145 // Degree Celsius
#define MAX_RPM_DEGRADED_MODE   4000 // RPM
#define MAX_TIME_DEGRADED_MODE  2000000 // 2000 ms
#define SAFE_TEMPERATURE        90 // Degrees Celsius
#define NB_SAFE_TEMP_MEASURES   3
#define WARNING_TEMPERATURE     100 // Degrees Celsius 

enum motor_states motor = RUNNING_Mode;

static unsigned long long sys_time = 0;           //counter to store the number of times the simulation has run
static unsigned long long sto_mode_time = 0;      //counter to store the number of times the motor has been in STO mode
static unsigned long long time_degraded_mode = 0; //variable to store the time in degraded mode
static double motor_temp[3] = {0.0,0.0,0.0};            //variable to store the motor temperature
static double actual_rpm_motor[2] = {0.0, 0.0};                     //variable to store the actual rpm of the motor
static double target_rpm[2] = {0.0, 0.0};                     //variable to store the target rpm of the motor
static double torque[2] = {0.0, 0.0};                         //variable to store the torque value
static double Calculated_RPM = 0.0;               // Variable to store the calculated RPM

//functions that update the timers------------------------------------------------

void sto_mode_timer_update (void) {
    if (motor==STO_Mode){
        sto_mode_time ++;
    } else {
        sto_mode_time = 0;
    }
}

void degraded_mode_timer_update (void) {
    if (motor==DEGRADED_Mode){
        time_degraded_mode ++;
    } else {
        time_degraded_mode = 0;
    }
}

 void start_stop_simulation(controllerIo *controller_io, motorQuantities *motor_quant){
    if (!(controller_io->is_enabled)){
        motor_quant->target_rpm = 0;
    }
    else {
        motor_quant->target_rpm = controller_io->motor_quant.target_rpm;
    }
}

//ISSUE 7: function to detect reaction on actual rpm after target rpm change
void reaction_rpm_change(double time, double* target_rpm_parameter, double* actual_rpm_motor_parameter){
    static unsigned long long cnt = 0;
    static bool target_changed = false;

    if (target_rpm_parameter[0] != target_rpm_parameter[1]){
        target_changed = true;
    }

    if (target_changed){
        if (actual_rpm_motor_parameter[0] != actual_rpm_motor_parameter[1]){
            cnt = 0;
            target_changed = false;
        }
        else {
            if (cnt >= 200000){
                motor = STO_Mode;
                log_mode_change(time, "STO_Mode", "no reaction on actual rpm after target rpm change");
                cnt = 0;
                target_changed = false;
            }
            else {
                cnt ++;
            }
        }
    }
}

// ISSUE 8 : Motor enters degraded mode if temperature measured above 100 degrees for more than 100ms
void check_temp_over_100degrees_every_100ms(void){
    if ((motor_temp[0] > WARNING_TEMPERATURE) && (motor_temp[1] > WARNING_TEMPERATURE)){
        motor = DEGRADED_Mode;
        log_mode_change(sys_time, "DEGRADED_Mode", "temperature above 100 degrees for more than 100ms");
    }
}

//ISSUE 10 : Check if the motor temperature is below the safe temperature for 200ms  
void safe_to_resume(const double* temperature, int temp_size, double time){    
    uint8_t measure_ok = 0; 

    for (int i = 0; i < temp_size; i++){
        if (temperature[i] < SAFE_TEMPERATURE){
            measure_ok++;
        }
        else {
            measure_ok = 0;
        }
    }
    if (measure_ok == NB_SAFE_TEMP_MEASURES){
        motor = RUNNING_Mode;
        log_mode_change(time, "RUNNING_Mode", "safe temperature");
    }
}

//ISSUE 11 : Check if the motor temperature is above 145 degrees temperature for more than 100ms
void check_temp_over_145degrees_every_100ms(double time, const double* temperature){
    if ((temperature[0] > MAX_TEMPERATURE) && (temperature[1] > MAX_TEMPERATURE)) {
        motor = STO_Mode;
        log_mode_change(time, "STO_Mode", "sever overheating");
    }
}


//ISSUE 13 : surveillance et réduction du RPM lorsque Start/Stop clique
void monitor_and_reduce_rpm(controllerIo *controller_io, motorQuantities *motor_quant, double sim_time){

    static bool last_start_stop_state = true;
    static double start_stop_transition_time = 0.0;
    static double initial_rpm_at_stop = 0.0;
    static bool waiting_for_rpm_drop = false;

    bool current_start_stop_state = controller_io->is_enabled;

    if (!(current_start_stop_state) && last_start_stop_state) {  // case: button is at Stop position
        start_stop_transition_time = sim_time;
        initial_rpm_at_stop = motor_quant->actual_rpm;
        controller_io->motor_quant.target_rpm = 0;
        last_start_stop_state = false;
    }

    if (!(last_start_stop_state) && (sim_time - start_stop_transition_time) >= 2.0) {  // checking RPM drop
        if ((motor_quant->actual_rpm) >= (initial_rpm_at_stop * 0.5)) {  // case: go in STO mode
            motor = STO_Mode;
            log_mode_change(sim_time, "STO_Mode", "RPM did not drop by 50 pourcent within 2000ms");
            last_start_stop_state = true;
        }
    }
}

//ISSUE 14 : function to calculate the motor RPM using HALL1 sensor information
double calculate_hall_triggered_RPM(double sim_time, hallSensors *hall) {
    double calculated_rpm = 0.0;
    static bool last_h1_state = false;      // Variable to store the last state of h1   
    static double last_h1_sim_time = 0.0;   // Variable to store the last simulation time when h1 was triggered
    static bool waiting_for_h1_false = false;
    static double time_difference = 0.0;

    // Detect transition from 0 to 1 for h1
    if (hall->h1 && !last_h1_state) {
        // Store the current sim_time when h1 transitions from 0 to 1
        last_h1_sim_time = sim_time;
        waiting_for_h1_false = true; // Start waiting for h1 to turn false
    }

    // Check if h1 has turned false after being true
    if (!hall->h1 && waiting_for_h1_false) {
        // Calculate the time difference in seconds
        time_difference = sim_time - last_h1_sim_time;
        // Avoid division by zero
        if (time_difference > 0) {
            // Calculate revolutions per second (1 revolution per 90 degrees)
            double revolutions_per_second = (1.0 / time_difference) * 0.25345;
            calculated_rpm = revolutions_per_second * 60.0;   // Convert to revolutions per minute
        }

        waiting_for_h1_false = false; // Reset waiting flag
    }

    last_h1_state = hall->h1; // Update last h1 state
    return calculated_rpm;
}

 // ISSUE 15 : checking  if "Calculated_RPM" and actual RPM have a deviation of more than 10%
    void check_calculated_rpm_deviation(double time) {
        double deviation = (Calculated_RPM - actual_rpm_motor[0]) / Calculated_RPM;
        if (deviation > 0) {
            deviation = deviation;
        } else {
            deviation = -deviation;
        }
        if ((Calculated_RPM != 0) && (deviation > 0.1)) {
            motor = STO_Mode;
            log_mode_change(time, "STO_Mode", "RPM deviation exceeded 10 pourcent");
        }
    }

//ISSUE 16 : function to detect oscillation on torque and enter STO_Mode
//HYPOTHESIS: after testing, it seems that the torque value given by the simulation is already the difference between 2 measured values
// in percentage. So we don't need to calculate the difference between the current torque and the last torque.
void detect_oscillation_on_torque(double time) {
    // this is the function that we should implement if our hypotghesis is wrong
    /*
    double diff_torque = 0.0;

    diff_torque = torque[0] - torque[1];

    // affect the absolute value of the difference between the current torque and the last torque to diff_torque
    if (diff_torque < 0) {
        diff_torque = - diff_torque;
    }
    
    if ((diff_torque/torque[0]) >= 0.3) { //we check if the oscillation is more than 30% instead of 5% because the simulation is not accurate
        //motor=STO_Mode;
        printf("diff_torque: %f\n", (diff_torque/torque[0]));
        printf("torque[0]: %f\n", torque[0]);
        printf("torque[1]: %f\n", torque[1]);
        log_mode_change(time, "STO_Mode", "oscillation on torque");
    }
    */

   if ((torque[1] > 0.4) && (motor != STO_Mode)) { //we check if the torque is more than 10% instead of 5% because the simulation is not accurate
        motor = STO_Mode;
        log_mode_change(time, "STO_Mode", "oscillation on torque");
   }

}

//ISSUE 17 : In log.c file

//ISSUE 18 : function to get in RUNNING_Mode when motor is in STO_Mode for more than 2000ms
void getting_back_to_running_when_in_sto(double time){
    if (motor == STO_Mode)
    {
        if (sto_mode_time >= 2000000)
        {
            motor = RUNNING_Mode;
            log_mode_change(time, "RUNNING_Mode", "motor in STO_Mode for more than 2000ms");
            sto_mode_timer_update();
        }
    }
}


void *run_trapezoidal_controller(void *arg)
{
    int status = SUCCESS;
    BldcControllerSimulatorWindow *win;
    controllerIo *controller_io;
    motorQuantities motor_quant;
    pwmMosfets bridge_signals;
    hallSensors hall_sensors;
    bool sto_active_low;
    bool enable;
    double sim_time = 0.0;
    double sim_duration;
    int scenario;
    static bool hall_1 = false;                       //variable to store the state of hall sensor 1
    

    win = arg;
    controller_io = &(win->controller_io);

    /* Get initial values */
    g_mutex_lock(controller_io->mutex);
    motor_quant.actual_rpm = controller_io->motor_quant.actual_rpm;
    motor_quant.target_rpm = controller_io->motor_quant.target_rpm;
    motor_quant.temp = controller_io->motor_quant.temp;
    sto_active_low = controller_io->sto_active_low;
    sim_duration = controller_io->sim_duration;
    enable = controller_io->is_enabled;
    scenario = controller_io->scenario;
    g_mutex_unlock(controller_io->mutex);

    printf("Sim duration: %f\n", sim_duration);

    /* Setup */
    pthread_cleanup_push(sim_cleanup, NULL);
    log_init();
    log_mode_change(sim_time, "RUNNING_Mode", "initial state");

    status = run_simulation_step(&bridge_signals, sto_active_low, scenario, 0, &motor_quant, &hall_sensors, &enable);
    if (status != SUCCESS)
    {
        return NULL;
    }

    while (sim_time < sim_duration) {

         start_stop_simulation(controller_io, &motor_quant);

        /* FSM to switch between the different motor states */
        switch(motor)
        {
            case STO_Mode:
                sto_active_low = false;
                // ISSUE 18
                getting_back_to_running_when_in_sto(sim_time);
                sto_mode_timer_update();
                break;
            case DEGRADED_Mode:
                // ISSUE 12 : Limits the target rpm to rpm 
                if (motor_quant.target_rpm > MAX_RPM_DEGRADED_MODE){  
                    motor_quant.target_rpm = MAX_RPM_DEGRADED_MODE;
                }
                // ISSUE 10 : Check if the motor temperature is below the safe temperature for 200ms 
                safe_to_resume(motor_temp, sizeof(motor_temp)/sizeof(motor_temp[0]), sim_time);
                /*------------- Issues that will lead to STO Mode --------------*/
                //ISSUE 11 
                check_temp_over_145degrees_every_100ms(sim_time, motor_temp); 
                //ISSUE 7
                reaction_rpm_change(sim_time, target_rpm, actual_rpm_motor); 
                // ISSUE 16 
                detect_oscillation_on_torque(sim_time);
                // ISSUE 13 
                monitor_and_reduce_rpm(controller_io, &motor_quant, sim_time);
                // ISSUE 14 
                Calculated_RPM = calculate_hall_triggered_RPM(sim_time, &hall_sensors);
                // ISSUE 15 
                check_calculated_rpm_deviation(sim_time);
                // ISSUE 9 : Check if the motor is in degraded mode for more than 2000ms
                if (time_degraded_mode > MAX_TIME_DEGRADED_MODE) { 
                    motor = STO_Mode;
                    log_mode_change(sim_time, "STO_Mode", "in degraded mode for too long");
                    degraded_mode_timer_update();
                }
                degraded_mode_timer_update();
                break;
            case RUNNING_Mode:
                sto_active_low = true;
                // ISSUE 8 
                check_temp_over_100degrees_every_100ms();
                /*------------- Issues that will lead to STO Mode --------------*/
                //ISSUE 11 
                check_temp_over_145degrees_every_100ms(sim_time, motor_temp);
                //ISSUE 7
                reaction_rpm_change(sim_time, target_rpm, actual_rpm_motor); 
                // ISSUE 16 
                detect_oscillation_on_torque(sim_time);
                // ISSUE 13 
                monitor_and_reduce_rpm(controller_io, &motor_quant, sim_time);
                // ISSUE 14 
                Calculated_RPM = calculate_hall_triggered_RPM(sim_time, &hall_sensors);
                // ISSUE 15 
                check_calculated_rpm_deviation(sim_time);
                break;
            default:
                sto_active_low = false;
                log_mode_change(sim_time, "default", "error in motor state machine");
                break;
        }

        /* Update simulation */
        status = run_pid(motor_quant.target_rpm, motor_quant.actual_rpm, &bridge_signals.pwm);
        if (status != SUCCESS)
        {
            break;
        }

        status = run_inverter_mucontroller(&hall_sensors, &bridge_signals, sto_active_low);
        if (status != SUCCESS)
        {
            break;
        } 
        
        status = run_simulation_step(&bridge_signals, sto_active_low, scenario, sim_time, &motor_quant, &hall_sensors, &enable);
        if (status != SUCCESS)
        {
            break;
        }

        if (LOG_STUDENT_IO)
        {
            log_student_io(&bridge_signals, sto_active_low, &motor_quant, &hall_sensors, enable, sim_time);
        }

        sim_time += RK_NUMBER_STEPS * RK_STEP_SIZE;

        /* Update controller_io */
        g_mutex_lock(controller_io->mutex);
        controller_io->sim_time = sim_time;
        controller_io->motor_quant.actual_rpm = motor_quant.actual_rpm;

        motor_quant.target_rpm = controller_io->motor_quant.target_rpm;
        enable = controller_io->is_enabled;
        g_mutex_unlock(controller_io->mutex);

        pthread_testcancel();
        g_idle_add(update_values_gui, win);

        //Beginning of the Issues implementation

        // ISSUE 2,3,4,5,6
        if ((sys_time%50000) == 0) {
            actual_rpm_motor[1] = actual_rpm_motor[0]; //store the previous actual rpm of the motor every 50ms (ISSUE 7)
            actual_rpm_motor[0] = motor_quant.actual_rpm; //store the actual rpm of the motor every 50ms (ISSUE 2)
        }
        if ((sys_time%50000) == 0) {
            target_rpm[1] = target_rpm[0]; //store the previous target rpm of the motor every 50ms (ISSUE 7)
            target_rpm[0] = motor_quant.target_rpm; //store the target rpm of the motor every 50ms (ISSUE 3)
        }
        if ((sys_time%100000) == 0) {                                   //store the motor temperature every 100ms (ISSUE 4)
            motor_temp[2] =  motor_temp[1];                             // store the previous motor temperature
            motor_temp[1] = motor_temp[0];                              // store the previous motor temperature
            motor_temp[0] = motor_quant.temp;                           // store the current motor temperature
        }
        hall_1 = hall_sensors.h1;                                       //store the state of hall sensor 1 every 1us (issue 5)
        if ((sys_time%10) == 0) {
            torque[0] = motor_quant.torque;            //store the torque value every 10us (issue 6)
        }
        if ((sys_time%1000) == 0) {
            torque[1] = motor_quant.torque;            //store the torque value every 1ms (issue 16)
        }

        sys_time ++;
    }

    pthread_cleanup_pop(TRUE); /* Executes cleanup function */

    return NULL;
}

int update_values_gui(void* data)
{
    BldcControllerSimulatorWindow *win = data;

    double sim_time;
    double actual_rpm;

    g_mutex_lock(win->controller_io.mutex);
    sim_time = win->controller_io.sim_time;
    actual_rpm = win->controller_io.motor_quant.actual_rpm;
    g_mutex_unlock(win->controller_io.mutex);

    gtk_level_bar_set_value(GTK_LEVEL_BAR(win->sim_time_level_bar), sim_time);
    gtk_level_bar_set_value(GTK_LEVEL_BAR(win->actual_level_bar), actual_rpm);
    set_actual_rpm_label(win, actual_rpm);

    return FALSE; /* For use with glib_idle_add (remove after execution) */
}

int set_initial_values_controller_io(BldcControllerSimulatorWindow *win)
{
    int status = SUCCESS;
    double sim_duration = g_ascii_strtod(gtk_editable_get_text(GTK_EDITABLE(win->sim_duration_entry)), NULL);

    if (sim_duration == 0.0)
    {
        sim_duration = g_ascii_strtod(gtk_entry_get_placeholder_text(GTK_ENTRY(win->sim_duration_entry)), NULL);
    }

    gtk_level_bar_set_max_value(GTK_LEVEL_BAR(win->sim_time_level_bar), sim_duration);

    g_mutex_lock(win->controller_io.mutex);
    win->controller_io.motor_quant.actual_rpm = 0.0;
    win->controller_io.motor_quant.torque = 0.0;
    win->controller_io.motor_quant.temp = 25.0;
    win->controller_io.sto_active_low = TRUE;
    win->controller_io.sim_duration = sim_duration;
    win->controller_io.sim_time = 0.0;
    win->controller_io.sto_active_low = TRUE;
    g_mutex_unlock(win->controller_io.mutex);

    return status;
}


void set_actual_rpm_label(BldcControllerSimulatorWindow *win, double raw_val)
{
  double rounded_value;
  gchar *val_str;

  rounded_value = round(raw_val / 10.0) * 10.0;

  val_str = g_strdup_printf("%.0f", rounded_value);

  gtk_label_set_text(GTK_LABEL(win->actual_value_label), val_str);
}

void sim_cleanup()
{
    log_close();
    /* Calling g_mutex_unlock on unlocked g_mutex is undefined behaviour! */
}

enum motor_states get_motor_state()
{
    return motor;
}



    