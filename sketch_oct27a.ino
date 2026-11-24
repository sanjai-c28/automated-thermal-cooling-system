#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire connected to Digital Pin 2
const int ONE_WIRE_BUS = 2; 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int relayPin = 7; // Relay connected to pin 7

float baselineTemp = 0.0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); 

  // Capture starting ambient/water temperature as baseline
  sensors.requestTemperatures();
  baselineTemp = sensors.getTempCByIndex(0);
  
  Serial.println("=== SYSTEM INITIALIZED ===");
  Serial.print("Baseline Ambient Temp: ");
  Serial.print(baselineTemp);
  Serial.println(" °C");
}

void loop() {
  sensors.requestTemperatures(); 
  float currentTemp = sensors.getTempCByIndex(0);
  
  // Calculate thermal lift (how much the temperature has risen from start)
  float tempRise = currentTemp - baselineTemp;

  // Print live readings and calculations
  Serial.print("Current Temp: ");
  Serial.print(currentTemp);
  Serial.print(" °C | Thermal Lift (ΔT): +");
  Serial.print(tempRise);
  Serial.println(" °C");

  // Control Logic (30°C Threshold)
  if (currentTemp < 30.0) {
    digitalWrite(relayPin, HIGH); // Turn heating loop ON
    Serial.println("-> Status: Capturing Waste Heat & Pre-Heating Water");
  } else {
    digitalWrite(relayPin, LOW);  
    Serial.println("-> Status: Target Temp Reached - Optimal Feedwater Temp");
  }
  
  Serial.println("--------------------------------------------------");
  delay(2000);
}
