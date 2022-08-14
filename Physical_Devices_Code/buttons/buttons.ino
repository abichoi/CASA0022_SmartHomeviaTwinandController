#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ezTime.h>
#include "Wire.h"
#include "secret.h"
const char* ssid1 = SECRET_SSID1;
const char* password1 = SECRET_PASS1;
const char* ssid2 = SECRET_SSID2;
const char* password2 = SECRET_PASS2;
const char* mqttserver = SECRET_MQTTURL;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;
int mqttport = 1884;

Timezone GB;


const char topic_lampbutton[] = TOPIC_LAMPBUTTON;
const char topic_stripbutton[] = TOPIC_STRIPBUTTON;
const char topic_lampbrightness[] = TOPIC_LAMPBRIGHTNESS;
const char topic_stripbrightness[] = TOPIC_STRIPBRIGHTNESS;
const char topic_cold[] = TOPIC_COLD;
const char topic_warm[] = TOPIC_WARM;
const char topic_red[] = TOPIC_RED;


float readlamp_brightness;
float readstrip_brightness;
const int lamp_brightness = 5; //D1
const int strip_brightness = 4; //D2
const int lamp_button = 15; //D8
const int strip_button = 2; //D4
const int strip_cold = 14; //D5
const int strip_warm = 12; //D6
const int strip_red = 13; //D7
int short_press_time = 500;
int long_press_time  = 1000;

bool lamp_button_toogle = false;
bool strip_button_toogle = false;

int lastState_lamp_button = LOW;
int lastState_strip_button = LOW;
int lastState_strip_cold = LOW;
int lastState_strip_warm = LOW;
int lastState_strip_red = LOW;


bool isPressing_lamp_button = false;
unsigned long pressedTime_lamp_button = 0;
bool isLongDetected_lamp_button = false;

bool isPressing_strip_button = false;
unsigned long pressedTime_strip_button = 0;
bool isLongDetected_strip_button = false;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
 
void setup() {
  Serial.begin(115200);
  startWifi();
  
  // get real date and time
  waitForSync();

  Serial.println("UTC: " + UTC.dateTime()); 
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());  
  //set up MQTT
  client.setServer(mqttserver, mqttport);  

  pinMode(lamp_brightness, OUTPUT);
  pinMode(strip_brightness, OUTPUT);
}

//set up wifi
void startWifi(){
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.begin(ssid1, password1);
  int currentwifi = 1;
  unsigned long previousMillis = millis();
  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    unsigned long currentMillis = millis();
     if (currentMillis - previousMillis >= 6000) {
      previousMillis = currentMillis;
      if (currentwifi == 1){
        Serial.print("Connecting to ");
        Serial.println(ssid2);
        WiFi.begin(ssid2, password2);
        currentwifi = 2;
      } else if (currentwifi == 2){
        Serial.print("Connecting to ");
        Serial.println(ssid1);
        WiFi.begin(ssid1, password1);
        currentwifi = 1;
      }
    }
    
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendMQTT(const char* topic, uint8_t msg) {
  Serial.print("message: ");
  Serial.println(msg);
  char payload[16];
  snprintf(payload, sizeof(payload), "%u", msg);
  client.publish(topic, payload);
}

void reconnect() {
  startWifi();
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
    }
  client.loop();
  int currentState_strip_cold = digitalRead(strip_cold);
  int currentState_strip_warm = digitalRead(strip_warm);
  int currentState_strip_red = digitalRead(strip_red);
  int currentState_lamp_button = digitalRead(lamp_button);
  int currentState_strip_button = digitalRead(strip_button);
  
  if(lastState_lamp_button == LOW && currentState_lamp_button == HIGH) {        // button is pressed
    pressedTime_lamp_button = millis();
    isPressing_lamp_button = true;
    isLongDetected_lamp_button = false;
  } else if(lastState_lamp_button == HIGH && currentState_lamp_button == LOW) { // button is released
    if (isLongDetected_lamp_button){
    sendMQTT(topic_lampbrightness, readlamp_brightness);
    }
    else {
      if (lamp_button_toogle){
        lamp_button_toogle = false;
      } else {
        lamp_button_toogle = true;
        }
      sendMQTT(topic_lampbutton, lamp_button_toogle);
    }
    isLongDetected_lamp_button = false;
    isPressing_lamp_button = false;
  }

  if(isPressing_lamp_button == true) {
    long pressDuration_lamp_button = millis() - pressedTime_lamp_button;
    if( pressDuration_lamp_button > long_press_time ) {
      Serial.println("A long press is detected");
      isLongDetected_lamp_button = true;
      digitalWrite(lamp_brightness, HIGH); //  Turn D1 On
      digitalWrite(strip_brightness, LOW); // Turn D2 Off
      readlamp_brightness = analogRead(0);
      readlamp_brightness = round(readlamp_brightness/890 * 255);
      if (readlamp_brightness > 255){
        readlamp_brightness = 255;
        }
      Serial.println("readlamp_brightness:");
      Serial.println(readlamp_brightness);
    }
  }
  
  lastState_lamp_button = currentState_lamp_button;

  if(lastState_strip_button == LOW && currentState_strip_button == HIGH) {        // button is pressed
    pressedTime_strip_button = millis();
    isPressing_strip_button = true;
    isLongDetected_strip_button = false;
  } else if(lastState_strip_button == HIGH && currentState_strip_button == LOW) { // button is released
    if (isLongDetected_strip_button){
    sendMQTT(topic_stripbrightness, readstrip_brightness);
    }
    else {
      if (strip_button_toogle){
        strip_button_toogle = false;
      } else {
        strip_button_toogle = true;
        }
      sendMQTT(topic_stripbutton, strip_button_toogle);
    }
    isLongDetected_strip_button = false;
    isPressing_strip_button = false;
  }

  if(isPressing_strip_button == true) {
    long pressDuration_strip_button = millis() - pressedTime_strip_button;
  
    if( pressDuration_strip_button > long_press_time ) {
      isLongDetected_strip_button = true;
      Serial.println("A long press is detected");
      digitalWrite(lamp_brightness, LOW); //  Turn D1 On
      digitalWrite(strip_brightness, HIGH); // Turn D2 Off
      readstrip_brightness = analogRead(0);
      Serial.println("readstrip_brightness:");
      Serial.println(readstrip_brightness);
      readstrip_brightness = round(readstrip_brightness/890 * 255);
      if (readstrip_brightness > 255){
        readstrip_brightness = 255;
        }
      Serial.println("readstrip_brightness:");
      Serial.println(readstrip_brightness);
    } 
  }
  
  lastState_strip_button = currentState_strip_button;

 
  if(lastState_strip_cold == LOW && currentState_strip_cold == HIGH){
    sendMQTT(topic_cold, 1);
    sendMQTT(topic_warm, 0);
    sendMQTT(topic_red, 0);
  } else if(lastState_strip_warm == LOW && currentState_strip_warm == HIGH){
    sendMQTT(topic_cold, 0);
    sendMQTT(topic_warm, 1);
    sendMQTT(topic_red, 0);
  }else if(lastState_strip_red == LOW && currentState_strip_red == HIGH){
    sendMQTT(topic_cold, 0);
    sendMQTT(topic_warm, 0);
    sendMQTT(topic_red, 1);
  } 

  lastState_strip_cold = currentState_strip_cold;
  lastState_strip_warm = currentState_strip_warm;
  lastState_strip_red = currentState_strip_red;

}
