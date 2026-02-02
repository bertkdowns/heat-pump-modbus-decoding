import paho.mqtt.client as mqtt
import json

BROKER = "localhost"      # or IP / hostname
PORT = 1883

client = mqtt.Client()

def preprocess_dev0(client, userdata,msg: mqtt.MQTTMessage):

    # Example preprocessing function for device 0
    data = json.loads(msg.payload.decode())
    if "2023" not in data:
        return
    inlet_temp = data["2023"] / 10
    outlet_temp = data["2024"] / 10
    air_ambient_temp = data["2026"] / 10
    coil_temp = data["2027"] / 10
    low_pressure = data["2087"] / 10 # bar

    client.publish("processed/inlet_temp", inlet_temp)
    client.publish("processed/outlet_temp", outlet_temp)
    client.publish("processed/air_ambient", air_ambient_temp)
    client.publish("processed/air_ambient2", air_ambient_temp)
    client.publish("processed/coil_temp", coil_temp)
    client.publish("processed/pressure_low", low_pressure)

topic_map: dict[str,mqtt.CallbackOnMessage] = {
    "modbus/device/0/registers": preprocess_dev0,
}




def main():
    client.connect(BROKER, PORT, keepalive=60)
    for topic, func in topic_map.items():
        client.message_callback_add(topic, func)
    client.subscribe([(key, 0) for key in topic_map.keys()])
    client.loop_forever()


if __name__ == "__main__":
    main()
