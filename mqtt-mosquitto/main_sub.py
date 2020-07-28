import paho.mqtt.client as mqtt
import json


broker_address = "0.0.0.0"
broker_port = 1883
broker_keepalive = 60

MQTT_TOPIC_GPS = "p1/gps"
MQTT_TOPIC_TEMP = "p1/temp"
MQTT_TOPIC_INFO = "p1/info"
MQTT_TOPIC_INFO_REQ = "p1/info/req"

device_info = {
    "device_id": "P1",
    "device_name": "DEVICE 1"
}


def on_log(client, userdata, level, buf):
    print("log: "+ buf)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected with result code "+str(rc))
    else:
        print("Invalid to connected, with result "+ rc)


def on_message_gps(client, userdata, message):
    payload = json.loads(message.payload)
    print("message received gps sensor ", payload)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)


def on_message_temp(client, userdata, message):
    payload = json.loads(message.payload)
    print("message received temp sensor ", payload)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)


def on_message_info(client, userdata, message):
    client.publish(MQTT_TOPIC_INFO, json.dumps(device_info))


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection.")
    else:
        print("Conection disconnected.")


def run(client):
    client.on_connect = on_connect
    client.on_log = on_log

    # connected to mqtt broker server
    client.connect(broker_address, broker_port, broker_keepalive)

    # subcribes using callback
    client.message_callback_add(MQTT_TOPIC_GPS, on_message_gps)
    client.message_callback_add(MQTT_TOPIC_TEMP, on_message_temp)
    client.message_callback_add(MQTT_TOPIC_INFO_REQ, on_message_info)

    client.subscribe([(MQTT_TOPIC_TEMP, 1), (MQTT_TOPIC_GPS, 1), (MQTT_TOPIC_INFO_REQ, 1)])

    # set connection forever
    client.loop_forever()


def close_connection(client):
    client.disconnect()


if __name__ == '__main__':
    try:
        client = mqtt.Client("P1")
        run(client)
    except KeyboardInterrupt:
        pass
    finally:
        close_connection(client)
