#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 4 //D2

CRGB leds[NUM_LEDS];

//User-specific vars
char ssid[] = "";
char pass[] = "";

const char mqttUser[]   = "";
const char mqttPass[]   = "";
const char mqttServer[] = "";
const int  mqttPort     = -1;
const char topic[]      = "Garage";

int timer = 0;

WiFiClient netClient;
PubSubClient mqttClient;

void setup() 
{
  //Init LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for(int i = 0; i < NUM_LEDS;i++)
  {
    leds[i] = CRGB::Blue;
  }
  FastLED.show(); 
  
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

  //connect to mqtt broker and subscribe to topic
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
  timer = 0;
  payload[length] = '\0'; //null-terminate the received msg
  /*
  payload is received as a byte*, 
  which needs to be cast to char* and dereferenced in order to compare it
  */
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
  FastLED.setBrightness(40);
  FastLED.show(); 
}

void loop() 
{
  if (!mqttClient.loop())
  {
    //turn half of the ring orange if the LED disconnects from MQTT
    for(int i = (NUM_LEDS/2); i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Orange;
    }
    FastLED.show(); 
    connect();
  }

  //Turn half of the ring blue if the sensor has not responded in ~12 seconds
  timer++;
  if(timer >= 12)
  {
     for(int i = (NUM_LEDS/2); i < NUM_LEDS; i++)
     {
       leds[i] = CRGB::Blue;
     }
     FastLED.show(); 
  }
  
  delay(1000);
}
