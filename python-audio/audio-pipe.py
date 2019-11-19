import serial
import sys

BAUD_RATE = 16 * 441 * 100  # 16bit 44.1kHz samplerate
with serial.Serial('/dev/cu.usbmodem14201', BAUD_RATE) as ser:
    while True:
        sys.stdout.buffer.write(ser.read(1024))
