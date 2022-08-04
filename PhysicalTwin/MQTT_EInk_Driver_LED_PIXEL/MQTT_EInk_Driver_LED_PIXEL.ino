#include <ESP8266WiFi.h>
#include <ezTime.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "secret.h"
const char* ssid1 = SECRET_SSID1;
const char* password1 = SECRET_PASS1;
const char* ssid2 = SECRET_SSID2;
const char* password2 = SECRET_PASS2;
const char* mqttserver = SECRET_MQTTURL;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

const int mqttport = 1884;

char* air;
String received_test;
float humi= 0;
float temp = 0;
float voc = 0;
String location = "Waiting for update";
bool maindoor_contact = false;
bool bathdoor_contact = false;
bool window_contact = false;
bool strip_onoff = false;
String strip_rgb;
byte  strip_R = 0;
byte  strip_G = 0;
byte  strip_B = 0;
float strip_brightness;
bool lamp_onoff = false;
int lamp_brightness;

char * ptr;

Timezone GB;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const char topic_test[] = TOPIC_TEST;
const char topic_air[] = TOPIC_AIR;
const char topic_air2[] = TOPIC_ENVIROPLUS;
const char topic_location[] = TOPIC_LOCATION;
const char topic_main_door[] = TOPIC_DOOR1;
const char topic_bath_door[] = TOPIC_DOOR2;
const char topic_window[] = TOPIC_DOOR3;
const char topic_strip_onoff[] = TOPIC_STRIP_ONOFF;
const char topic_strip_rgb[] = TOPIC_STRIP_RGB;
const char topic_strip_brightness[] = TOPIC_STRIP_BRIGHTNESS;
const char topic_lamp_onoff[] = TOPIC_LAMP_ONOFF;
const char topic_lamp_brightness[] = TOPIC_LAMP_BRIGHTNESS;


//GxEPD e-ink display
#include <GxEPD.h>
// display class
#include <GxGDEM029T94/GxGDEM029T94.h>      // 2.9" b/w
// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D0*/ 16); // original does not work;
GxEPD_Class display(io, /*RST=D0*/ 16, /*BUSY=D6*/ 12); // adapted from http://psychomechanics.de/?p=738&page=2

//driver
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
#define FREQUENCY 50

//PIXEL
#include <Adafruit_NeoPixel.h>
#define DIN D4
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS,DIN, NEO_GRB + NEO_KHZ800);

//LED
#define LED D9

void setup() {
  // open serial connection
  Serial.begin(115200);

  //set up NeoPixel
  pixels.begin();
  pixels.clear();
  for (int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i,pixels.Color(0, 0, 0));
  }
  pixels.show();

  //set up servo motor
  Serial.println("setup servo motor");
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pwm.setPWM(0, 0, pulseWidth(90)); //main door
  pwm.setPWM(1, 0, pulseWidth(90)); //bathroom door
  pwm.setPWM(2, 0, pulseWidth(90)); //window
  Serial.println("setup servo motor DONE");
  
  
  delay(100);

  startWifi();

  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime()); 
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());  

  //set up MQTT
  client.setServer(mqttserver, mqttport);
  client.setCallback(callback);

  //set up e ink display
  Serial.println("setup e-ink");
  display.init(115200);
  Serial.println("setup e-ink DONE");
  display.eraseDisplay();
  display.print("Getting Data");
  display.drawPaged(refreshdisplay);
  
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void loop() {
  if(strip_onoff == false){
    pixels.clear();
    for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i,pixels.Color(0, 0, 0));
    }
    pixels.show();
  }
  if(lamp_onoff == false){
    analogWrite(LED, 0);
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 //  sendMQTT();
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

//void sendMQTT() {
//  delay(5000);
//  ++value;
//  snprintf (msg, 50, "hello world #%ld", value);
//  Serial.print("Publish message: ");
//  Serial.println(msg);
//  client.publish(topic_test, msg);
//}

void callback(String topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  char combine[length+1];
  for (int i = 0; i < length; i++) {
    combine[i]= (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  //terminate the message variable
  combine[length] = NULL;
  Serial.print(" bytes:");
  Serial.println(length);
  Serial.println(combine);
  Serial.println(topic);

  if (topic == topic_test){
    received_test = String(combine);
    Serial.print("received_test:");
    Serial.println(received_test);
  } else if (topic == topic_air2){
//    air = combine;
//    Serial.print("air:");
//    Serial.println(air);
    Serial.print("air monitor update");
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);
    humi  = doc["humidity"];
    temp = doc["temperature"];
    voc = doc["VOC"];
    Serial.print("humidity:");
    Serial.println(humi);    
    Serial.print("temperature:");
    Serial.println(temp);
    Serial.print("voc:");
    Serial.println(voc);
    display.drawPaged(refreshdisplay);
  }else if (topic == topic_location){
    location = String(combine);
    Serial.print("Location:");
    Serial.println(location);
    display.drawPaged(refreshdisplay);
  }else if (topic == topic_main_door){
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);
    if (String(doc["contact"]) == "true"){
      maindoor_contact = true; //door closed
      pwm.setPWM(0, 0, pulseWidth(90)); //main door close
    } else{
      maindoor_contact = false; //door opened
      pwm.setPWM(0, 0, pulseWidth(0)); //main door open
      }
    Serial.print("maindoor_contact:");
    Serial.println(maindoor_contact);
  }else if (topic == topic_bath_door){
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);
    if (String(doc["contact"]) == "true"){
      bathdoor_contact = true; //door closed
      pwm.setPWM(1, 0, pulseWidth(90)); //bathroom door close
    } else{
      bathdoor_contact = false; //door opened
      pwm.setPWM(1, 0, pulseWidth(0)); //bathroom door open
    }
    Serial.print("bathdoor_contact:");
    Serial.println(bathdoor_contact);
  }else if (topic == topic_window){
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);
    if (String(doc["contact"]) == "true"){
      window_contact = true; //window closed
      pwm.setPWM(2, 0, pulseWidth(90)); //window door close
    } else{
      window_contact = false; //window opened
      pwm.setPWM(2, 0, pulseWidth(20 )); //window door open
     
    }
    Serial.print("window_contact:");
    Serial.println(window_contact);
  }else if (topic == topic_strip_onoff){
    if (strcmp(combine,"on")==0){
      strip_onoff = true;
    } else{
      strip_onoff = false;
     }
     Serial.print("strip_onoff:");
    Serial.println(strip_onoff);
  }else if (topic == topic_strip_rgb){
    strip_rgb = String(combine);
    int posred = strip_rgb.indexOf('(');
    String red = strip_rgb.substring(posred +1, posred +4);
    int posgrn = strip_rgb.indexOf(','); 
    String green = strip_rgb.substring(posgrn +1, posgrn +5);
    int posblu = strip_rgb.indexOf(')');
    String blue = strip_rgb.substring(posblu -3, posblu);
    strip_R = red.toInt(); 
    strip_G = green.toInt();
    strip_B = blue.toInt();
    Serial.print("strip_R:");
    Serial.println(strip_R);
    Serial.print("strip_G:");
    Serial.println(strip_G);
    Serial.print("strip_B:");
    Serial.println(strip_B);
    
    if (strip_onoff == true){
      for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i,pixels.Color(strip_R*strip_brightness, strip_G*strip_brightness, strip_B*strip_brightness));
      }
      pixels.show();
     } else{
      strip_onoff = false;
      strip_brightness = 0;
      strip_R = 0;
      strip_G = 0;
      strip_B = 0;
      pixels.clear();
      for (int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i,pixels.Color(0, 0, 0));
      }
      pixels.show();
     }
  }else if (topic == topic_strip_brightness){
    strip_brightness = atoi(combine);
    strip_brightness = strip_brightness/100;
    Serial.print("strip_brightness:");
    Serial.println(strip_brightness);
  }else if (topic == topic_lamp_onoff){
    if (strcmp(combine,"on")==0){
      lamp_onoff = true; //lamp is on
      analogWrite(LED, lamp_brightness);
    } else{
      lamp_onoff = false; //lamp is off
      analogWrite(LED, 0);
    }
    Serial.print("lamp_onoff:");
    Serial.println(lamp_onoff);
  }else if (topic == topic_lamp_brightness){
    lamp_brightness = atoi(combine);
    analogWrite(LED, lamp_brightness);
    Serial.print("lamp_brightness:");
    Serial.println(lamp_brightness);
  }
  Serial.print("humidity:");
    Serial.println(humi);    
    Serial.print("temperature:");
    Serial.println(temp);
    Serial.print("voc:");
    Serial.println(voc);
    Serial.print("Location:");
    Serial.println(location);
    Serial.print("maindoor_contact:");
    Serial.println(maindoor_contact);
    Serial.print("bathdoor_contact:");
    Serial.println(bathdoor_contact);
    Serial.print("window_contact:");
    Serial.println(window_contact);
    Serial.print("strip_onoff:");
    Serial.println(strip_onoff);
    Serial.print("strip_R:");
    Serial.println(strip_R);
    Serial.print("strip_G:");
    Serial.println(strip_G);
    Serial.print("strip_B:");
    Serial.println(strip_B);
    Serial.print("strip_brightness:");
    Serial.println(strip_brightness);
    Serial.print("lamp_onoff:");
    Serial.println(lamp_onoff);
    Serial.print("lamp_brightness:");
    Serial.println(lamp_brightness);

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
      client.subscribe(topic_test);
      client.subscribe(topic_air);
      client.subscribe(topic_air2);
      client.subscribe(topic_location);
      client.subscribe(topic_main_door);
      client.subscribe(topic_bath_door);
      client.subscribe(topic_window);
      client.subscribe(topic_strip_onoff);
      client.subscribe(topic_strip_rgb);
      client.subscribe(topic_strip_brightness);
      client.subscribe(topic_lamp_onoff);
      client.subscribe(topic_lamp_brightness);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

void refreshdisplay(){
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextSize(1);
  display.setRotation(1);
  display.setCursor(0, 0);
  display.println();
  display.print("Temperature:");
  display.print(temp);
  display.println("C");
  display.print("Humidity:");
  display.print(humi);
  display.println("%");
  display.print("TVOC:");
  display.print(voc);
  display.println("ppb");
  display.print("Location:");
  display.print(location);
  display.update();
}
