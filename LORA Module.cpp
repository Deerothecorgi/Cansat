#include <SPI.h>
#include <LoRa.h>

#define BAND 915E6  // LoRa frequency

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Send data
  LoRa.beginPacket();
  LoRa.print("Hello, world!");
  LoRa.endPacket();

  delay(1000);
}