# Can the concept of a smart home be used in a small living space and linked to a physical model with two-way communication for real-time IoT data?
This repository is a dissertation for UCL MSc Connected Environments.

## Hypothesis: Deploying a smart home system in a small living space via a physical twin and controller communication model.

## Aims
The aims of this dissertation is to investigate the possibility of deploying a smart home in a small living space and utilise the data collected by the smart home system through a physical ‘twin’ model. 

## Methodology 
A smart home based on Home Assistant is deployed in a student accomadation studio room with the following layout.
![image](https://user-images.githubusercontent.com/91946874/184563082-746f74e4-83db-4650-84e4-a6540e78fa75.png)
In addition, a physical twin and a controller are 3D printed and laser cut.

## Results
It is possible to deploy a smart home in a small living space and utilise the data collected by the smart home outside the system via a physical twin and controller. However, it is noted that the potential of the smart home is limited as it is easy for sensors' detection areas to overlay and it is difficult to differentiate different activitis in the same area.

## Used Softwares and Hardwares
### Physical Twin
1. Modeling Software: Autodesk Fusion 360
2. 3D Printer: Original Prusa i3 MK3S+ 3D printer
3. Laser Cutter: Piranha Bodor laser cutter/ Trotec laser cutter
4. Laser Cutter Software: RD Works version 7/ Rhino
5. Servo Driver PCA9685
6. NodeMCU
7. Micro USB to USB-A or USB-C cable
8. Servo Motor x3
9.  2.9" E-Ink Display Module
10. LED
11. 220Ω Resistor
12. NeoPixel Stick
13. Power Bank: Anker PowerCore 20000 PD (Optional)
#### Files
Version 1: DigitalTwin_3DPrintWall_Github v3.f3d
Version 2: DigitalTwin_LaserCutWall_Github v8.f3d, the whole DXF folder


### Controller
1. Modeling Software: Autodesk Fusion 360
2. 3D Printer: Original Prusa i3 MK3S+ 3D printer
3. Laser Cutter: Piranha Bodor laser cutter/ Trotec laser cutter
4. Laser Cutter Software: RD Works version 7/ Rhino
6. NodeMCU
7. Micro USB to USB-A or USB-C cable
8. Push Button x 5
9. 10kΩ Resistor x 5
10. 4.7k Resistor
11. Potentiometer x2
12. Diode x2
#### File
Buttons_GIthub v11.f3d

### Self-Built Air Monitor
1. Modeling Software: Autodesk Fusion 360
2. 3D Printer: Original Prusa i3 MK3S+ 3D printer
3. Pimoroni Enviro for Raspberry Pi
4. PMS5003 Particulate Matter Sensor
5. SGP30
6. Raspberry Pi 4B
#### File
SelfBuiltAirMonitor_Case_Github v3.f3d

### Smart Home
1. Smart Home Platform: Home Assistant
2. Raspberry Pi 4B
3. SONOFF Zigbee 3.0 USB Dongle Plus
4. Aqara Motion Sensor x3
5. Aqara TVOC Air Quality Monitor
6. Aqara Door and Window Sensor
7. TP-LINK Kasa Smart Bulb LB110
8. TP-Link Kasa Smart Light Strip KL430
#### Intergrations
1. MQTT
2. OpenWeatherMap
3. TP-Link Kasa Smart
#### Add-ons
1. File editor
2. Terminal & SSH
3. Grafana
4. InfluxDB
5. Zigbee2MQTT
