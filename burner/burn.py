#!/usr/bin/env python3

import serial
import time
import random

def write_word(ser, word):
    ser.write([word & 0xff, word >> 8])

with serial.Serial('/dev/cu.usbmodem14301', 115200) as ser:
    # Need to read the first byte to prime the channel
    c = ser.read()

    ser.write(b's')
    line = ser.readline()
    print(line.decode().strip())

    data = []
    for i in range(8192):
        data.append(random.randrange(256))

    length = len(data)
    offset = 0

    checksum = 0
    for i in range(len(data)):
        checksum += checksum + data[i]
        checksum = checksum & 0xffff

    ser.write(b'w')
    write_word(ser, length)
    write_word(ser, offset)
    write_word(ser, checksum)

    sent = 0
    percent = 10
    for b in data:
        ser.write([b])
        c = ser.read()
        sent += 1
        if sent / len(data) > percent / 100:
            print(f'Sent {percent}%')
            percent += 10
        if c != b'+':
            print('Received', c)
    print('Sent 100%')

    line = ser.readline()
    print(line.decode().strip())
