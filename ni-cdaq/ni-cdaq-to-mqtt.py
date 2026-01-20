import time
import json
import nidaqmx
from nidaqmx.system import System
from nidaqmx.constants import TemperatureUnits, ThermocoupleType
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

# We are assuming the system has the current setup:
#   - Device 0: cDAQ9189 chassis
#   - Device 1: Voltage input module
#   - Device 2: Current input module 
#   - Device 3: Thermocouple input module, T type 



if not system.devices:
    raise RuntimeError("No NI-DAQmx devices found")

device = system.devices[0]  # or System.local().devices["cDAQ9189-XXXX"]
device_name = device.name

# If you want to ignore some devices, comment them out and replace them with an empty list
voltage_ai_channels = list(system.devices[1].ai_physical_chans)
current_ai_channels = list(system.devices[2].ai_physical_chans)
thermocouple_ai_channels = list(system.devices[3].ai_physical_chans)

# -----------------------
# MQTT setup
# -----------------------
mqtt_client = mqtt.Client(client_id=f"nidaq-{device_name}")
mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.loop_start()

def publish_mqtt_data(device_name: str, device_data: dict[str,float]):
    topic = f"{MQTT_BASE_TOPIC}/{device_name}/snapshot"

    payload = {
        "device": device_name,
        "timestamp": time.time(),
    }

    mqtt_client.publish(
        topic,
        json.dumps(payload),
        qos=MQTT_QOS,
        retain=MQTT_RETAIN,
    )



# -----------------------
# DAQ task 
# -----------------------
with nidaqmx.Task() as task:
    all_channels = voltage_ai_channels + current_ai_channels + thermocouple_ai_channels
    for ch in voltage_ai_channels:
        task.ai_channels.add_ai_voltage_chan(ch.name)
    for ch in current_ai_channels:
        task.ai_channels.add_ai_current_chan(ch.name)
    for ch in thermocouple_ai_channels:
        task.ai_channels.add_ai_thrmcpl_chan(ch.name, thermocouple_type=ThermocoupleType.T, units=TemperatureUnits.DEG_C)

    print("Found channels:")
    for ch in all_channels:
        print(f" - {ch.name}")
    print("Starting on-demand acquisition...")

    try:
        while True:
            values = task.read()

            device_data = {}
            for ch, value in zip(all_channels, values):
                device_data[ch.name] = float(value)

            publish_mqtt_data(device_name, device_data)

            time.sleep(PUBLISH_INTERVAL)

    except KeyboardInterrupt:
        print("Stopping...")
    finally:
        mqtt_client.loop_stop()
        mqtt_client.disconnect()
