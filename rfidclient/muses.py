#!/usr/bin/env python3

import asyncio
import serial
import pyautogui
import findserial

ports = findserial.getPorts()

serial_port = ""

for port, desc, hwid in sorted(ports):
    print("{}: {} [{}]".format(port, desc, hwid))
    if "USB to UART Bridge Controller" in desc :
        serial_port = port

if len(serial_port) < 2:
    print("No Device Found")
    exit()
else:
    print("Using {}".format(serial_port))


# Set the serial port and baud rate
ser = serial.Serial(serial_port, 9600)

async def processData(data):
    val_pair = str(data).split(";")
    if len(val_pair) > 1:
        if str(val_pair[0]).__contains__("UID"):
            uid = str(val_pair[1]).replace("\\n",'').replace(' ','').replace('\'','').upper()
            print(uid)
            for c in uid:
                pyautogui.press(c)
            print('\n\n')

# Continuously read serial data and emulate keyboard input
while True:
    data = ser.readline()
    #print(data)
    asyncio.run(processData(data))