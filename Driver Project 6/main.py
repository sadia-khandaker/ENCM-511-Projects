import numpy as np
import pandas as pd
import csv as csv
import matplotlib.pyplot as plt
import serial
import time
import datetime
import plotly.express as px
import plotly.io as pio

# OPEN SERIAL PORT
ser = serial.Serial('COM4', baudrate=9600, bytesize=8, timeout=2,
                    stopbits=serial.STOPBITS_ONE)  # open serial port for communication with pic24f16ka101
ser.flush()  # flush the serial port

# INITIALIZE VARIABLES for data collection and storage
# Create empty lists to store data
adc_list = []  # list to store adc values
voltage_list = []  # list to store voltage values
time_list = []  # list to store time values

# READ DATA FROM SERIAL PORT
def get_data():
    rcv_bytes = ''  # initialize variable to store received bytes

    start_time = time.time()  # Get start time
    while time.time() - start_time < 10:
        ser_bytes = ser.readline()  # read the serial port
        rcv_bytes = rcv_bytes + ser_bytes.decode('ascii')  # Decode serial port
        time_elapsed = time.time() - start_time  # Get time elapsed
        time_list.append(time_elapsed)  # Append time elapsed to time list
        adc_list.append(rcv_bytes)  # Append adc value to adc list
        voltage_list.append(float(rcv_bytes) * 3.3 / 1023)  # Append voltage value to voltage list
        rcv_bytes = ''  # Reset rcv_bytes variable
        time.sleep(0.1)  # Wait 0.1 seconds before reading serial port again

    return adc_list, voltage_list  # Return adc and voltage lists

get_data()  # Call get_data function

# CREATE DATAFRAME
df = pd.DataFrame({'ADC Buffer': adc_list, 'Voltage': voltage_list})
fig = px.line(df, x=df.index, y='ADC Buffer', title='ADC Buffer vs. Time')  # Create line plot of ADC Buffer vs. Time
fig.show()  # Show line plot

# Plot Voltage vs. Time
fig = px.line(df, x=df.index, y='Voltage', title='Voltage vs. Time')
fig.show()

# Save data to csv file or excel file
df.to_csv('data.csv', index=False)  # Save data to csv file
df.to_excel('data.xlsx', index=False)  # Save data to excel file

# Close serial port
ser.close()


