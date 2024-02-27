#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <BH1750FVI.h>

// WiFi and Netpie credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
const char* netpie_user = "NETPIE_USER";
const char* netpie_token = "NETPIE_TOKEN";
const char* netpie_topic = "NETPIE_TOPIC";

// DHT11 Sensor
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// GYNE06MY2GPS Sensor
#define GPS_RX D5
#define GPS_TX D6
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;


// BH1750FVI Sensor
BH1750FVI lightMeter;

// WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming messages if needed
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", netpie_user, netpie_token)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();
  lightMeter.begin();

  setup_wifi();
  client.setServer("mqtt.netpie.io", 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Reading sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float lightIntensity = lightMeter.GetLightIntensity();

  // GPS Data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude= ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude= ");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }

  // Publish sensor data to Netpie
  String payload = String("{\"temperature\":") + temperature + ",\"humidity\":" + humidity + ",\"light\":" + lightIntensity + "}";
  client.publish(netpie_topic, payload.c_str());

  delay(5000); // Adjust delay as needed
}
