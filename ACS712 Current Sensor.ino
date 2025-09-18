// ESP8266 code for ACS712 20A Current Sensor to measure AC Current (RMS)

// The analog pin connected to the OUT pin of the ACS712
const int SENSOR_PIN = A0;

// The ACS712 20A model has a sensitivity of 100mV/A (0.1V/A)
const float SENSITIVITY = 0.100; // Volts per Amp

// The operating voltage of the ESP8266 is 3.3V.
const float VCC = 3.3;

// ===============================================================
// SETUP: Runs once when the board is powered on or reset
// ===============================================================
void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  Serial.println("\nACS712 AC Current Sensor");
  Serial.println("--------------------------");
}

// ===============================================================
// LOOP: Runs continuously after setup()
// ===============================================================
void loop() {
  // Calculate the RMS current
  float Irms = getRMSCurrent();

  // Print the results to the Serial Monitor
  Serial.print("AC Current: ");
  Serial.print(Irms, 2); // Print current with 2 decimal places
  Serial.println(" A");

  // Wait for a second before the next reading
  delay(1000);
}

// ===============================================================
// getRMSCurrent(): Samples the sensor to find peak-to-peak voltage
// and calculates the RMS current.
// ===============================================================
float getRMSCurrent() {
  int rawValue;
  int maxValue = 0;
  int minValue = 1023;

  // The sampling window should be at least one full cycle of the AC waveform.
  // For 50Hz (20ms) or 60Hz (16.7ms), a 100ms window is more than enough.
  unsigned long startTime = millis();
  while ((millis() - startTime) < 100) {
    rawValue = analogRead(SENSOR_PIN);
    if (rawValue > maxValue) {
      maxValue = rawValue; // Capture the maximum ADC reading
    }
    if (rawValue < minValue) {
      minValue = rawValue; // Capture the minimum ADC reading
    }
  }

  // Calculate the peak-to-peak voltage from the ADC readings
  // Vpp = (max ADC - min ADC) * (Vref / ADC resolution)
  float Vpp = ((maxValue - minValue) * VCC) / 1023.0;

  // Calculate the peak current (I_peak)
  // I_peak = (Vpp / 2) / Sensitivity
  float Ipeak = (Vpp / 2.0) / SENSITIVITY;

  // Calculate the RMS current (I_rms) for a sine wave
  // I_rms = I_peak / sqrt(2)
  float Irms = Ipeak * 0.7071;

  return Irms;
}
