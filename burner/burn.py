#!/usr/bin/env python3

import serial
import time

with serial.Serial('/dev/cu.usbmodem14301', 115200) as ser:
    # Need to read the first byte to prime the channel
    c = ser.read()

    ser.write(b's')
    ser.flush()
    line = ser.readline()
    print(line.decode().strip())

    ser.write(b't')
    ser.flush()
    line = ser.readline()
    print(line.decode().strip())
