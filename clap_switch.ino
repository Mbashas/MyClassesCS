const int sensorAnalogPin = A0; // Analog output pin of the sound sensor
const int ledPin1 = 2;          // Pin for the first LED
const int ledPin2 = 3;          // Pin for the second LED

const int threshold = 500;     // Threshold for detecting a clap - adjust based on your sensor

bool led1State = LOW;         // Initial state of LED 1 (off)
bool led2State = HIGH;        // Initial state of LED 2 (on)
bool clapInProgress = false;  // New flag for debouncing

void setup() {
  pinMode(sensorAnalogPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(9600); // Initialize serial communication for monitoring
}

void loop() {
  int sensorValue = analogRead(sensorAnalogPin);
  Serial.println(sensorValue); // Print sensor value to Serial Monitor

  // When sensor value exceeds threshold and no clap is in progress
  if (sensorValue > threshold && !clapInProgress) {
      clapInProgress = true;
      // Toggle LED states
      led1State = !led1State;
      led2State = !led2State;
      digitalWrite(ledPin1, led1State);
      digitalWrite(ledPin2, led2State);
  }
  // Reset the flag when the sensor value has dropped below threshold
  else if (sensorValue < threshold && clapInProgress) {
    clapInProgress = false;
  }
}