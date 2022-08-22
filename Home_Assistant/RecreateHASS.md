# Steps to Recreate HASS in this dissertation
1. Pair your WiFi devices to your WiFi and connect Sonoff Zigbee Dongle
2. HASS installation: https://www.home-assistant.io/installation/raspberrypi
3. Follow the steps on the WebUI
4. HASS should automatically search for devices. If not, install the corresponding intergration manually.
5. Install the intergrations and add-on listed below:
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
 
    [A Tutorial on how to install InfluxDB and Grafana](https://www.youtube.com/watch?v=rXF-LycbjoA) by [Smart Home Junkie on YouTube](https://www.youtube.com/c/SmartHomeJunkie)  
    **Please note:**  
    When pairing devices with Zigbee2MQTT (Z2M), it is possible that you would need to pair and disconnect them a few times to get all the entities.   
    For Aqara TVOC monitor, when pairing it with Z2M, press the button every second until it is paired to get all the entities (TVOC, Humidity, Temeperature), it is possible that you would need to repeat pairing it a lot of times until it works. 
   
7. To add the values of the device status to HASS, edit the configuration.yaml in your HASS with the File editor add-on. The [configuration.yaml](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Home_Assistant/configuration.yaml) file used in this dissertation is provided in this Home_Assistant folder.  
   **Please note:**  
   Please make sure the name and unique_id are correct. If you change it afterwards, HASS will not store the previous data in the new name.
8. After ensuring all devices are connected to HASS, add automations by editing automations.yaml or through the "Automations & Scenes" tab in the Settings. The [automations.yaml file](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Home_Assistant/automations.yaml) used in this dissertation is provided in this Home_Assistant folder and the explanation of automations is in [automation_explain.pdf](https://github.com/abichoi/CASA0022_SmartHomeviaTwinandController/blob/main/Home_Assistant/automation_explain.pdf). 
   **Please note:**  
   If the automations with time triggers/conditions are not working at that specific time, it is possible that you need to set up your time zone in Settings > System > General.
