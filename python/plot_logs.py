"""--------------------------------------------------------------------------
 * @file     plot_logs.py
 * @author   Urban Willi
 * @date     Feb, 2024
 * @brief    Plot bldc motor simulation
 * ---------------------------------------------------------------------------
 * Robust and Safe Systems Center Fribourg
 * HES-SO - Haute école d’ingénierie et d’architecture Fribourg HEIA Fribourg
 * Passage du Cardinal 13B / Halle Bleue
 * CH-1700 Fribourg
 * Switzerland
 * ---------------------------------------------------------------------------
 """

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os as os
from pathlib import Path


if __name__ == '__main__':
# Import
    ws_path = os.path.dirname(os.path.abspath(__file__))
    input_path_relative = "../logs/"
    output_path_relative = "./plots/"

    input_file1 = 'simulation_steps.csv'
    input_file2 = 'student_input_output.csv'
    output_file1 = Path(input_file1).stem + '.png'
    output_file2 = Path(input_file2).stem + '.png'

    input_path1 = os.path.join(ws_path, input_path_relative, input_file1)
    input_path2 = os.path.join(ws_path, input_path_relative, input_file2)
    output_path1 = os.path.join(ws_path, output_path_relative, output_file1)
    output_path2 = os.path.join(ws_path, output_path_relative, output_file2)

    df1 = pd.read_csv(input_path1, delimiter=';')
    df2 = pd.read_csv(input_path2, delimiter=';')

# df1 - Simulation steps
    df1_x = np.arange(0, len(df1)) * df1.loc[0, 'step_size']
    # df1_x = np.arange(0, len(df1)) * 1e-8
    df1.insert(9, 'sim_time', df1_x)
    # print(df1.columns)

    df1.insert(10, 'Theta_deg', df1['Theta_m'] * 180 / np.pi)
    df1.insert(11, 'omega_rpm', df1['omega_m'] * 30 / np.pi)

    total_revolutions = df1['Theta_m'].iloc[-1] / (2 * np.pi)
    print('Total revolutions of the motor: ', total_revolutions)

# df2 - Student IO
    # print(df2.columns)

    df2_shift = df2.copy()
    df2_shift['hall_s1'] = df2_shift['hall_s1'] + 4
    df2_shift['hall_s2'] = df2_shift['hall_s2'] + 2

    df2_shift['bridge_s1'] = df2_shift['bridge_s1'] + 10
    df2_shift['bridge_s2'] = df2_shift['bridge_s2'] + 8
    df2_shift['bridge_s3'] = df2_shift['bridge_s3'] + 6
    df2_shift['bridge_s4'] = df2_shift['bridge_s4'] + 4
    df2_shift['bridge_s5'] = df2_shift['bridge_s5'] + 2

    print('Found invalid hall sensor values 0, 0, 0: ', ((df2['hall_s1'] == 0) & (df2['hall_s2'] == 0)& (df2['hall_s3'] == 0)).any())
    
# df1 - Plot
    fig, axs = plt.subplots(4, 1)

    ax = axs[0]
    df1.plot(x='sim_time', y='omega_rpm', ax=ax, legend=False)
    ax.set_title('Actual RPM')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Angular Velocity [1/min]')

    ax = axs[1]
    df1.plot(x='sim_time', y='v_a', ax=ax)
    df1.plot(x='sim_time', y='v_b', ax=ax)
    df1.plot(x='sim_time', y='v_c', ax=ax)
    ax.set_title('Phase Voltages')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Voltage [V]')

    ax = axs[2]
    df1.plot(x='sim_time', y='i_a', ax=ax)
    df1.plot(x='sim_time', y='i_b', ax=ax)
    df1.plot(x='sim_time', y='i_c', ax=ax)
    ax.set_title('Phase Currents')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Current [A]')

    ax = axs[3]
    df1.plot(x='sim_time', y='Theta_deg', ax=ax, legend=False)
    ax.set_title('Total Mechanical Angle')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Angle [deg]')

    fig.suptitle('BLDC Controller and Simulator - Dataframe 1 Simulation Steps')
    fig.tight_layout()
    fig.set_size_inches((11, 8.5))
    #plt.savefig(output_path1, dpi=800)
    #plt.show()

# df2 - Plot
    fig, axs = plt.subplots(3, 2)

    ax = axs[0, 0]
    df2.plot(x='sim_time', y='target_rpm', ax=ax)
    df2.plot(x='sim_time', y='actual_rpm', ax=ax)
    ax.set_title('RPM')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Angular Velocity [1/min]')

    ax = axs[1, 0]
    df2.plot(x='sim_time', y='torque', ax=ax, legend=False)
    ax.set_title('Torque')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Torque [Nm]')

    ax = axs[2, 0]
    df2.plot(x='sim_time', y='temperature', ax=ax, legend=False)
    ax.set_title('Temperature')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Temperature [deg C]')

    ax = axs[0, 1]
    df2_shift.plot(x='sim_time', y='hall_s1', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='hall_s2', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='hall_s3', ax=ax, legend=False)
    ax.set_title('Hall Sensors')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Logic Signals [-]')
    #ax.yaxis.set_visible(False)

    ax = axs[1, 1]
    df2_shift.plot(x='sim_time', y='bridge_s1', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='bridge_s2', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='bridge_s3', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='bridge_s4', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='bridge_s5', ax=ax, legend=False)
    df2_shift.plot(x='sim_time', y='bridge_s6', ax=ax, legend=False)
    ax.set_title('Logic Signals Bridge Circuit')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Logic Signals [-]')
    ax.yaxis.set_visible(False)

    ax = axs[2, 1]
    df2.plot(x='sim_time', y='bridge_pwm', ax=ax, legend=False)
    ax.set_title('Duty Cycle Bridge Circuit')
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Int [-255, 255]')

    fig.suptitle('BLDC Controller and Simulator - Dataframe 2 Student Input/Output')
    fig.tight_layout()
    fig.set_size_inches((11, 8.5))
    #plt.savefig(output_path2, dpi=800)
    plt.show()