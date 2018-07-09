import os
import serial
import sys

chunk_size = 4094
ser = serial.Serial('/dev/ttyUSB0', 115200)
ser.write(b'\1');
f = open(sys.argv[1], "rb")
f_infos = os.stat(sys.argv[1])
size = f_infos.st_size
ser.write(size)
ser.write(chunk_size)

i = 0
while i * chunk_size < size:
    data = f.read(chunk_size)
    ser.write(data)
    i = i + 1
