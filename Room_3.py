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

def update_room3_temp():
    query = 'SELECT * FROM "{}" WHERE location=\'room3\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        temp = point['temperature_evt']
        room3_temp.append(temp)
        print("room3 temp :",temp)

        if len(room3_temp) > 10:
            room3_temp.popleft()

def update_room3_humid():
    query = 'SELECT * FROM "{}" WHERE location=\'room3\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        humid = point['humidity_evt']
        room3_humid.append(humid)
        print("room3 humid :",humid)

        if len(room3_humid) > 10:
            room3_humid.popleft()

room3_temp = deque([0], maxlen=10)
room3_humid = deque([0], maxlen=10)

st.sidebar.markdown("# 윤수네 방")
st.write('## 윤수네 방')

room3_1, room3_2 = st.columns(2)
with room3_1:
    st.write('### Temperature')
    room3_temp_chart = st.line_chart(list(room3_temp))
with room3_2:
    st.write('### Humidity')
    room3_humid_chart = st.line_chart(list(room3_humid))
    
while True:
    update_room3_humid()
    update_room3_temp()

    room3_humid_chart.line_chart(list(room3_humid))
    room3_temp_chart.line_chart(list(room3_temp))

    t.sleep(1)
