#!/usr/bin/env python3

import serial
import time

def write_word(ser, word):
    ser.write([word & 0xff, word >> 8])

with serial.Serial('/dev/cu.usbmodem14301', 115200) as ser:
    # Need to read the first byte to prime the channel
    c = ser.read()

    ser.write(b's')
    line = ser.readline()
    print(line.decode().strip())

    data = []
    for i in range(256):
        data.append(i)

    length = len(data)
    offset = 0

    checksum = 0
    for i in range(256):
        checksum += checksum + data[i]
        checksum = checksum & 0xffff

    ser.write(b'w')
    write_word(ser, length)
    write_word(ser, offset)
    write_word(ser, checksum)

    for b in data:
        ser.write([b])
    
    line = ser.readline()
    print(line.decode().strip())
