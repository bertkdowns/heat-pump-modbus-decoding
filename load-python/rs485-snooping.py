import serial
import time
import pandas as pd

# -----------------------------
# Configuration
# -----------------------------
SERIAL_PORT = "/dev/ttyUSB0"   # Change as needed
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
last_rx_time = None

rows = []  # collected packets

print("RS485 sniffer started...")

# -----------------------------
# Main loop
# -----------------------------
try:
    while True:
        now = time.perf_counter()

        # Read all available bytes
        data = ser.read(ser.in_waiting or 1)

        if data:
            if last_rx_time is None:
                start_time = now

            recv_buffer.extend(data[:MAX_BUFFER_SIZE - len(recv_buffer)])
            last_rx_time = now

        # Detect message gap
        if last_rx_time is not None and (now - last_rx_time) > MESSAGE_GAP_TIMEOUT:
            receive_time_us = int(start_time * 1_000_000)

            hex_payload = " ".join(f"{b:02X}" for b in recv_buffer)

            rows.append({
                "receive_time_us": receive_time_us,
                "msg_index": msg_index,
                "uart_data": hex_payload,
                "length": len(recv_buffer)
            })

            msg_index = (msg_index + 1) % 1000
            recv_buffer.clear()
            last_rx_time = None

        time.sleep(0.0005)  # small CPU relief

except KeyboardInterrupt:
    print("\nStopping capture...")

finally:
    ser.close()

# -----------------------------
# Create DataFrame
# -----------------------------
df = pd.DataFrame(rows)

print("\nCaptured packets:")
print(df.head())

# df is now available for:
# - filtering
# - decoding Modbus
# - saving later if you want
