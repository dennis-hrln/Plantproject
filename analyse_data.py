import serial
import time
import csv

# Configure the serial port and baud rate to match your Arduino settings
x = True
while (x):
    try:
        ser = serial.Serial('COM7', 9600,timeout=1)  # Update 'COM7' to your Arduino's serial port
        x = False
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")

def read_serial_data():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(line)
            save_to_csv(line)

def save_to_csv(data):
    with open('sensor_data.csv', mode='a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(data.split(', '))

if __name__ == "__main__":
    try:
        read_serial_data()
    except KeyboardInterrupt:
        print("Exiting program")
        ser.close()