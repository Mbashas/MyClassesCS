#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 7
#define GREEN_LED_PIN 4   // Green LED pin
#define RED_LED_PIN 5     // Red LED pin
#define BUZZER_PIN 8      // Buzzer pin
#define RELAY_PIN 3       // Relay pin to control servo motor
#define SERVO_PIN 9       // Servo motor pin

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myServo;                  // Create Servo instance

// Define the correct card UID (replace with your actual card UID)
byte correctCardUID[] = {0x60, 0xEA, 0x21, 0x12}; // Example UID

void setup() {
  Serial.begin(9600);         // Start the serial communication at 9600 baud rate
  SPI.begin();                // Start the SPI bus
  rfid.PCD_Init();            // Initialize the MFRC522 reader
  
  // Set the LED, Buzzer, and Relay pins as outputs
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Attach the servo motor to the defined pin
  myServo.attach(SERVO_PIN);
  
  // Turn off all initially (LEDs, buzzer, relay)
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);  // Relay off initially

  Serial.println("System ready. Scan an RFID card to get its UID...");
}

void loop() {
  // Check if a new RFID card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Read the card's serial number (UID)
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print the UID of the card to the serial monitor
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println(); // Add a new line after the UID

  // Check if the scanned card is the correct one
  if (checkUID(rfid.uid.uidByte, rfid.uid.size)) {
    // Correct card detected
    Serial.println("Correct Card! Access Granted.");
    digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on green LED
    digitalWrite(BUZZER_PIN, HIGH);     // Turn on buzzer for short beep
    delay(200);                         // Beep duration
    digitalWrite(BUZZER_PIN, LOW);      // Turn off buzzer

    // Activate relay and start servo motor (continuous rotation)
    Serial.println("Activating relay and starting continuous rotation of servo...");
    digitalWrite(RELAY_PIN, HIGH);      // Turn on relay (enable servo motor)
    myServo.write(180);                 // Servo rotates continuously in one direction
    delay(5000);                        // Keep rotating for 5 seconds (or adjust as needed)
    
    // Stop the servo motor
    Serial.println("Stopping servo motor...");
    myServo.write(90);                  // Stop servo motor (neutral position)
    digitalWrite(RELAY_PIN, LOW);       // Turn off relay (disable servo motor)
    digitalWrite(GREEN_LED_PIN, LOW);   // Turn off green LED
  } else {
    // Incorrect card detected
    Serial.println("Incorrect Card! Access Denied.");
    digitalWrite(RED_LED_PIN, HIGH);    // Turn on red LED
    
    // Continuous short beeps for 3 seconds
    unsigned long startTime = millis(); // Record the start time
    while (millis() - startTime < 3000) {  // 3 seconds duration
      digitalWrite(BUZZER_PIN, HIGH);   // Turn on buzzer for short beep
      delay(200);                       // Beep duration
      digitalWrite(BUZZER_PIN, LOW);    // Turn off buzzer
      delay(200);                       // Pause between beeps
    }
    
    digitalWrite(RED_LED_PIN, LOW);     // Turn off red LED

    // Keep relay off, so the servo motor does nothing
    Serial.println("Relay remains off, servo does nothing.");
    digitalWrite(RELAY_PIN, LOW);       // Ensure relay is off
    myServo.write(90);                  // Ensure servo motor is stopped
  }

  // Halt the card after reading the UID
  rfid.PICC_HaltA();
}

// Routine to compare the scanned UID with the correct one
bool checkUID(byte *buffer, byte bufferSize) {
  if (bufferSize != sizeof(correctCardUID)) return false; // If sizes don't match
  for (byte i = 0; i < bufferSize; i++) {
    if (buffer[i] != correctCardUID[i]) return false;
  }
  return true;
}
