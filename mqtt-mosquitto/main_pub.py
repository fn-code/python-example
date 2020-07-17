import paho.mqtt.client as mqtt
import json


gps_sensor = {
    "sensor_id": "GPS_P1",
    "type": "gps_sensor1",
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

broker_address = "0.0.0.0"
# user port 9001 or 9003 for websocket
broker_port = 9003
broker_keepalive = 60

MQTT_TOPIC_GPS = "p1/gps"
MQTT_TOPIC_TEMP = "p1/temp"
MQTT_TOPIC_ALTITUDE = "p1/altitude"

MQTT_TOPIC_INFO = "p1/info"
MQTT_TOPIC_INFO_REQ = "p1/info/req"

mqtt_is_connected = False

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    mqtt_is_connected = True


def on_publish(client, userdata, mid):
    print("Message Published...", mid)


def on_log(client, userdata, level, buf):
    print("log"+ buf)


def send_temp_data(client):
    payload = json.dumps(temp_sensor)
    client.publish(MQTT_TOPIC_TEMP, payload)


def send_gps_data(client):
    payload = json.dumps(gps_sensor)
    client.publish(MQTT_TOPIC_GPS, payload)


def send_info_data(client):
    client.publish(MQTT_TOPIC_INFO_REQ, "REQ")


def on_message_info(client, userdata, message):
    payload = json.loads(message.payload)
    print("message received device information ", payload)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)


client = mqtt.Client(client_id="P2", transport="websockets")
client.on_connect = on_connect
client.on_publish = on_publish
client.connect(broker_address, broker_port)

client.loop_start()
# subscribe topic
print("Set subscribe topic!")
client.message_callback_add(MQTT_TOPIC_INFO, on_message_info)
client.subscribe(MQTT_TOPIC_INFO, 1)

while True:
    print("1. SEND TEMP DATA")
    print("2. SEND GPS DATA")
    print("3. SEND REQ DEVICE INFO")
    args = input("send acction ? ")
    if args == "1":
        send_temp_data(client)
    elif args == "2":
        send_gps_data(client)
    elif args == "3":
        send_info_data(client)
    else:
        client.loop_stop()
        client.disconnect()
        mqtt_is_connected = False
        break





