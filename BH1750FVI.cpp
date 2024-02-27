#include <Wire.h>
#include <BH1750FVI.h>

BH1750FVI LightSensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  LightSensor.begin();
}

void loop() {
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light intensity: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}