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
        break
    if "USB2.0-Serial" in desc:
        serial_port = port 
        break       

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
        if str(val_pair[0]).__contains__("UID.HEX"):
            uid = str(val_pair[1]).replace("\\n",'').replace(' ','').replace('\'','').replace("\\r",'\r').lower()
            for c in uid:
                pyautogui.press(c)

# Continuously read serial data and emulate keyboard input
while True:
    data = ser.readline()
    asyncio.run(processData(data))