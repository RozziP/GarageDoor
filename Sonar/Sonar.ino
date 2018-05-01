#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// defines pins numbers
#define TRIG_PIN 2  //D4
#define ECHO_PIN 0  //D3
#define OPEN_DISTANCE 50 //cm

long duration;
int  distance = 0;
int  tempDistance;

//User-specific vars
char ssid[] = "";
char pass[] = "";

const char mqttUser[]   = "";
const char mqttPass[]   = "";
const char mqttServer[] = "";
const int  mqttPort     =-1;
const char topic[]      = "Garage";

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
  
  //Begins sound transmission
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Receives the sound transmission and its travel time
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculating the distance
  tempDistance = duration*0.034/2;

  //Only publish if the distance has changed
  if(tempDistance != distance)
  {
    distance = tempDistance;
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
  }
  
  //make sure we stay connected
  if (mqttClient.loop()) 
  {
    connect();
  }
    
  delay(2000);
}

