import paho.mqtt.client as mqtt
import json
import time
# -----------------------
# Configuration
# -----------------------
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_BASE_TOPIC = "modbus"

MQTT_QOS = 0
MQTT_RETAIN = True


class MQTTAdapter:




    def __init__(self):
        self.mqtt_client = mqtt.Client(client_id=f"MODBUS-adapter")
        self.mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
        self.mqtt_client.loop_start()
        
    def publish_modbus_data(self, device_id: int, data: dict[int,int]):
        # Publish the message to the MQTT broker
        topic = f"{MQTT_BASE_TOPIC}/device/{device_id}/registers"
        
        # Add timestamp and device_id to payload
        payload = {
            "timestamp": time.time(),  # unix timestamp
            "device_id": str(device_id),
        }
        # Add register data
        payload.update({str(reg): val for reg, val in data.items()})
        
        self.mqtt_client.publish(
            topic,
            json.dumps(payload),
            qos=MQTT_QOS,
            retain=MQTT_RETAIN
        )

        pass