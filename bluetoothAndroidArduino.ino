#include <SoftwareSerial.h>
// SoftwareSerial library for serial communication with Bluetooth module
// Can download from https://github.com/PaulStoffregen/SoftwareSerial

SoftwareSerial BTSerial(10, 11); // RX, TX for Bluetooth module

const int ledPin = 13; // LED connected to pin 13
bool isConnected = false; // Track Bluetooth connection status

void setup() {
    pinMode(ledPin, OUTPUT);
    BTSerial.begin(9600); // HC-05 default baud rate
    Serial.begin(9600); // Serial Monitor
    Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
    if (BTSerial.available()) {
        isConnected = true; // Bluetooth is connected if data is received
        char command = BTSerial.read();
        Serial.print("Received: ");
        Serial.println(command); // Print received command for debugging

        if (command == '1') { 
            digitalWrite(ledPin, HIGH); // Turn LED ON
            Serial.println("LED ON");
        } 
        else if (command == '0') { 
            digitalWrite(ledPin, LOW); // Turn LED OFF
            Serial.println("LED OFF");
        }
    }

    // Check Bluetooth status
    if (isConnected) {
        Serial.println("Bluetooth Connected.");
        isConnected = false; // Reset status until next message
    } else {
        Serial.println("Bluetooth Disconnected. Waiting...");
    }

    delay(1000); // Delay for better readability in Serial Monitor
}

// Make an app to send '1' to turn LED ON and '0' to turn LED OFF from MIT App Inventor
// Tutorial: https://randomnerdtutorials.com/mit-app-inventor-bluetooth-arduino-tutorial/
// https://www.youtube.com/watch?v=9thse2PmyoQ
// https://www.youtube.com/watch?v=w5LgLsCumFI&t=16s
// https://www.youtube.com/watch?v=aQcJ4uHdQEA