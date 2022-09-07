#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_wpa2.h"
#include <stdio.h>

#define EAP_IDENTITY ""
#define EAP_PASSWORD ""

//sensor
int digitalPin = 22;
char msg[50];

// WiFi
const char *ssid = "INHA-WLAN2"; // Enter your WiFi name
//const char *password = "";  // Enter WiFi password

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
 
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
  esp_wifi_sta_wpa2_ent_enable(&config);

  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 
// while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.println("Connecting to WiFi..");
// }
// Serial.println("Connected to the WiFi network");


 
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
