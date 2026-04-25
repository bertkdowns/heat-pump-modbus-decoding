import serial
import time
import pandas as pd
from parse_csv import PacketParser, PacketParseError
from mqtt_adapter import MQTTAdapter
# -----------------------------
# Configuration
# -----------------------------
SERIAL_PORT = "/dev/serial/by-id/usb-DFRobot_www.dfrobot.com__0043_34330313431351700141-if00"   # /dev/ttyACM0, Change as needed
BAUDRATE = 4800               # Modbus RTU baud
MESSAGE_GAP_TIMEOUT = 0.005   # 5 ms
MAX_BUFFER_SIZE = 700

# -----------------------------
# Serial setup
# -----------------------------
ser = serial.Serial(
    port=SERIAL_PORT,
    baudrate=BAUDRATE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=0  # non-blocking
)

# -----------------------------
# State
# -----------------------------
recv_buffer = bytearray()
msg_index = 0
start_time = time.perf_counter()
last_rx_time = None


print("RS485 sniffer started...")

def flip_bytes(data):
    # not inverts the bytes, &0xff masks so that it is only one byte long.
    flipped_bytes = bytes([~b & 0xFF for b in data])
    return flipped_bytes

# -----------------------------
# Main loop
# -----------------------------
try:
    parser = PacketParser()
    mqtt_adapter = MQTTAdapter()
    
    while True:
        now = time.perf_counter()

        # Read all available bytes
        data = ser.read(ser.in_waiting or 1)

        if data:
            if last_rx_time is None:
                start_time = now

            #data = flip_bytes(data)


            recv_buffer.extend(data[:MAX_BUFFER_SIZE - len(recv_buffer)])
            last_rx_time = now
        

        # Detect message gap
        if last_rx_time is not None and (now - last_rx_time) > MESSAGE_GAP_TIMEOUT:
            receive_time_us = int(start_time * 1_000_000)

            hex_payload = " ".join(f"{b:02X}" for b in recv_buffer)
            print("[DEBUG] Captured packet:", hex_payload)
            try:
                device, registers = parser.parse_packet(recv_buffer)  # just for debug parsing

                mqtt_adapter.publish_modbus_data(device,registers)
                print("capytured well!")

            except PacketParseError as e:
                print(f"[WARNING] Failed to parse packet: {e}")



            msg_index = (msg_index + 1) % 1000
            recv_buffer.clear()
            last_rx_time = None

        time.sleep(0.000005)  # small CPU relief

except KeyboardInterrupt:
    print("\nStopping capture...")

finally:
    ser.close()
