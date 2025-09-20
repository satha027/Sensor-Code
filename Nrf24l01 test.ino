#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define the pins based on your provided table
#define CE_PIN 22
#define CSN_PIN 21

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Define the built-in LED pin for the ESP32
const int LED_PIN = 2; 

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  Serial.println(F("Starting nRF24L01 status check..."));

  // This function attempts to initialize the nRF24L01 module.
  // It returns true if successful, false otherwise.
  if (radio.begin()) {
    // If the module works (radio.begin() returns true)
    Serial.println(F("SUCCESS: nRF24L01 module detected."));
    Serial.println(F("The LED will now blink every second."));
    
    // You can optionally print the configuration details here
    // radio.printDetails(); 
  } else {
    // If the module is not detected
    Serial.println(F("ERROR: nRF24L01 module not detected."));
    Serial.println(F("The LED will be HIGH continuously."));
    digitalWrite(LED_PIN, HIGH); // Turn the LED ON continuously
    
    // Halt the program, as the core task failed
    while (1) {} 
  }
}

void loop() {
  // This part of the code only runs if radio.begin() was successful
  digitalWrite(LED_PIN, HIGH); // Turn the LED ON
  delay(1000); // Wait for 1 second
  digitalWrite(LED_PIN, LOW); // Turn the LED OFF
  delay(1000); // Wait for 1 second
}
