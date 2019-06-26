#include "secrets.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>

//Analog inputs
int sensor_pin = A0;

//Digital outputs
int solenoid_1 = D3;
int water_pump = D4;
int light_sensor_vcc = D5;
int moisture_sensor_vcc = D6;
int moisture_sensor2_vcc = D7;

//WIFI
const char ssid[] = WIFI_SSID;        // SSID of wifi network
const char password[] = WIFI_PASSWORD;   // Password of wifi netowrk
WiFiClient espClient;

//MQTT
const char mqtt_server[] = MQTT_SERVER;                   // MQTT server ip
const int mqtt_port = 1883;                      // MQTT server port   default: 1883
const char mqtt_user[] = MQTT_USER;                     // MQTT username
const char mqtt_password[] = MQTT_PASSWORD;                 // MQTT user password
const char device_id[] = DEVICE_ID;                     // MQTT device name
PubSubClient client(espClient);
char msg[64];
char topic[32];

//Light sensor
unsigned long previousMillis = 0;        // will store last time light sensor was updated
const long interval = 300000;  

void setup() {
  // Initiate the pins
  pinMode(moisture_sensor_vcc, OUTPUT);
  digitalWrite(moisture_sensor_vcc, LOW);
  pinMode(moisture_sensor2_vcc, OUTPUT);
  digitalWrite(moisture_sensor2_vcc, LOW);
  pinMode(light_sensor_vcc, OUTPUT);
  digitalWrite(light_sensor_vcc, LOW);
  pinMode(solenoid_1, OUTPUT);
  digitalWrite(solenoid_1, LOW);
  pinMode(water_pump, OUTPUT);
  digitalWrite(water_pump, LOW);

  //Initiate WIFI
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.hostname("Garden Irrigation");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Initiate MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password )) 
    {
      Serial.println("connected");
      client.subscribe("home/garden_irrigation");  
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  memcpy(msg, payload, length);
  msg[length] = '\0';
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  if(strcmp(msg,"check_moisture") == 0) 
  {
    Serial.println("Reading moisture sensor 1");
    int sensor_1 = read_moisture_sensor1();
    snprintf(topic, 32, "home/garden_moisture_sensor_1");
    snprintf(msg, 64, sensor_1, device_id);
    client.publish(topic,msg); 
    delay(1000);
    Serial.println("Reading moisture sensor 2");
    int sensor_2 = read_moisture_sensor2(); 
    snprintf(topic, 32, "home/garden_moisture_sensor_2");
    snprintf(msg, 64, sensor_2, device_id);
    client.publish(topic,msg); 
    
  }
  else if(strcmp(msg,"water_zone_1") == 0)
  {
    Serial.println("Opening Solenoid");
    open_solenoid();
    delay(500);
    Serial.println("Starting waterpump");
    snprintf(topic, 32, "home/garden_water_zone_1");
    snprintf(msg, 64, "Watering", device_id);
    client.publish(topic,msg);
    start_water_pump();
    delay(30000); //Water for 30seconds
    Serial.println("Stopping waterpump");
    stop_water_pump();
    Serial.println("Closing solenoid");
    close_solenoid();
    snprintf(topic, 32, "home/garden_water_zone_1");
    snprintf(msg, 64, "Done", device_id);
    client.publish(topic,msg); 
  }

  
 
}

int read_moisture_sensor() {
  digitalWrite(moisture_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(moisture_sensor_vcc, LOW);
  return 1023 - value;
}

int read_moisture_sensor2() {
  digitalWrite(moisture_sensor2_vcc, HIGH);
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(moisture_sensor2_vcc, LOW);
  return 1023 - value;
}

int read_light_sensor() {
  digitalWrite(light_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(light_sensor_vcc, LOW);
  return value;
}

void open_solenoid() {
  digitalWrite(solenoid_1, HIGH);
}

void close_solenoid() {
  digitalWrite(solenoid_1, LOW);
}

void start_water_pump() {
  digitalWrite(water_pump, HIGH);
}

void stop_water_pump() {
  digitalWrite(water_pump, LOW);
}

void loop() {
  client.loop();

  //Only update light sensor if the interval has passed.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated sensor
    previousMillis = currentMillis;
    Serial.println("Reading light sensor");
    int light = read_light_sensor(); 
    snprintf(topic, 32, "home/garden_light_sensor");
    snprintf(msg, 64, light, device_id);
    client.publish(topic,msg); 
  }   
}
