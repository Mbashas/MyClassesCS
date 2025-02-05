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
  
  delay(50); // Small delay to prevent multiple detections for a single clap


}

// Sensitivity (Hardware): This is adjusted by the potentiometer on your sound sensor module. It determines how much sound the sensor needs to pick up before it outputs a change in voltage. Turning the potentiometer adjusts the gain of the sensor. Higher sensitivity means the sensor will react to quieter sounds, while lower sensitivity requires louder sounds to trigger a response.
// Threshold (Software): This is the threshold variable in your Arduino code. It's a numerical value that the analogRead() value from the sensor must exceed to be considered a "clap." It acts as a filter on the sensor's output.
// Here’s how they interact:

// Sensitivity impacts Threshold: If the sensitivity is set too low (potentiometer), the sensor might not pick up quiet claps, and the analogRead() value might never exceed your threshold, no matter how low you set it. Conversely, if the sensitivity is set too high, the sensor might pick up a lot of background noise, causing the analogRead() value to constantly exceed the threshold, triggering false positives.
// Threshold impacts Sensitivity: The threshold value in your code can compensate for the sensitivity setting. If the sensitivity is high and the sensor is picking up too much noise, you can raise the threshold to ignore the noise. If the sensitivity is low, and the sensor isn't picking up claps reliably, you can lower the threshold (but only to a point – if the sensitivity is too low, lowering the threshold won't help).
// In summary, the potentiometer sets the hardware's sound level required for a response, and the threshold filters the resulting signal in software to determine if it's a valid "clap". They work together to detect claps accurately.