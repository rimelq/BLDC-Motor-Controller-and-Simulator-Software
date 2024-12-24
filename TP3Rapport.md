# TP3 Implementation

Remarque 1: Ordinateur preté pendant une partie du semestre: plusieurs commits de l'élève pedro.condesalvarelli@epfl.ch ont été fait depuis le git de farouk.himmiche@epfl.ch

Remarque 2: We are aware that some of our issue implementations are not working. However, the logic behind it is correct. The reasons for their malfunction is due to the fact that we loss a lot of time trying to solve technical problems. Thank you for your understanding. 

## Code hyphothesis: 
    - Issue 1: this general issue has not a proper code implementation. The rest of the issues have been coded taking into account this fisrt consideration.

    - Issue 2,3,4,5,6: as the requirement for the maximum measurement interval issues wasn't imposing a specific mesurement value, we decided to take the maximum allowed measurement interval.

    - Issue 16: the numerical value of motor_quant.torque seems to be already in pourcentage. We have finally decided to put in comment our initial function, so you could refer to it to see the logic we would have implement if it was not in pourcentage.

## CppCheck:
Some errors come from the code provided to us and not from our modifications. This remarks from CppCheck will not be analyzed here.
Errors with the same error message has been treated just one time.

    1. "#include <stdio.h><--- The Standard Library input/output functions shall not be used"
    It has only been used for debugging purposes with "printf".

    2. "void sto_mode_timer_update (void) <--- Function types shall be in prototype form with named parameters<--- A compatible declaration shall be visible when an object or function with external linkage is defined"
    We have not used a header file as all our modifications are on the students_code.c file.

    3. "printf("STOP\n");<--- The value returned by a function having non-void return type shall be used" 
    False Positive as the error is just a printf

    4. "<--- All if \342\200\246 else if constructs shall be terminated with an else statement" 
    Error not from our code.

    5. "static unsigned long long ref_time = 0;<--- The scope of the variable 'ref_time' can be reduced."
    Its "unsigned long long". Could have been optimize but it works and we didn't want to take any risks by defining it in another way.

    6. "if (status != SUCCESS)<--- Because of missing configuration, misra checking is incomplete. There can be false negatives! Variable 'SUCCESS' is unknown"
    Configuration missing. We didn't know how to fix this.

    7. "if (sto_mode_time >= 2000000)<--- Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category"
    We have used double and long log type in many variables and applied ints. A possible solution would be to cast.

    9. "return true;<--- A function should have a single point of exit at the end"
    In our case its negligible.

    10. "while (sim_time < sim_duration) <--- A loop counter shall not have essentially floating type<--- There should be no more than one break or goto statement used to terminate any iteration statement" 
    Error not from our code.

    11. "switch(motor)<--- Every switch statement shall have a default label"
    Added a default that has the same as the RUNNING_MODE case.

    12. "<--- win->controller_io.sto_active_low is overwritten" 
    Error not from our code.