"""--------------------------------------------------------------------------
 * @file     calculator.py
 * @author   Urban Willi
 * @date     March, 2024
 * @brief    Verifying mathematical model and debugging
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------
 """

import numpy as np

# Motor Parameters Maxon 323217
MOTOR_P  = 4                      # [-]              Number of poles         Maxon 323217 
MOTOR_J  = 0.000000554            # [Kg m²]          Moment of Inertia       Maxon 323217 
MOTOR_R  = 0.323 / 2              # [Ohm]            Stator Resistance       Maxon 323217 
MOTOR_L  = 0.0000283 / 2          # [H]              Stator Inductance       Maxon 323217 
MOTOR_B  = 0.0                    # [-]              Damping Constant        Estimation   TODO: DO ESTIMATION 
MOTOR_KV = 94.980818              # [rad / (V sec)]  Back EMF Constant       Maxon 323217 
MOTOR_KT = 0.0105                 # [Nm / A]         Motor Torque Constant   Maxon 323217 

MOTOR_SUPPLY_VOLTAGE = 18         # [V]  Supply Voltage of the Inverter      Maxon 323217 


# Plausibility No load speed -> steady state di/dt = 0
v_a = MOTOR_SUPPLY_VOLTAGE / 2
i_no_load = 0.218 # datasheet

no_load_speed = (v_a - MOTOR_R * i_no_load) * MOTOR_KV
print('No load speed: ', no_load_speed * 30 / np.pi) # 8131 rpm (half of actual speed which is result of both active phases)

# Plausibility stall current
stall_current = (v_a / MOTOR_R)
print('Stall current: ', stall_current) # 55.7 A -> Resistance division by 2 confirmed

# Plausibility Stall torque
stall_torque = MOTOR_KT * 55.8
print('Stall torque: ', stall_torque) # 0.586 Nm = 586 mNm

# Plausibility KV -> no load speed
kv_speed_no_load = MOTOR_KV * MOTOR_SUPPLY_VOLTAGE
print('KV used to estimate no load speed: ', kv_speed_no_load / (2 * np.pi) * 60) # 16326
print('KV directly from datasheet used to estimate no load speed: ', 907 * MOTOR_SUPPLY_VOLTAGE) # 16326 rpm (correct according to datasheet)

# Transformation KV from rpm / Volt to rad / (V sec)
kv_transformed = 907 * 2 * np.pi / 60
print('Transformed KV from datasheet to rad/(Vs): ', kv_transformed)