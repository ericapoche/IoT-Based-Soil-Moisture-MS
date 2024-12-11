#include <WiFi.h>
#include <Wire.h>   /*Only needed for Arduino 1.6.5 and earlier*/            
#include <SH1106.h>      /* legacy: #include "SSD1306.h"*/
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char *ssid =  "Back_Space";  /*Your Wifi Name*/
const char *pass =  "Grateful24";  /*Wifi Password*/
 
SH1106 display(0x3c, 21, 22);
WiFiClient client;
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "ericapoche" /*Adafruit Username*/
#define MQTT_PASS "aio_NuwA96oBH4W2ROpG2ZoX7acsrrQV" /*Adafruit IO AIO key*/

const int moisturePin = A0;   /*Moisture Sensor Pin*/         
int moisturePercentage;   /*Moisture Reading*/          

Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Publish AgricultureData = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/AgricultureData"); /*AgricultureData is the feed name where you will publish your data*/
void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("");  /*print ... till not connected*/           
  }
  Serial.println("");
  Serial.println("WiFi Connected!!");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}
void loop()
{
    MQTT_connect();
    int sensorValue = analogRead(moisturePin);
    moisturePercentage = ( 100 - ( (analogRead(moisturePin) / 4095.00) * 100 ) );

    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(10, 128, String(millis()));
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(50, 0, "Soil");
    display.drawString(25, 20, "Moisture");

    String data1 = String(moisturePercentage);
    display.drawString(45, 40, data1 + " %");
    display.display();

    if (!AgricultureData.publish(moisturePercentage))  /*This condition is used to publish the Variable (moisturePercentage) on adafruit IO. Change thevariable according to yours.*/
    {
        Serial.println(F("Failed to publish data to Adafruit IO."));
        delay(5000);
    }

    delay(6000);
}


void MQTT_connect() 
{
  int8_t ret;
 
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) /*connect will return 0 for connected*/
  { 
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0)  
       {
         
         while (1);
       }
  }
}

//Soil Moisture