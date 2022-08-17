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

### Controller
![image](https://user-images.githubusercontent.com/91946874/185166107-f1fa6994-3d10-4034-8128-97ac7b2eee27.png)  

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

## How to recreate this project
### Physical Twin
1. Connect the components according to the circuit diagram above and run the [code](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/tree/main/Physical_Devices_Code/MQTT_EInk_Driver_LED_PIXEL) on NodeMCU through ArduinoIDE
2. Print out the 3D model of different parts one by one and assemble them just like the 3D model file
**Please note:**  
The furnitures can be printed at 10% infill (except the desk, chair, toilet, sinks and stove), so that the top of the physical twin won't be too heavy.  
3. Laser cut the walls using the DXF files in the [DXF folder](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/tree/main/DXF) if you would like to have a clear wall

### Controller
1. Connect the components according to the circuit diagram above and run the [code](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/tree/main/Physical_Devices_Code/buttons) on NodeMCU through ArduinoIDE
2. Print out the 3D model of the cover and base of the controller
3. Laser cut the cover if you would like to see be able to see the circuit
4. Instruction on the controller is printed with Phomemo M02 Pocket Printer, this is optional

### Self Built Air Monitor
1. Connect the Enviro to Raspberry Pi and SGP30 and PMS5003 to Enviro according to the table below
| Enviro  | Other Compoenents |
| ------------- | ------------- |
| 3V3  | SGP30 VIN  |
| GND  | SGP30 GND  |
| SCL  | SGP30 SCL  |
| SDA  | SGP30 SDA  |
| PM  | PMS5003  |
2. Create a .env file to store the MQTT credentials follwing this [tutorial](https://www.geeksforgeeks.org/how-to-hide-sensitive-credentials-using-python/)
3. run the [code](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Physical_Devices_Code/SelfBuiltAirMonitor.py) on Raspberry Pi


### Home Assistant
[Setps to recreate Home Assistant](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Home_Assistant/RecreateHASS.md)
