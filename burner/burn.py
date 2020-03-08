#!/usr/bin/env python3

import serial

with serial.Serial('/dev/cu.usbmodem14301', 115200) as ser:
    tries = 0
    c = ''
    while c != b'H':
        tries += 1
        ser.write(b's')
        ser.flush()
        c = ser.read()
        print(c)
    line = ser.readline()
    print(line)
    print("tries", tries)
