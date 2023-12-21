import streamlit as st
from datetime import datetime
import time as t
from influxdb import InfluxDBClient
from datetime import datetime
from collections import deque
import matplotlib.pyplot as plt

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

def update_Lroom_temp():
    query = 'SELECT * FROM "{}" WHERE location=\'Lroom\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        temp = point['temperature_evt']
        Lroom_temp.append(temp)
        print("Lroom temp :",temp)
        
        if len(Lroom_temp) > 10:
            Lroom_temp.popleft()

def update_Lroom_humid():
    query = 'SELECT * FROM "{}" WHERE location=\'Lroom\' ORDER BY time DESC LIMIT 1'.format(measurement)
    result = client_db.query(query)

    for point in result.get_points():
        humid = point['humidity_evt']
        Lroom_humid.append(humid)
        print("Lroom humid :",humid)

        if len(Lroom_humid) > 10:
            Lroom_humid.popleft()

def show_current_time():
    current_time = datetime.now().strftime("%H:%M:%S")
    return f"Recent Update : {current_time}"

room1_temp = deque([0], maxlen=10)
room1_humid = deque([0], maxlen=10)

room2_temp = deque([0], maxlen=10)
room2_humid = deque([0], maxlen=10)

room3_temp = deque([0], maxlen=10)
room3_humid = deque([0], maxlen=10)

Lroom_temp = deque([0], maxlen=10)
Lroom_humid = deque([0], maxlen=10)

 
st.write('# Happy Home')

############## Room 1 ##############
st.write('## 용주네 방')

room1_1, room1_2 = st.columns(2)

with room1_1:
    st.write('### Temperature')
    room1_temp_chart = st.line_chart(list(room1_temp))
with room1_2:
    st.write('### Humidity')
    room1_humid_chart = st.line_chart(list(room1_humid))
####################################
############## Room 2 ##############
st.write('## 승진이네 방')

room2_1, room2_2 = st.columns(2)
with room2_1:
    st.write('### Temperature')
    room2_temp_chart = st.line_chart(list(room2_temp))
with room2_2:
    st.write('### Humidity')
    room2_humid_chart = st.line_chart(list(room2_humid))
####################################
############## Room 3 ##############
st.write('## 윤수네 방')

room3_1, room3_2 = st.columns(2)
with room3_1:
    st.write('### Temperature')
    room3_temp_chart = st.line_chart(list(room3_temp))
with room3_2:
    st.write('### Humidity')
    room3_humid_chart = st.line_chart(list(room3_humid))
####################################
############## Lroom ##############
st.write('## Lroom')

Lroom_1, Lroom_2 = st.columns(2)
with Lroom_1:
    st.write('### Temperature')
    Lroom_temp_chart = st.line_chart(list(Lroom_temp))
with Lroom_2:
    st.write('### Humidity')
    Lroom_humid_chart = st.line_chart(list(Lroom_humid))
####################################
time_placeholder = st.sidebar.empty()

while True:
    time_placeholder.markdown(show_current_time(), unsafe_allow_html=True)

    update_room1_humid() 
    update_room1_temp()

    update_room2_humid() 
    update_room2_temp()

    update_room3_humid() 
    update_room3_temp()

    update_Lroom_humid()  
    update_Lroom_temp()

    room1_humid_chart.line_chart(list(room1_humid))
    room1_temp_chart.line_chart(list(room1_temp))

    room2_humid_chart.line_chart(list(room2_humid))
    room2_temp_chart.line_chart(list(room2_temp))

    room3_humid_chart.line_chart(list(room3_humid))
    room3_temp_chart.line_chart(list(room3_temp))

    Lroom_humid_chart.line_chart(list(Lroom_humid))
    Lroom_temp_chart.line_chart(list(Lroom_temp))

    t.sleep(1)
