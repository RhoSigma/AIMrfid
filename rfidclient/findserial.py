import serial.tools.list_ports

def getPorts():
    return serial.tools.list_ports.comports()