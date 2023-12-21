import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
from datetime import datetime
import time

port = 8086
database = 'sensorDB'
username = 'ssm'
password = ''

topic = "TeamET/#"
server = "54.173.215.51"
measurement = 'happyHome'
room1_flag = [0.0, 0]
room2_flag = [0.0, 0]
room3_flag = [0.0, 0]
Lroom_flag = [0.0, 0]

def on_connect(client, userdata, flags, rc):
    print("Connected with RC : " + str(rc))
    client.subscribe(topic)

def on_message(client, userdata, msg):
    global measurement
    global room1_flag
    global room2_flag
    global room3_flag
    global Lroom_flag

    sensor_place, sensor_topic = msg.topic.split("/")[1], msg.topic.split("/")[-1]
    if sensor_topic == "cmd":
        return
    if msg.payload.decode('utf-8') == "nan":
        return
    data = float(msg.payload.decode('utf-8'))

    if sensor_place == "room1":
        tags = {'location': 'room1'}
        if sensor_topic == "humid":
            room1_flag[0] = data
            if data >= 70:
                client.publish("TeamET/room1/servo/cmd", "open")
                client.publish("TeamET/room1/neopixel/cmd", "greenx")
            elif data <= 50:
                client.publish("TeamET/room1/servo/cmd", "close")
                client.publish("TeamET/room1/neopixel/cmd", "green")
            else:
                client.publish("TeamET/room1/servo/cmd", "close")
                client.publish("TeamET/room1/neopixel/cmd", "greenx")

        elif sensor_topic == "temp":
            room1_flag[1] = data
            if data >= 23:
                client.publish("TeamET/room1/neopixel/cmd", "redx")
                client.publish("TeamET/room1/neopixel/cmd", "blue")
            elif data <= 22:
                client.publish("TeamET/room1/neopixel/cmd", "bluex")
                client.publish("TeamET/room1/neopixel/cmd", "red")
            else:
                client.publish("TeamET/room1/neopixel/cmd", "bluex")
                client.publish("TeamET/room1/neopixel/cmd", "redx")

        elif sensor_topic == "light":
            if data < 800:
                client.publish("TeamET/room1/neopixel/cmd", "white")
            else:
                client.publish("TeamET/room1/neopixel/cmd", "whitex")

        if room1_flag[0] != 0.0 and room1_flag[1] != 0:
            fields = {'humidity_evt': room1_flag[0], 'temperature_evt': room1_flag[1]}
            json_body = [
                {
                    "measurement": measurement,
                    "tags": tags,
                    "time": datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ'),
                    "fields": fields
                }
            ]
            client_db.write_points(json_body)
            room1_flag[0] = 0.0
            room1_flag[1] = 0

    elif sensor_place == "room2":
        tags = {'location': 'room2'}
        if sensor_topic == "humid":
            room2_flag[0] = data
            if data >= 70:
                client.publish("TeamET/room2/servo/cmd", "open")
                client.publish("TeamET/room2/neopixel/cmd", "greenx")
            elif data <= 50:
                client.publish("TeamET/room2/servo/cmd", "close")
                client.publish("TeamET/room2/neopixel/cmd", "green")
            else:
                client.publish("TeamET/room2/servo/cmd", "close")
                client.publish("TeamET/room2/neopixel/cmd", "greenx")

        elif sensor_topic == "temp":
            room2_flag[1] = data
            if data >= 23:
                client.publish("TeamET/room2/neopixel/cmd", "redx")
                client.publish("TeamET/room2/neopixel/cmd", "blue")
            elif data <= 22:
                client.publish("TeamET/room2/neopixel/cmd", "bluex")
                client.publish("TeamET/room2/neopixel/cmd", "red")
            else:
                client.publish("TeamET/room2/neopixel/cmd", "bluex")
                client.publish("TeamET/room2/neopixel/cmd", "redx")

        elif sensor_topic == "light":
            if data < 800:
                client.publish("TeamET/room2/neopixel/cmd", "white")
            else:
                client.publish("TeamET/room2/neopixel/cmd", "whitex")

        if room2_flag[0] != 0.0 and room2_flag[1] != 0:
            fields = {'humidity_evt': room2_flag[0], 'temperature_evt': room2_flag[1]}
            json_body = [
                {
                    "measurement": measurement,
                    "tags": tags,
                    "time": datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ'),
                    "fields": fields
                }
            ]
            client_db.write_points(json_body)
            room2_flag[0] = 0.0
            room2_flag[1] = 0

    elif sensor_place == "room3":
        tags = {'location': 'room3'}
        if sensor_topic == "humid":
            room3_flag[0] = data
            if data >= 70:
                client.publish("TeamET/room3/servo/cmd", "open")
                client.publish("TeamET/room3/neopixel/cmd", "greenx")
            elif data <= 50:
                client.publish("TeamET/room3/servo/cmd", "close")
                client.publish("TeamET/room3/neopixel/cmd", "green")
            else:
                client.publish("TeamET/room3/servo/cmd", "close")
                client.publish("TeamET/room3/neopixel/cmd", "greenx")

        elif sensor_topic == "temp":
            room3_flag[1] = data
            if data >= 23:
                client.publish("TeamET/room3/neopixel/cmd", "redx")
                client.publish("TeamET/room3/neopixel/cmd", "blue")
            elif data <= 22:
                client.publish("TeamET/room3/neopixel/cmd", "bluex")
                client.publish("TeamET/room3/neopixel/cmd", "red")
            else:
                client.publish("TeamET/room3/neopixel/cmd", "bluex")
                client.publish("TeamET/room3/neopixel/cmd", "redx")

        elif sensor_topic == "light":
            if data < 800:
                client.publish("TeamET/room3/neopixel/cmd", "white")
            else:
                client.publish("TeamET/room3/neopixel/cmd", "whitex")

        if room3_flag[0] != 0.0 and room3_flag[1] != 0:
            fields = {'humidity_evt': room3_flag[0], 'temperature_evt': room3_flag[1]}
            json_body = [
                {
                    "measurement": measurement,
                    "tags": tags,
                    "time": datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ'),
                    "fields": fields
                }
            ]
            client_db.write_points(json_body)
            room3_flag[0] = 0.0
            room3_flag[1] = 0

    elif sensor_place == "Lroom":
        tags = {'location': 'Lroom'}
        if sensor_topic == "humid":
            Lroom_flag[0] = data
            if data >= 70:
                client.publish("TeamET/Lroom/servo/cmd", "open")
                client.publish("TeamET/Lroom/neopixel/cmd", "greenx")
            elif data <= 50:
                client.publish("TeamET/Lroom/servo/cmd", "close")
                client.publish("TeamET/Lroom/neopixel/cmd", "green")
            else:
                client.publish("TeamET/Lroom/servo/cmd", "close")
                client.publish("TeamET/Lroom/neopixel/cmd", "greenx")

        elif sensor_topic == "temp":
            Lroom_flag[1] = data
            if data >= 23:
                client.publish("TeamET/Lroom/neopixel/cmd", "redx")
                client.publish("TeamET/Lroom/neopixel/cmd", "blue")
            elif data <= 22:
                client.publish("TeamET/Lroom/neopixel/cmd", "bluex")
                client.publish("TeamET/Lroom/neopixel/cmd", "red")
            else:
                client.publish("TeamET/Lroom/neopixel/cmd", "bluex")
                client.publish("TeamET/Lroom/neopixel/cmd", "redx")

        elif sensor_topic == "light":
            if data < 800:
                client.publish("TeamET/Lroom/neopixel/cmd", "white")
            else:
                client.publish("TeamET/Lroom/neopixel/cmd", "whitex")

        elif sensor_topic == "fire":
            if data == 0:
                client.publish("TeamET/Lroom/buzzer/cmd", "on")
                client.publish("TeamET/room1/servo/cmd", "open")
                client.publish("TeamET/room2/servo/cmd", "open")
                client.publish("TeamET/room3/servo/cmd", "open")
                client.publish("TeamET/Lroom/servo/cmd", "open")
                time.sleep(6)
            elif data == 1:
                client.publish("TeamET/Lroom/buzzer/cmd", "off")

        if Lroom_flag[0] != 0.0 and Lroom_flag[1] != 0:
            fields = {'humidity_evt': Lroom_flag[0], 'temperature_evt': Lroom_flag[1]}
            json_body = [
                {
                    "measurement": measurement,
                    "tags": tags,
                    "time": datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ'),
                    "fields": fields
                }
            ]
            client_db.write_points(json_body)
            Lroom_flag[0] = 0.0
            Lroom_flag[1] = 0

    elif sensor_place == "door":
        if data == 1:
            client.publish("TeamET/door/buzzer/cmd", "on")

client_db = InfluxDBClient(server, port, username, password, database)

client = mqtt.Client()
client.connect(server, 1883, 60)
client.on_connect = on_connect
client.on_message = on_message
client.loop_forever()
client_db.close()
