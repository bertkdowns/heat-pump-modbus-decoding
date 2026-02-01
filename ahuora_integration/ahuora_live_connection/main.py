from ahuora_live.solver import AhuoraLiveSolver, ScenarioTagMap, PropertyTagMap, PropertyValueId
import json
import time
import threading
from typing import Any, Dict
from common.models.idaes import FlowsheetSchema
from pathlib import Path


import paho.mqtt.client as mqtt
# Make a mapping between mqtt topics and data tags.
# load in the AhuoraLiveSolver with the scenario
# Store all the recent data from topics and periodically solve
# publish back to new mqtt topics

# Config format:
# {
# "my/mqtt/topic1": "data_tag_1",
# "my/mqtt/topic2": {
#     "my_json_key": "data_tag_2"
# }
#  should result in a persistant store of most recent values like:
# {
# data_tag_1: 23.4
# data_tag_2: 45.6
# }

# -----------------------------
# Bridge
# -----------------------------
class MQTTSolverBridge:
    def __init__(
        self,
        broker: str,
        topic_tag_map: Dict[str, Any],
        scenario: str,
        scenario_tag_map: ScenarioTagMap,
        solve_interval_s: int = 5,
        publish_prefix: str = "solver/output/",
    ):
        self.topic_tag_map = topic_tag_map
        self.latest_values: Dict[str, float] = {}

        self.solve_interval_s = solve_interval_s
        self.publish_prefix = publish_prefix


        schema = FlowsheetSchema.model_validate_json(Path(scenario).read_text())


        self.solver = AhuoraLiveSolver(schema, scenario_tag_map)

        self.client = mqtt.Client()
        self.client.on_message = self._on_message
        self.client.connect(broker)

        for topic in topic_tag_map:
            self.client.subscribe(topic)

    # -------------------------
    # MQTT callback
    # -------------------------
    def _on_message(self, client, userdata, msg):
        topic = msg.topic
        payload = msg.payload.decode()

        mapping = self.topic_tag_map.get(topic)
        if mapping is None:
            return

        try:
            data = json.loads(payload)
        except json.JSONDecodeError:
            data = payload

        # Scalar mapping
        if isinstance(mapping, str):
            try:
                self.latest_values[mapping] = float(data)
            except (ValueError, TypeError):
                pass

        # JSON key mapping
        elif isinstance(mapping, dict) and isinstance(data, dict):
            for json_key, tag in mapping.items():
                if json_key in data:
                    try:
                        self.latest_values[tag] = float(data[json_key])
                    except (ValueError, TypeError):
                        pass

    # -------------------------
    # Solver loop
    # -------------------------
    def _solve_loop(self):
        while True:
            time.sleep(self.solve_interval_s)

            if not self.latest_values:
                continue

            results = self.solver.solve(self.latest_values)
            print(f"Publishing tags to MQTT with prefix {self.publish_prefix}")
            for tag, value in results.values.items():
                topic = f"{self.publish_prefix}{tag}"
                self.client.publish(topic, value)

    # -------------------------
    # Run
    # -------------------------
    def start(self):
        threading.Thread(target=self._solve_loop, daemon=True).start()
        self.client.loop_forever()


# -----------------------------
# Entry point
# -----------------------------
if __name__ == "__main__":
    TOPIC_TAG_MAP = {
        "valve/write": "ValvePosition",
        "modbus/device/0/registers": {
            "2027": "TemperatureTag"
        }
    }

    scenario_tag_map : ScenarioTagMap = {
    "ValvePosition": PropertyTagMap(
        property= PropertyValueId(338165),
        units= "mol/s"
    ),
    "TemperatureTag": PropertyTagMap(
        property= PropertyValueId(338263),
        units="mol/s"
    )
}

    bridge = MQTTSolverBridge(
        broker="localhost",
        topic_tag_map=TOPIC_TAG_MAP,
        scenario="/Scratch/Ahuora-Live/src/configs/plant_hp/scenario.json",
        scenario_tag_map=scenario_tag_map,
        solve_interval_s=10,
    )

    bridge.start()