import time
import sys
from pyfirmata import Arduino, util
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
# -------------------------
# Arduino Setup
# -------------------------
print("Connecting to Arduino...")
board = Arduino(ARDUINO_PORT)

# Required for stable Firmata operation
it = util.Iterator(board)
it.start()

valve_pin = board.get_pin(f"d:{VALVE_PWM_PIN}:p")
valve_pin.write(1.0) # initialise to open
vsd1_pin = board.get_pin(f"d:{VSD_1_PWM_PIN}:p")
vsd2_pin = board.get_pin(f"d:{VSD_2_PWM_PIN}:p")
vsd1_pin.write(0.0) # initialise to off
vsd2_pin.write(0.0) # initialise to off

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
        valve_pin.write(value)
        print(f"Valve PWM set to {value:.2f}")

    elif topic == "vsd1/write":
        print(f"VSD1 received {value}")
        vsd1_pin.write(value)

    elif topic == "vsd2/write":
        print(f"VSD2 received {value}")
        vsd2_pin.write(value)

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
