#   Simple UART Control Protocol
#   first byte = always 0x00 second byte = 0x01 is digitalread 0x02 is analogRead,
#    0x03 is digitalwrite, 0x04 is analogWrite 
#    third byte is pin number (0-20)
#    fourth byte (if digitalwrite or analogwrite) is value 0 or 1 for digital or 0-255 for analog 
#    a 5ms delay between reading on the serial clears the serial buffer. 
#    if a read command is recieved, one byte of digital or analog data is returned over uart. 
#    also write a library of python functions to run these. 
#    Make sure to also handle switching the pins to the correct pinmode.


import serial
import time

START = 0x00

CMD_DIGITAL_READ  = 0x01
CMD_ANALOG_READ   = 0x02
CMD_DIGITAL_WRITE = 0x03
CMD_ANALOG_WRITE  = 0x04

class ArduinoUART:
    def __init__(self, port, baudrate=115200, timeout=0.1):
        self.ser = serial.Serial(port, baudrate, timeout=timeout)
        time.sleep(2)  # Arduino reset

    def _send(self, data):
        self.ser.reset_input_buffer()
        self.ser.write(bytes(data))

    def digital_write(self,pin, value: bool):
        self._send([START, CMD_DIGITAL_WRITE,pin, 1 if value else 0])
        self.read_confirmation()

    def analog_write(self,pin, value: float):
        pwm = int(max(0, min(255, value * 255)))
        self._send([START, CMD_ANALOG_WRITE,pin, pwm])
        self.read_confirmation()

    def digital_read(self,pin) -> bool:
        self._send([START, CMD_DIGITAL_READ,pin])
        b = self.ser.read(1)
        if not b:
            raise TimeoutError("No response")
        return bool(b[0])

    def analog_read(self,pin) -> float:
        self._send([START, CMD_ANALOG_READ,pin])
        b = self.ser.read(1)
        if not b:
            raise TimeoutError("No response")
        return b[0] / 255.0
    
    def read_confirmation(self) -> str:
        # one 0x00 byte is sent back to confirm write commands
        b = self.ser.read(1)
        if not b:
            raise TimeoutError("No confirmation received")

    def close(self):
        self.ser.close()