import streamlit as st
from datetime import datetime
import time as t
from influxdb import InfluxDBClient
from datetime import datetime
from collections import deque

server = "54.173.215.51"
port = 8086
database = 'sensorDB'
username = 'ssm'
password = ''

client_db = InfluxDBClient(server, port, username, password, database)
measurement = 'happyHome'

def update_room2_temp():
    query = 'SELECT * FROM "{}" WHERE location=\'room2\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        temp = point['temperature_evt']
        room2_temp.append(temp)
        print("room2 temp :",temp)

        if len(room2_temp) > 10:
            room2_temp.popleft()

def update_room2_humid():
    query = 'SELECT * FROM "{}" WHERE location=\'room2\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        humid = point['humidity_evt']
        room2_humid.append(humid)
        print("room2 humid :",humid)

        if len(room2_humid) > 10:
            room2_humid.popleft()

room2_temp = deque([0], maxlen=10)
room2_humid = deque([0], maxlen=10)

st.sidebar.markdown("# 승진이네 방")

st.write('## 승진이네 방')

room2_1, room2_2 = st.columns(2)
with room2_1:
    st.write('### Temperature')
    room2_temp_chart = st.line_chart(list(room2_temp))
with room2_2:
    st.write('### Humidity')
    room2_humid_chart = st.line_chart(list(room2_humid))

while True:
    update_room2_humid()
    update_room2_temp()

    room2_humid_chart.line_chart(list(room2_humid))
    room2_temp_chart.line_chart(list(room2_temp))

    t.sleep(1)
