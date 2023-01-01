#!/usr/bin/env python3
import asyncio
import serial
import pyautogui
import findserial
import pyperclip

serial_port = ""
found_port = False

def resolvePort():
    ports = findserial.getPorts()
    for port, desc, hwid in sorted(ports):
        
        #print("{}: {} [{}]".format(port, desc, hwid))
        
        if "USB to UART Bridge Controller" in desc :
            return port
        
        if "USB2.0-Serial" in desc:
            return port
        
        if "USB Serial" in desc:
            return port
    
    return ""

async def processData(data):
    print(data)
    val_pair = str(data).split(";")
    if len(val_pair) > 1:
        if str(val_pair[0]).__contains__("UID.HEX"):
            uid = str(val_pair[1]).replace("\\n",'').replace(' ','').replace('\'','').replace("\\r",'\r').lower()
            pyperclip.copy(uid)
            for c in uid:
                pyautogui.press(c)

# Continuously read serial data and emulate keyboard input
while True:
    if found_port:
        # Set the serial port and baud rate
        try:
            data = ser.readline()
            asyncio.run(processData(data))
        except:
            found_port = False
            serial_port = ""
    else:
        serial_port = resolvePort()
        if len(serial_port) > 2:
            print("{}".format(serial_port))
            found_port = True
            try:
                ser = serial.Serial(serial_port, 9600)
            except:
                found_port = False