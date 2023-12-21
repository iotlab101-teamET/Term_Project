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

def update_room1_temp():
    query = 'SELECT * FROM "{}" WHERE location=\'room1\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        temp = point['temperature_evt']
        room1_temp.append(temp)
        print("room1 temp :",temp)

        if len(room1_temp) > 10:
            room1_temp.popleft()

def update_room1_humid():
    query = 'SELECT * FROM "{}" WHERE location=\'room1\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        humid = point['humidity_evt']
        room1_humid.append(humid)
        print("room1 humid :",humid)

        if len(room1_humid) > 10:
            room1_humid.popleft()

def show_current_time():
    current_time = datetime.now().strftime("%H:%M:%S")
    return f"Current Time: {current_time}"

room1_temp = deque([0], maxlen=10)
room1_humid = deque([0], maxlen=10)

st.sidebar.markdown("# 용주네 방")
st.write('## 용주네 방')

room1_1, room1_2 = st.columns(2)

with room1_1:
    st.write('### Temperature')
    room1_temp_chart = st.line_chart(list(room1_temp))
with room1_2:
    st.write('### Humidity')
    room1_humid_chart = st.line_chart(list(room1_humid))
    
while True:
    update_room1_humid() 
    update_room1_temp()

    room1_humid_chart.line_chart(list(room1_humid))
    room1_temp_chart.line_chart(list(room1_temp))

    t.sleep(1)
