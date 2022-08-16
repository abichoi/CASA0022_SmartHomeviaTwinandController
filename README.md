# Can the concept of a smart home be used in a small living space and linked to a physical model with two-way communication for real-time IoT data?
This repository is a part of a dissertation for UCL MSc Connected Environments.

## Hypothesis: Deploying a smart home system in a small living space via a physical twin and controller communication model.

## Aims
The aims of this dissertation is to investigate the possibility of deploying a smart home in a small living space and utilise the data collected by the smart home system through a physical ‘twin’ model. 

## Methodology 
A smart home based on Home Assistant is deployed in a student accomadation studio room with the following layout.
![image](https://user-images.githubusercontent.com/91946874/184563082-746f74e4-83db-4650-84e4-a6540e78fa75.png)
In addition, a physical twin and a controller are 3D printed and laser cut.

### Circuits  
#### Physical Twin  
![image](https://user-images.githubusercontent.com/91946874/184911723-add674e3-bae1-4c86-a6cd-7701b7fa4579.png)  
  
#### Controller  
![image](https://user-images.githubusercontent.com/91946874/184911785-45d2c517-2914-4e67-bddc-eddeeab01069.png)  

### Code
[Code Explanation](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Physical_Devices_Code/Code_Explanation.md)

## Results
It is possible to deploy a smart home in a small living space and utilise the data collected by the smart home outside the system via a physical twin and controller. However, it is noted that the potential of the smart home is limited as it is easy for sensors' detection areas to overlay and it is difficult to differentiate different activitis in the same area.

## Resulted Devices
### Physical Twin with 3D Printed Walls
![image](https://user-images.githubusercontent.com/91946874/184908438-35610964-ef20-4f38-941f-fb350af042e4.png)

### Physical Twin with Laser Cut Walls
![image](https://user-images.githubusercontent.com/91946874/184907338-3e9f2732-d167-4e4e-9327-4e68d02420f8.png)

### Self Built Air Monitor
![image](https://user-images.githubusercontent.com/91946874/184907957-27bd574c-8a06-423a-ab1b-dc6a0b1ad9c2.png)

## Used Softwares and Hardwares
### General
1. Modeling Software: Autodesk Fusion 360
2. 3D Printer: Original Prusa i3 MK3S+ 3D printer
### Physical Twin
1. Laser Cutter: Piranha Bodor laser cutter/ Trotec laser cutter
2. Laser Cutter Software: RD Works version 7/ Rhino
3. Servo Driver PCA9685
4. NodeMCU
5. Micro USB to USB-A or USB-C cable
6. Servo Motor x3
7.  2.9" E-Ink Display Module
8. LED
9. 220Ω Resistor
10. NeoPixel Stick
11. Power Bank: Anker PowerCore 20000 PD (Optional)
#### Files
Version 1: PhysicalTwin_3DPrintWall_Github v3.f3d  
Version 2: PhysicalTwin_LaserCutWall_Github v8.f3d, the whole DXF folder


### Controller
1. Laser Cutter: Piranha Bodor laser cutter/ Trotec laser cutter
2. Laser Cutter Software: RD Works version 7/ Rhino
3. NodeMCU
4. Micro USB to USB-A or USB-C cable
5. Push Button x 5
6. 10kΩ Resistor x 5
7. 4.7k Resistor
8. Potentiometer x2
9. Diode x2
10. Phomemo M02 Pocket Printer (Optional)
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
1. Smart Home Platform: Home Assistant OS
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
