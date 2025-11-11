#include <Arduino.h>

const int potPin = 34;  // GPIO34 (ADC1_CH6)
const float Vref = 3.3; // ESP32 ADC reference voltage

void setup() {
  Serial.begin(115200);  // Start serial communication
}

void loop() {
  int val = analogRead(potPin);           // Read ADC value (0–4095)
  float voltage = val * Vref / 4095.0;    // Convert to actual voltage
  Serial.print("ADC: ");
  Serial.print(val);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);               // Print with 3 decimal places
  Serial.println(" V");
  delay(100);
}