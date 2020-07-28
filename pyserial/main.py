import serial
import serial.tools.list_ports as list_ports
from binascii import *
import sys
import struct


# list of serial port
# ports = list(list_ports.comports())
# print(len(ports))
# for p in ports:
#     print(p[2])

ser = serial.Serial('/dev/cu.usbmodem1424301', 115200)  # , timeout=1)

if ser.isOpen():
    print("Serial connection is still open.")

sensorData = []
while True:
    header = ser.read(1)
    if header == b'\x81':
        sensor = ser.read(8)
        sensorA = struct.unpack('<f', sensor[0:4])[0]
        sensorB = struct.unpack('<f', sensor[4:len(sensor)])[0]
        print("sendor A ", sensorA)
        print("sendor B ", sensorB)

    # header = ser.readline()
    # if header:
    #     if header.decode().split('\r\n')[0] == "129":
    #         print("start")
    #         sensorData = bytearray()
    #         # sensorData = []
    #         while True:
    #             payload = ser.readline().decode().split('\r\n')[0]
    #             if payload == "255":
    #                 print("done")
    #                 break
    #             if payload:
    #                 sensorData.append(int(payload))
    #
    #         print(sensorData)
    #         sensorA = struct.unpack('<f', sensorData[0:4])
    #         sensorB = struct.unpack('<f', sensorData[4:len(sensorData)])
    #         print("sensor A ", sensorA)
    #         print("sensor B ", sensorB)
    #
    #         intbit = (sensorData[3] << 24) | ((sensorData[2] & 0xff) << 16) | ((sensorData[1] & 0xff) << 8) | (sensorData[0] & 0xff);
    #         print(intbit)
