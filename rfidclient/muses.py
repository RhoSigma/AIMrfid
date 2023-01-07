#!/usr/bin/env python3
import asyncio
import serial
import pyautogui
import findserial
import playsound
import os
import time

from sys import platform

serial_port = ""
found_port = False

is_mac = platform == "darwin"
dir_path = os.path.dirname(os.path.realpath(__file__))

if is_mac:
    import pyperclip as clipper
else:
    import pyclip as clipper

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
            playsound.playsound(dir_path + '/beep.wav', False)
            clipper.copy(uid)
            if is_mac:
                power_key = "command"
            else:
                power_key = "ctrl"
            time.sleep(0.2)
            #pyautogui.keyDown('backspace')
            pyautogui.keyDown(power_key)
            pyautogui.press('v')
            pyautogui.keyUp(power_key)
            pyautogui.press('\n')

# Play sound twice to signify that the reader is good, and load in memory
playsound.playsound(dir_path + '/beep.wav', True)
time.sleep(0.2)
playsound.playsound(dir_path + '/beep.wav', True)

# Continuously read serial data and emulate keyboard input
while True:
    if found_port:
        # Set the serial port and baud rate
        try:
            data = ser.readline()
            asyncio.run(processData(data))
        except Exception as e:
            found_port = False
            serial_port = ""
            print(e)
    else:
        serial_port = resolvePort()
        if len(serial_port) > 2:
            print("{}".format(serial_port))
            found_port = True
            try:
                ser = serial.Serial(serial_port, 9600)
            except:
                found_port = False