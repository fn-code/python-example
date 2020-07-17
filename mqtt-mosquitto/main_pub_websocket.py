import paho.mqtt.client as mqtt
import json


gps_sensor = {
    "sensor_id": "GPS_P1",
    "type": "gps_sensor",
    "payload": {
        "latitude": 0.123123,
        "longitude": 123.1232323
    }
}

temp_sensor = {
    "sensor_id": "TEMP_P1",
    "type": "temp_sensor",
    "payload": {
        "humidity": 0.123123,
    }
}

broker_address = "localhost"
broker_port = 9003
broker_keepalive = 60

MQTT_TOPIC_GPS = "p1/gps"
MQTT_TOPIC_TEMP = "p1/temp"
MQTT_TOPIC_ALTITUDE = "p1/altitude"


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))


def on_publish(client, userdata, mid):
    print("Message Published...", mid)


def on_log(client, userdata, level, buf):
    print("log"+ buf)


client = mqtt.Client(client_id="P2", transport="websockets")
client.on_connect = on_connect
client.on_publish = on_publish
client.on_log = on_log
client.connect(broker_address, broker_port)


client.loop_start()

payload = json.dumps(gps_sensor)
client.publish(MQTT_TOPIC_GPS, payload)

client.loop_stop()
client.disconnect()