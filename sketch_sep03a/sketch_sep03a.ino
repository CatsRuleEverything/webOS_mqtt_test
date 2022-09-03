#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>

//sensor
int digitalPin = 22;
char msg[50];

// WiFi
const char *ssid = "Arduino"; // Enter your WiFi name
const char *password = "kernel1234";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "34.82.211.143";
const char *topic = "esp32/doorOpen";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  // sensor
 pinMode(digitalPin, INPUT);
  
 // Set software serial baud to 115200;
 Serial.begin(9600);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str())) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 Serial.println("publish1");
 client.publish(topic, "1");

}


void loop() {
  int value = digitalRead(digitalPin);
  Serial.printf("digital read : %d\n",value);
  sprintf(msg, "%c", value);
  if(!value){
    client.publish(topic, "open"); 
  }
  else client.publish(topic, "closed");
 delay(500);
 client.loop();
}
