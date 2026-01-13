import time
import json
import nidaqmx
from nidaqmx.system import System
import paho.mqtt.client as mqtt

# -----------------------
# Configuration
# -----------------------
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_BASE_TOPIC = "nidaq"

PUBLISH_INTERVAL = 1.0  # seconds
MQTT_QOS = 0
MQTT_RETAIN = True

# -----------------------
# Discover device & channels
# -----------------------
system = System.local()

if not system.devices:
    raise RuntimeError("No NI-DAQmx devices found")

device = system.devices[1]  # or System.local().devices["cDAQ9189-XXXX"]
device_name = device.name

ai_channels = list(device.ai_physical_chans)

if not ai_channels:
    raise RuntimeError(f"No analog input channels found on {device_name}")

print(f"Using device: {device_name}")
for ch in ai_channels:
    print(f"  - {ch.name}")

# -----------------------
# MQTT setup
# -----------------------
mqtt_client = mqtt.Client(client_id=f"nidaq-{device_name}")
mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.loop_start()

# -----------------------
# DAQ task (on-demand)
# -----------------------
with nidaqmx.Task() as task:
    for ch in ai_channels:
        task.ai_channels.add_ai_voltage_chan(ch.name)

    print("Starting on-demand acquisition...")

    try:
        while True:
            values = task.read()

            # Normalize shape
            if len(ai_channels) == 1:
                values = [values]

            payload = {
                "device": device_name,
                "timestamp": time.time(),
            }

            for ch, value in zip(ai_channels, values):
                channel_name = ch.name.split("/")[-1]
                payload[channel_name] = float(value)

            topic = f"{MQTT_BASE_TOPIC}/{device_name}/snapshot"

            mqtt_client.publish(
                topic,
                json.dumps(payload),
                qos=MQTT_QOS,
                retain=MQTT_RETAIN,
            )

            time.sleep(PUBLISH_INTERVAL)

    except KeyboardInterrupt:
        print("Stopping...")
    finally:
        mqtt_client.loop_stop()
        mqtt_client.disconnect()
