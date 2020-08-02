import serial
import serial.tools.list_ports as list_ports
from binascii import *
import sys
import struct
import time


def readData():
    sensorData = []
    while True:
        header = ser.read(1)
        if header == b'\x84':
            print("Lat Lng sensor")
            sensor = ser.read(8)
            lat = struct.unpack('<f', sensor[0:4])[0]
            lng = struct.unpack('<f', sensor[4:len(sensor)])[0]
            print("lat ", lat)
            print("lag ", lng)
            
        # accelero meter sensor
        if header == b'\x81':
            print("accelerometer and Gyro sensor")
            sensor = ser.read(36)
            aX = struct.unpack('<f', sensor[0:4])[0]
            aY = struct.unpack('<f', sensor[4:8])[0]
            aZ = struct.unpack('<f', sensor[8:12])[0]

            gX = struct.unpack('<f', sensor[12:16])[0]
            gY = struct.unpack('<f', sensor[16:20])[0]
            gZ = struct.unpack('<f', sensor[20:24])[0]

            gdX = struct.unpack('<f', sensor[24:28])[0]
            gdY = struct.unpack('<f', sensor[28:32])[0]
            gdZ = struct.unpack('<f', sensor[32:len(sensor)])[0]
            
            print("acel X ", int(aX))
            print("acel Y ", int(aY))
            print("acel Z ", int(aZ))

            print("gyro X ", gX)
            print("gyro Y ", gY)
            print("gyro Z ", gZ)

            print("gyro_d X ", gdX)
            print("gyro_d Y ", gdY)
            print("gyro_d Z ", gdZ)

        if header == b'\x82':
            print("TEMP, PRESURRE AND ALTITUDE sensor")
            sensor = ser.read(12)
            temp = struct.unpack('<f', sensor[0:4])[0]
            presurre = struct.unpack('<f', sensor[4:8])[0]
            altitude = struct.unpack('<f', sensor[8:len(sensor)])[0]
            print("TEMP ", temp)
            print("PRESURRE ", presurre)
            print("ALTITUDE ", altitude)

        if header == b'\x83':
            print("RPM AND WIND SPEED sensor")
            sensor = ser.read(8)
            sensorRPM = struct.unpack('<f', sensor[0:4])[0]
            sensorWIND = struct.unpack('<f', sensor[4:len(sensor)])[0]
            print("RPM ", sensorRPM)
            print("WIND SPEED ", sensorWIND)

# list of serial port
test = ["ttyACM3","ttyACM2","ttyACM1","ttyACM0"]
ports = list(list_ports.comports())
# print(len(ports))
while True:
    for p in test:
        try:
            ser = serial.Serial('/dev/'+p, 115200)  # , timeout=1)
            if ser.isOpen():
                print("Serial connection is still open.")
                readData()
            
        except serial.serialutil.SerialException:
            print("Searching Devices...")
    time.sleep(5)

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
