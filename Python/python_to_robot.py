#Team ID: 681
#Author List: Karthik S Nayak
#Filename: python_to_robot.py
#Theme: eYRC-2018 HOMECOMING
#Functions: serial_send(filename)
#Global Variables: position, character, connected

######################################################### MODULES REQUIRED #########################################################
import serial, time
from datetime import datetime as dt
import argparse
import os
import serial.tools.list_ports

#List all the ports connected to the system
connected = [port.device for port in serial.tools.list_ports.comports()]


position=[]
character = []

'''
This function is used to transmit data serially to the UART of Firebird by opening COM port
'''
def serial_send(final_file):
    with open(final_file) as f1:
            text = f1.read()

    for idx, val in enumerate(text):
        position.append(idx)
        character.append(val)

    if(character[-1]!="#"):
            with open(final_file, "a+") as f1:
                    f1.write(" #")

    start = dt.now().strftime('%H:%M:%S')
    print(start)

    serialPort = serial.Serial()
    serialPort.port = connected[0]
    serialPort.baudrate = 9600
    serialPort.bytesize = serial.EIGHTBITS
    serialPort.parity = serial.PARITY_NONE
    serialPort.stopbits = serial.STOPBITS_ONE
    serialPort.timeout = 1
    serialPort.write_timeout = 2

    try:
        serialPort.open()
    except Exception as err:
        print("error. Open the serial port: "+ str(err))
        exit()

    if serialPort.isOpen():
        with open(final_file) as f:
            for line in f:
                for ch in line:
                    time.sleep(0.5)
                    serialPort.write(ch.encode())
            f.seek(0)            
            print(repr(f.read()))
    serialPort.close()

    end = dt.now().strftime('%H:%M:%S')
    print(end)    

if __name__ == '__main__':
    serial_send('./final.txt')
    pass
