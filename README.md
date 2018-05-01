# GarageDoorStatus

### Arduino Dependencies:  
**_These can be included through the arduino IDE_**  
PubSubClient -> https://github.com/knolleary/pubsubclient  
FastLED -> http://fastled.io/  
ESP8266 core -> https://github.com/esp8266/Arduino


### JS Dependencies:  
MQTT.js -> https://github.com/mqttjs/MQTT.js  
Express -> https://expressjs.com/  
Pug -> https://pugjs.org/api/getting-started.html

Web service hosted with Now -> https://zeit.co/now  
MQTT broker -> https://www.cloudmqtt.com/

The garage door has two states: open(red) or closed(green)  

The ultrasonic sensor should be seated on the ceiling of your garage.   
If there is anything solid within a certain distance from the sensor, the door will be considered open.   

An indication message of open(o) or closed(c) will be published to a topic via MQTT.  
The message can be received by any MQTT-capable client.  
I am currently displaying through a web service(for remote access) and and LED indicator(for on-site use)

Project Doc: https://docs.google.com/document/d/1ySeSMahcPlOSBYkPWiXMdqJ8c2PwzKsGFz7UmFfxYXY/edit?usp=sharing
                

