import socket
import json
import time
import paho.mqtt.client as mqtt

# =====================
# CONFIGURATION
# =====================

# Power meter TCP settings
METER_HOST = "192.168.1.100"   # <-- change
METER_PORT = 5025              # common SCPI port, change if needed
SOCKET_TIMEOUT = 5

# MQTT settings
MQTT_BROKER = "localhost"      # <-- change
MQTT_PORT = 1883
MQTT_TOPIC = "power_meter/measurements"
MQTT_CLIENT_ID = "power_meter_bridge"

# Polling
POLL_INTERVAL_SEC = 5

# =====================
# TCP / SCPI FUNCTIONS
# =====================

def send_command(sock, command):
    """
    Send SCPI command and read response.
    """
    cmd = command.strip() + "\n"
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
            data[key.strip()] = value.strip()
        else:
            # Fallback (shouldn't normally happen)
            data[field.strip()] = None

    return data


# =====================
# MQTT SETUP
# =====================

mqtt_client = mqtt.Client(client_id=MQTT_CLIENT_ID, clean_session=True)
mqtt_client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
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
                    "source": "power_meter",
                    "timestamp": time.time(),
                    "measurement": parsed
                }

                mqtt_client.publish(
                    MQTT_TOPIC,
                    json.dumps(payload),
                    qos=0,
                    retain=False
                )

                print("Published measurement")

        except Exception as e:
            print(f"Error: {e}")

        time.sleep(POLL_INTERVAL_SEC)


if __name__ == "__main__":
    main()
