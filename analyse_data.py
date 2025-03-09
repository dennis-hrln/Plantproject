import serial
import datetime
from time import sleep
import matplotlib.pyplot as plt
import csv
import os
#to activate venv type in cmd venv\Scripts\activate


def remove_old_file(filename):
    if os.path.exists(filename):
        os.remove(filename)


# Configure the serial port and baud rate to match your Arduino settings
x = True
while (x):
    try:
        ser = serial.Serial('COM7', 9600,timeout=1)  # Update 'COM7' to your Arduino's serial port
        x = False
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")

def read_serial_data(ser):
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(line)
            save_to_csv(line)

def save_to_csv(data):
    with open('sensor_data.csv', mode='a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(data.split(', '))

def read_csv_data(filename):
    runtimes = []
    humidities = []
    with open(filename, mode='r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip the header row
        for row in reader:
            if len(row) >= 3:  # Ensure there are enough columns
                runtime = row[0]
                cleaned_string = row[3].replace(',', '')
                humidity = cleaned_string
                runtimes.append(runtime)
                humidities.append(humidity)
    return runtimes, humidities

def plot_data(runtime, humidities):
    plt.figure(figsize=(10, 5))
    plt.plot(runtime, humidities, label='Optimal Humidity')
    plt.xlabel('runtime in seconds')
    plt.ylabel('Humidity')
    plt.title('Humidity Over runtime')
    plt.legend()
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    try:
        sleep(5)
        print("press control + c to stop the programm")
        remove_old_file('sensor_data.csv')
        read_serial_data(ser)
    except KeyboardInterrupt:
        filename = 'sensor_data.csv'
        times, humidities = read_csv_data(filename)
        plot_data(times, humidities)
        print("Exiting program")
        ser.close()
