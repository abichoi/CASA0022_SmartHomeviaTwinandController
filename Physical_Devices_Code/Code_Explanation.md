# Code Explanation
## MQTT_EInk_Driver_LED_PIXEL
### Device: Physical Twin
#### Detail explanation:
##### _setup()_
The NodeMCU would initialise the NeoPixel, servo motors and the e-ink display, connect to WiFi by running the startWiFi() function, connect to the MQTT and set callback() as the call back of the MQTT.
##### _startWifi()_
This function attempts to connect to the WiFi with the two WiFi credentials provided in _secret.h_, where all the WiFi and MQTT credentials and the topics are stored. It attempts to connect to the WiFi with the credentials named _SECRET_SSID1_ and _SECRET_PASS1_ in secret.h for 6 seconds. If it is not able to connect to the WiFi, then it would use the credentials named _SECRET_SSID2_ and _SECRET_PASS2_ for 10 seconds. The function will be looped until it is connected to WiFi. During the making of this project, the model was moved between the Lab and the studio room. To avoid having to change the WiFi credentials and upload the code every time the model is moved, two sets of credentials are stored in the secret.h and this function is changed so that it can connect to two different WiFi.
##### _pulseWidth()_
This function is used to convert the angle motor is turning to pulse width. 
##### _loop()_
At the beginning of the loop, if the LED and NeoPixel are on but not caused by the MQTT values, they will be turned off. This is written as they are always on when the physical twin is powered on. If the MQTT is disconnected, _reconnect()_ is called and this function connects back to the WiFi and MQTT. 
##### _callback()_
This function prints out the received message’s topic and its size. Then, it will read the message character by character and store it into a variable named combine. After that, the combine is terminated adding a NULL at the end of the variable. At last, the combine is stored in a variable according to the topic if the values are the humidity, temperature and TVOC of the room and the location of the user. For other values such as the status of the doors, window, light blub and light strip, the values are used to control the motors, LED, and NeoPixel after identifying the topic of the message. For example, if the value of the door contact sensor is true, the motor will close the door in the twin. If the value of the light strip colour is changed, then NeoPixel will change to that colour as well. 
##### _refreshdisplay()_
This function refreshes the e-ink display so that it shows the humidity, temperature, TVOC and the location of the user every time one of these values changes.

## buttons
### Device: Controller
#### Detail explanation:
##### _setup()_
The NodeMCU would connect to WiFi by running the startWiFi() function, which is identical to the one in MQTT_EInk_Driver_LED_PIXEL, and connect to the MQTT. 
##### _sendMQTT()_
This function is used to publish messages into MQTT.
##### _loop()_
First, reconnect(), the same as the one in MQTT_EInk_Driver_LED_PIXEL, is called to connect back to the WiFi or MQTT if MQTT is disconnected. Then, the status of the push buttons is read. If the desk lamp or the light strip buttons are pressed for more than 1 second, then it would be counted as long pressed, and it will read in the value of the corresponding potentiometer and send it to MQTT until the button is released. Otherwise, the corresponding light will be toggled through MQTT topics. If cold white, warm white or red buttons are pressed, then one of their MQTT topics will receive a 1 and the rest will receive a 0. The changes in the MQTT topics will trigger the automation mentioned in MQTT_EInk_Driver_LED_PIXEL.

## SelfBuiltAirMonitor.py
### Device: Self Built Air Monitor
This code is a combination of the following two python scripts in Pimoroni's Github repository:
1. https://github.com/pimoroni/enviroplus-python/blob/master/examples/mqtt-all.py
2. https://github.com/pimoroni/sgp30-python/blob/ef99b705293d5f92c2b6487d460ab42e4a728440/examples/test.py
#### Detail explanation:
It first get the MQTT credentials that are stored as environment variables and attemps to connect with the MQTT. Then it reads the values from BME280, PMS5003 and SGP30. After that, it publishes the values to the MQTT. At last, it displays the Raspberry Pi seial, WiFi status, connected MQTT name and the CPU temperature of the Raspberry Pi on the LCD display on the Enviro. 
#### Tutorial
A guide on how to store credetials as environment variables for python files: https://www.geeksforgeeks.org/how-to-hide-sensitive-credentials-using-python/
