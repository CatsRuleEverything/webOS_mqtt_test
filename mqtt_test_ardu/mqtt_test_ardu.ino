#include <SoftwareSerial.h>
#include <WiFiEsp.h> 
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include <stdlib.h>
#include <Wire.h>


IPAddress server();  //your mqtt server adress
char ssid[] = "";           // your network SSID (name)
char pass[] = "";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

WiFiEspClient esp8266Client;
PubSubClient client(esp8266Client);
SoftwareSerial esp8266(10, 11); // RX, TX to ESP-01


void setup(){
  Serial.begin(9600);
  esp8266.begin(9600);  //software serial to ESP8266
  WiFi.init(&esp8266); //ESP8266 wifi    //HDC1000 sensor
  // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
    // don't continue
    while (true);
    }
  // attempt to connect to WiFi network
    while ( status != WL_CONNECTED) {
       Serial.print("Attempting to connect to WPA SSID: ");
       Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    }
  // you're connected now, so print out the data
    Serial.println("You're connected to the network");
  //connect to MQTT server
    client.setServer("",1883);
    //client.setCallback(callback);
}

void loop(){
 
  // mqtt out
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    client.connect("arduinoClient1");
    Serial.println("connected");
    delay (1000);
    client.publish("temperature","온도1");
    delay (1000);
    client.publish("humidity","습도1");
    delay (1000);
  }
  Serial.println("퍼블리싱중"); 
  client.publish("temperature","777");
  delay (1000);
  client.publish("humidity","습도2");
  delay (1000);
  client.loop();
  delay(3000);
}
