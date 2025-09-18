// A clear and reliable code for the ZMPT101B AC Voltage Sensor

// Define the analog pin connected to the sensor's output
const int SENSOR_PIN = A0;

// Set the operating voltage of your Arduino (5.0V for Uno/Nano, 3.3V for ESP boards)
const float VCC = 3.3;

// Set the resolution of the ADC (1023.0 for 10-bit Arduinos)
const float ADC_RESOLUTION = 1023.0;

// ====================================================================================
// This is the only value you need to change. See calibration instructions below.
// ====================================================================================
float calibration_factor = 735.7; // This is an EXAMPLE value. Calculate your own!


// Variable to hold the DC offset (the "zero point" of the sine wave)
int zeroPoint;

void setup() {
  Serial.begin(115200);
  
  // Find the zero-point of the sensor automatically
  // Run this with no AC voltage connected to the sensor
  calibrateZeroPoint();
}

void loop() {
  float voltageRMS = getVoltageRMS();

  Serial.print("Voltage: ");
  Serial.print(voltageRMS, 2); // Print RMS voltage with 2 decimal places
  Serial.println(" V");

  delay(1000);
}

// Finds the DC offset of the sensor by taking multiple readings
void calibrateZeroPoint() {
  Serial.println("Calibrating sensor... Please wait.");
  long totalValue = 0;
  // Take 500 readings to get a stable average
  for (int i = 0; i < 500; i++) {
    totalValue += analogRead(SENSOR_PIN);
    delay(1);
  }
  zeroPoint = totalValue / 500;
  Serial.print("Calibration complete. Zero Point = ");
  Serial.println(zeroPoint);
}

// Samples the waveform and calculates the RMS voltage
float getVoltageRMS() {
  int rawValue;
  int maxValue = 0;
  int minValue = 1023;

  // Sample the waveform for a period to find the peak-to-peak amplitude
  unsigned long startTime = millis();
  while ((millis() - startTime) < 100) { // Sample for 100ms
    rawValue = analogRead(SENSOR_PIN);
    if (rawValue > maxValue) {
      maxValue = rawValue; // Find the highest point
    }
    if (rawValue < minValue) {
      minValue = rawValue; // Find the lowest point
    }
  }

  // Calculate the peak-to-peak ADC reading
  int peakToPeak = maxValue - minValue;
  
  // The peak ADC reading is half of the peak-to-peak value
  int peakValue = peakToPeak / 2;

  // Convert the peak ADC value to a peak voltage at the pin
  // Vpeak = (peak ADC value * VCC) / ADC Resolution
  float peakVoltage = (peakValue * VCC) / ADC_RESOLUTION;

  // Calculate the RMS voltage at the pin (for a sine wave)
  // Vrms = Vpeak / sqrt(2)
  float rmsVoltage_uncalibrated = peakVoltage * 0.7071;

  // Apply the calibration factor to get the actual mains voltage
  float rmsVoltage_calibrated = rmsVoltage_uncalibrated * calibration_factor;

  return rmsVoltage_calibrated;
}
