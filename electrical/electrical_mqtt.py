import socket
import json
import time
import paho.mqtt.client as mqtt

# =====================
# CONFIGURATION
# =====================

# Power meter TCP settings
METER_HOST = "192.168.1.2"   # <-- change
METER_PORT = 3365              # common SCPI port, change if needed
SOCKET_TIMEOUT = 5
POWER_METER_NAME = "powermeter"  # <-- change

# MQTT settings
MQTT_BROKER = "localhost"      # <-- change
MQTT_PORT = 1883
MQTT_CLIENT_ID = "power_meter_bridge"

# Polling
POLL_INTERVAL_SEC = 1

# =====================
# TCP / SCPI FUNCTIONS
# =====================

def send_command(sock, command):
    """
    Send SCPI command and read response.
    """
    cmd = command.strip() + "\r\n"
    sock.sendall(cmd.encode("ascii"))

    response = sock.recv(65535)
    return response.decode("ascii").strip()


def parse_measurement(response):
    """
    Convert:
      Date 2026,01,20;Time 12,20,05;Status 00000000;U1_Ins 13.6E+00;...
    into a dict.
    """
    data = {}

    fields = response.split(";")
    for field in fields:
        if not field.strip():
            continue

        # Split on first space only
        if " " in field:
            key, value = field.split(" ", 1)
            key = key.strip()
            value = value.strip()

            # Keep Date/Time and Status as strings
            if key in ("Date", "Time") or key.lower().startswith("status"):
                data[key] = value
                continue

            # Try to coerce numeric-looking values to float so Telegraf/Inﬂux store them as numbers
            try:
                # Some values are in scientific notation like 96.9E+00
                num = float(value)
                data[key] = num
            except (ValueError, TypeError):
                # Fallback to original string (including when value is empty or non-numeric)
                data[key] = value
        else:
            # Fallback (shouldn't normally happen)
            data[field.strip()] = None

    return data


# =====================
# MQTT SETUP
# =====================

mqtt_client = mqtt.Client(client_id=MQTT_CLIENT_ID)
mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.loop_start()


# =====================
# MAIN LOOP
# =====================

def main():
    while True:
        try:
            with socket.create_connection(
                (METER_HOST, METER_PORT),
                timeout=SOCKET_TIMEOUT
            ) as sock:

                # Optional: ensure headers are ON
                send_command(sock, ":HEADER ON")

                # Request measurement
                response = send_command(sock, ":MEASURE:POWER?")

                # Parse response
                parsed = parse_measurement(response)

                # Build MQTT payload
                payload = {
                    "device": POWER_METER_NAME,
                    "timestamp": time.time(),
                    "val1":34.12
                }
                #payload.update(parsed)

                mqtt_client.publish(
                    f"power/{POWER_METER_NAME}/measurements",
                    json.dumps(payload),
                    qos=0,
                    retain=True
                )

                print("Published measurement")

        except Exception as e:
            print(f"Error: {e}")

        time.sleep(POLL_INTERVAL_SEC)


if __name__ == "__main__":
    main()
