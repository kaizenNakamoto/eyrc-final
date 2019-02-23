import serial, time
from datetime import datetime as dt
import argparse
import os
import serial.tools.list_ports

connected = [port.device for port in serial.tools.list_ports.comports()]
...
parser = argparse.ArgumentParser(description="Using PySerial to send data serially to UART of 2560")
parser.add_argument('-f','--file', default="./final.txt", help="text file to be sent")
args = parser.parse_args()

position=[]
character = []

# final_file = args.file

def serial_send(final_file):
    with open(final_file) as f1:
            text = f1.read()

    for idx, val in enumerate(text):
        position.append(idx)
        character.append(val)

    if(character[-1]!="#"):
            with open(args.file, "a+") as f1:
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
        with open(args.file) as f:
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
    f = args.file
    serial_send(f)
    pass
