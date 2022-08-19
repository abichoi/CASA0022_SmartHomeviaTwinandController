import argparse
import colorsys
import math
import json
import requests
import os

import sys

import ST7735
import time
import ssl
from bme280 import BME280
from pms5003 import PMS5003, ReadTimeoutError, SerialTimeoutError
from enviroplus import gas
from sgp30 import SGP30

sgp30 = SGP30()
result = sgp30.command('set_baseline', (0xFECA, 0xBEBA))
result = sgp30.command('get_baseline')
print(["{:02x}".format(n) for n in result])

print("Sensor warming up, please wait...")
def crude_progress_bar():
    sys.stdout.write('.')
    sys.stdout.flush()

sgp30.start_measurement(crude_progress_bar)
sys.stdout.write('\n')

try:
    # Transitional fix for breaking change in LTR559
    from ltr559 import LTR559

    ltr559 = LTR559()
except ImportError:
    import ltr559

from subprocess import PIPE, Popen, check_output
from PIL import Image, ImageDraw, ImageFont
from fonts.ttf import RobotoMedium as UserFont
import json

import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus

from decouple import config
MQTT_BROKER = config('MQTT_BROKER')
MQTT_BROKER_PORT = int(config('MQTT_BROKER_PORT'))
MQTT_TOPIC = config('MQTT_TOPIC')
READ_INTERVAL = 60
USERNAME = config('USERNAME')
PASSWORD = config('PASSWORD')


# mqtt callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK")
    else:
        print("Bad connection Returned code=", rc)


def on_publish(client, userdata, mid):
    print("mid: " + str(mid))


# Read values from BME280 and return as dict
def read_bme280(bme280):
    # Compensation factor for temperature
    comp_factor = 2.25
    values = {}
    raw_temp = bme280.get_temperature()  # float
    values["temperature"] = int(raw_temp)
    values["pressure"] = round(
        int(bme280.get_pressure() * 100), -1
    )  # round to nearest 10
    values["humidity"] = int(bme280.get_humidity())
    raw_humidity = int(bme280.get_humidity())
    data = gas.read_all()
    values["oxidised"] = int(data.oxidising / 1000)
    values["reduced"] = int(data.reducing / 1000)
    values["nh3"] = int(data.nh3 / 1000)
    values["lux"] = int(ltr559.get_lux())
    return values


# Read values PMS5003 and return as dict
def read_pms5003(pms5003):
    values = {}
    try:
        pm_values = pms5003.read()  # int
        values["pm1"] = pm_values.pm_ug_per_m3(1)
        values["pm25"] = pm_values.pm_ug_per_m3(2.5)
        values["pm10"] = pm_values.pm_ug_per_m3(10)
    except ReadTimeoutError:
        pms5003.reset()
        pm_values = pms5003.read()
        values["pm1"] = pm_values.pm_ug_per_m3(1)
        values["pm25"] = pm_values.pm_ug_per_m3(2.5)
        values["pm10"] = pm_values.pm_ug_per_m3(10)
    return values

def read_sgp30():
    values = {}
    result = sgp30.get_air_quality()
    print(result)
    eco2, tvoc = sgp30.command('measure_air_quality')
    print(eco2, tvoc)
    values["CO2"] = eco2
    values["VOC"] = tvoc
    return values


# Get CPU temperature to use for compensation
def get_cpu_temperature():
    process = Popen(
        ["vcgencmd", "measure_temp"], stdout=PIPE, universal_newlines=True
    )
    output, _error = process.communicate()
    return float(output[output.index("=") + 1:output.rindex("'")])


# Get Raspberry Pi serial number to use as ID
def get_serial_number():
    with open("/proc/cpuinfo", "r") as f:
        for line in f:
            if line[0:6] == "Serial":
                return line.split(":")[1].strip()


# Check for Wi-Fi connection
def check_wifi():
    if check_output(["hostname", "-I"]):
        return True
    else:
        return False


# Display Raspberry Pi serial and Wi-Fi status on LCD
def display_status(disp, mqtt_broker):
    # Width and height to calculate text position
    WIDTH = disp.width
    HEIGHT = disp.height
    # Text settings
    font_size = 12
    font = ImageFont.truetype(UserFont, font_size)
    cpu_temp = get_cpu_temperature()
    wifi_status = "connected" if check_wifi() else "disconnected"
    text_colour = (255, 255, 255)
    back_colour = (0, 170, 170) if check_wifi() else (85, 15, 15)
    device_serial_number = get_serial_number()
    message = "{}\nWi-Fi: {}\nmqtt: {}\ncpu-temp: {}".format(
        device_serial_number, wifi_status, mqtt_broker, cpu_temp
    )
    img = Image.new("RGB", (WIDTH, HEIGHT), color=(0, 0, 0))
    draw = ImageDraw.Draw(img)
    size_x, size_y = draw.textsize(message, font)
    x = (WIDTH - size_x) / 2
    y = (HEIGHT / 2) - (size_y / 2)
    draw.rectangle((0, 0, 160, 80), back_colour)
    draw.text((x, y), message, font=font, fill=text_colour)
    disp.display(img)


def main():
    # Raspberry Pi ID
    device_serial_number = get_serial_number()
    device_id = "raspi-" + device_serial_number

    mqtt_client = mqtt.Client(client_id=device_id)
    if USERNAME and PASSWORD:
        mqtt_client.username_pw_set(USERNAME, PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_publish = on_publish

    if USERNAME is not None:
        mqtt_client.username_pw_set(USERNAME, password=PASSWORD)

    mqtt_client.connect(MQTT_BROKER, port=MQTT_BROKER_PORT)

    bus = SMBus(1)

    # Create BME280 instance
    bme280 = BME280(i2c_dev=bus)

    # Create LCD instance
    disp = ST7735.ST7735(
        port=0, cs=1, dc=9, backlight=12, rotation=270, spi_speed_hz=10000000
    )

    # Initialize display
    disp.begin()

    # Try to create PMS5003 instance
    HAS_PMS = False
    try:
        pms5003 = PMS5003()
        _ = pms5003.read()
        HAS_PMS = True
        print("PMS5003 sensor is connected")
    except SerialTimeoutError:
        print("No PMS5003 sensor connected")

    # Display Raspberry Pi serial and Wi-Fi status
    print("RPi serial: {}".format(device_serial_number))
    print("Wi-Fi: {}\n".format("connected" if check_wifi() else "disconnected"))
    print("MQTT broker IP: {}".format(MQTT_BROKER))

    # Main loop to read data, display, and send over mqtt
    mqtt_client.loop_start()
    while True:
        try:
            values = read_bme280(bme280)
            sgp_values =  read_sgp30()
            values.update(sgp_values)
            pms_values = read_pms5003(pms5003)
            values.update(pms_values)
            print(values)
            mqtt_client.publish(MQTT_TOPIC, json.dumps(values))
            display_status(disp, MQTT_BROKER)
            time.sleep(READ_INTERVAL)
        except Exception as e:
            print(e)


if __name__ == "__main__":
    main()
