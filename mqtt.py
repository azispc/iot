import paho.mqtt.client as mqtt
import time

def on_message(client, userdata, message):
    print("message Temperatur", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    print(message.topic, payload)

broker_address="192.168.43.201"

print("creating new instance")
client=mqtt.Client("P1")
client.on_message=on_message

print("connecting to broker")
client.connect(broker_address)

print("Subcribing to topic", "hello2")
client.subscribe("hello2")

try:
    led = str(input("Masukkan Status LED: "))
    print(led)
    if(led=="ON"):
        client.loop_start()
        print("Publishing message to topic", "ON")
        client.publish("hello","1")
        time.sleep(4)
    else:
        client.loop_start()
        print("Publishing message to topic", "OFF")
        client.publish("hello","0")
        time.sleep(4)

    #client.loop_start()
    #print("Publishing message to topic", "hello")
    #client.publish("hello","0")
    #time.sleep(4)
except KeyboardInterrupt:
    client.loop_stop()
