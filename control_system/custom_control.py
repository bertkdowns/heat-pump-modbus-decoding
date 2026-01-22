from arduino_uart import ArduinoUART

arduino = ArduinoUART("/dev/ttyACM0")


import time
import sys
import paho.mqtt.client as mqtt

# -------------------------
# Configuration
# -------------------------
MQTT_BROKER = "localhost"   # change if needed
MQTT_PORT = 1883

TOPICS = [
    ("valve/write", 0),
    ("vsd1/write", 0),
    ("vsd2/write", 0),
]

ARDUINO_PORT = "/dev/ttyUSB0"  # Windows: COM3, COM4, etc
VALVE_PWM_PIN = 6             # D6 (PWM)
VSD_1_PWM_PIN = 10            # D10 (PWM)
VSD_2_PWM_PIN = 11            # D11 (PWM)           


arduino = ArduinoUART(ARDUINO_PORT)
# -------------------------
# MQTT Callbacks
# -------------------------
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
        for topic, qos in TOPICS:
            client.subscribe(topic, qos)
            print(f"Subscribed to {topic}")
    else:
        print("MQTT connection failed:", rc)

def clamp(value, min_val=0.0, max_val=1.0):
    return max(min_val, min(max_val, value))

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode().strip()

    try:
        value = float(payload)
        value = clamp(value)

    except ValueError:
        print(f"Ignoring invalid payload on {topic}: {payload}")
        return

    if topic == "valve/write":
        arduino.analog_write(VALVE_PWM_PIN, value)
        print(f"Valve PWM set to {value:.2f}")

    elif topic == "vsd1/write":
        print(f"VSD1 received {value}")
        arduino.analog_write(VSD_1_PWM_PIN, value)

    elif topic == "vsd2/write":
        print(f"VSD2 received {value}")
        arduino.analog_write(VSD_2_PWM_PIN, value)
# -------------------------
# MQTT Client
# -------------------------
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

# -------------------------
# Main Loop
# -------------------------
try:
    client.loop_forever()

except KeyboardInterrupt:
    print("\nShutting down...")

finally:
    valve_pin.write(0.0)
    board.exit()
    sys.exit(0)
