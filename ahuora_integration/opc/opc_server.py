import json
import threading
from typing import Dict

import paho.mqtt.client as mqtt
from opcua import ua, Server


MQTT_BROKER = "localhost"
MQTT_PORT = 1883

OPC_ENDPOINT = "opc.tcp://0.0.0.0:4840/mqtt-opc/"
OPC_NAMESPACE = "urn:mqtt:adapter"


class MqttToOpcAdapter:
    def __init__(self):
        # OPC UA setup
        self.server = Server()
        self.server.set_endpoint(OPC_ENDPOINT)

        self.idx = self.server.register_namespace(OPC_NAMESPACE)
        self.objects = self.server.get_objects_node()

        self.topic_nodes: Dict[str, ua.NodeId] = {}

        # MQTT setup
        self.mqtt = mqtt.Client()
        self.mqtt.on_connect = self.on_connect
        self.mqtt.on_message = self.on_message

    # ---------- OPC UA ----------
    def start_opc(self):
        self.server.start()
        print(f"OPC UA server started at {OPC_ENDPOINT}")

    def stop_opc(self):
        self.server.stop()

    def get_or_create_topic_node(self, topic: str):
        if topic in self.topic_nodes:
            return self.topic_nodes[topic]

        # OPC UA node names can't contain '/'
        node_name = topic.replace("/", "_")
        node = self.objects.add_object(self.idx, node_name)
        self.topic_nodes[topic] = node
        print(f"Created OPC node for topic: {topic}")
        return node

    # ---------- MQTT ----------
    def on_connect(self, client, userdata, flags, rc):
        print("Connected to MQTT broker")
        client.subscribe("#")  # listen to ALL topics

    def on_message(self, client, userdata, msg):
        payload = msg.payload.decode(errors="ignore")
        topic_node = self.get_or_create_topic_node(msg.topic)

        # Try JSON first
        try:
            data = json.loads(payload)
            if isinstance(data, dict):
                self.handle_json(topic_node, data)
                return
        except json.JSONDecodeError:
            pass

        # Try numeric
        try:
            value = float(payload)
            self.handle_scalar(topic_node, value)
        except ValueError:
            print(f"Ignored non-numeric, non-JSON payload on {msg.topic}")

    # ---------- Payload handling ----------
    def handle_json(self, topic_node, data: dict):
        for key, value in data.items():
            var = self.get_or_create_variable(topic_node, key, value)
            var.set_value(value)

    def handle_scalar(self, topic_node, value: float):
        var = self.get_or_create_variable(topic_node, "value", value)
        var.set_value(value)

    def get_or_create_variable(self, parent, name, value):
        try:
            return parent.get_child(f"{self.idx}:{name}")
        except Exception:
            var = parent.add_variable(self.idx, name, value)
            var.set_writable()
            print(f"Created variable: {parent} → {name}")
            return var

    # ---------- Run ----------
    def start(self):
        self.start_opc()
        self.mqtt.connect(MQTT_BROKER, MQTT_PORT, 60)

        mqtt_thread = threading.Thread(target=self.mqtt.loop_forever, daemon=True)
        mqtt_thread.start()


if __name__ == "__main__":
    adapter = MqttToOpcAdapter()
    adapter.start()

    try:
        while True:
            pass
    except KeyboardInterrupt:
        adapter.stop_opc()
