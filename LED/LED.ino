#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 4 //D2

CRGB leds[NUM_LEDS];

//User-specific vars
const char ssid[] = "";
const char pass[] = "";

const char mqttUser[]   = "";
const char mqttPass[]   = "";
const char mqttServer[] = "";
const int  mqttPort     = -1;
const char topic[]      = "garage"

WiFiClient netClient;
PubSubClient mqttClient;

void setup() 
{
  //Init LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for(int i = 0; i < NUM_LEDS;i++)
  {
    leds[i] = CRGB::Blue;
    FastLED.show(); 
  }
  
  //Init WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  //Init MQTT
  mqttClient.setClient(netClient);
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
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
   Serial.println("\nWiFiconnected");

  //connect to mqtt broker
  while(!mqttClient.connected())
  {
  mqttClient.connect("Light", mqttUser, mqttPass);
  Serial.print(".");
  delay(500);
  }
  Serial.println("\nMQTT connected");
  mqttClient.subscribe(topic);
}

//Executes when a message is received
void callback(const char topic[], byte* payload, unsigned int length)
{
  payload[length] = '\0'; //null-terminate the received msg
  /*
  payload is received as a byte*, 
  which needs to be casted to char* and dereferenced in order to compare it
  */
  Serial.println(*(char*)payload); //print for testing purposes
  if(*(char*)payload == 'o')
  {
    for(int i = 0; i < NUM_LEDS;i++)
    {
      leds[i] = CRGB::Red;
    }
  }
  else if(*(char*)payload == 'c')
  {
      for(int i = 0; i < NUM_LEDS;i++)
    {
      leds[i] = CRGB::Green;
    }
  }
  FastLED.show(); 
  FastLED.setBrightness(20);
}

void loop() 
{
  mqttClient.loop();
  if (!mqttClient.connected()) 
  {
    connect();
  }
  delay(1000);
}
