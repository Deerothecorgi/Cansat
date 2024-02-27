#include <Wire.h>
#include <MPU6050.h>

// MPU6050 setup
MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calculate acceleration in m/s^2
  float accelerationX = ax / 16384.0; // 16384 LSB per g (acceleration due to gravity)
  float accelerationY = ay / 16384.0;
  float accelerationZ = az / 16384.0;

  Serial.print("Acceleration X: ");
  Serial.print(accelerationX);
  Serial.print(" m/s^2, Y: ");
  Serial.print(accelerationY);
  Serial.print(" m/s^2, Z: ");
  Serial.print(accelerationZ);
  Serial.println(" m/s^2");

  delay(1000); // Adjust delay as needed
}
