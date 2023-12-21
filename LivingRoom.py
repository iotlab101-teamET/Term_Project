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

client_db = InfluxDBClient(server, port, username, password, dataabase)
measurement = 'happyHome'

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

Lroom_temp = deque([0], maxlen=10)
Lroom_humid = deque([0], maxlen=10)

st.sidebar.markdown("# 거실")
st.write('## 거실')

Lroom_1, Lroom_2 = st.columns(2)
with Lroom_1:
    st.write('### Temperature')
    Lroom_temp_chart = st.line_chart(list(Lroom_temp))
with Lroom_2:
    st.write('### Humidity')
    Lroom_humid_chart = st.line_chart(list(Lroom_humid))
    
while True:
    update_Lroom_humid()  
    update_Lroom_temp()

    Lroom_humid_chart.line_chart(list(Lroom_humid))
    Lroom_temp_chart.line_chart(list(Lroom_temp))

    t.sleep(1)
