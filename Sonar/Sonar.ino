#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// defines pins numbers
#define TRIG_PIN 2  //D4
#define ECHO_PIN 0  //D3
#define OPEN_DISTANCE 30 //cm

long duration;
int  distance;

//User-specific vars
char ssid[] = "";
char pass[] = "";

const char mqttUser[]   = "";
const char mqttPass[]   = "";
const char mqttServer[] = "";
const int  mqttPort     = -1;
const char topic[]      = "garage"

WiFiClient netClient;
PubSubClient mqttClient;

void setup() 
{
//Init sonar
pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an Output
pinMode(ECHO_PIN, INPUT); // Sets the ECHO_PIN as an Input

//Init WiFi
Serial.begin(115200);
WiFi.begin(ssid,pass);

//Init MQTT
mqttClient.setClient(netClient);
mqttClient.setServer(mqttServer, mqttPort);
connect();
}

void connect() 
{
  //double check wifi
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi connected");
  
  //connect to mqtt broker
  while(!mqttClient.connected())
  {
  mqttClient.connect("Sensor", mqttUser, mqttPass);
  Serial.print(".");
  delay(500);
  }
  Serial.println("\nMQTT connected");
  mqttClient.subscribe(topic);
}

void loop() 
{
// Clears the TRIG_PIN
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);

// Sets the TRIG_PIN on HIGH state for 10 micro seconds
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

// Reads the ECHO_PIN, returns the sound wave travel time in microseconds
duration = pulseIn(ECHO_PIN, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Publish that shit
if(distance > OPEN_DISTANCE)
{
  mqttClient.publish(topic,"c");
}
else
{
  mqttClient.publish(topic,"o");
}
Serial.print(distance);
delay(2000);
}

