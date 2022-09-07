#include "esp_camera.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> //ArduinoJSON6
DynamicJsonDocument CONFIG(2048);

const char* ssid = "Arduino";
const char* password = "kernel1234";
const char* mqtt_server = "34.82.211.143";
const char* HostName = "ESP32CAM_MQTT_01";
const char* topic_PHOTO = "TakeAPicture";
const char* topic_UP = "/picture";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(String topic, byte* message, unsigned int length) {
  Serial.println("callback");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  if (topic == topic_PHOTO) {
    Serial.println("PING");
    take_picture();
  }
}

void camera_init() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = 5;
  config.pin_d1       = 18;
  config.pin_d2       = 19;
  config.pin_d3       = 21;
  config.pin_d4       = 36;
  config.pin_d5       = 39;
  config.pin_d6       = 34;
  config.pin_d7       = 35;
  config.pin_xclk     = 0;
  config.pin_pclk     = 22;
  config.pin_vsync    = 25;
  config.pin_href     = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn     = 32;
  config.pin_reset    = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size   = FRAMESIZE_VGA; // QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 10;           
  config.fb_count     = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void take_picture() {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  if (MQTT_MAX_PACKET_SIZE == 128) {
    //SLOW MODE (increase MQTT_MAX_PACKET_SIZE)
    client.publish_P(topic_UP, fb->buf, fb->len, false);
  }
  else {
    //FAST MODE (increase MQTT_MAX_PACKET_SIZE)
    client.publish(topic_UP, fb->buf, fb->len, false);
    Serial.println(fb->len);
  }
  Serial.println("CLIC");
  esp_camera_fb_return(fb);
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HostName);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName)) {
      Serial.println("connected");
      client.subscribe(topic_PHOTO);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  camera_init();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
