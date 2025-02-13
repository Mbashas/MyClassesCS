#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "Mashas";
const char* password = "12345678";

WebSocketsServer webSocket = WebSocketsServer(81);

const int LED_PIN = 2;
const int MOISTURE_SENSOR_PIN = 34;  // Soil moisture sensor connected to GPIO34

// Variables to control sensor update interval (every 5 seconds)
unsigned long previousSensorMillis = 0;
const unsigned long sensorInterval = 5000;  // 5 seconds in milliseconds

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;

        case WStype_CONNECTED: {
            Serial.printf("[%u] Connected!\n", num);
            String message = "Welcome to ESP32 WebSocket Server!";
            webSocket.sendTXT(num, message);
        }
        break;

        case WStype_TEXT: {
            String text = String((char*)payload);
            Serial.printf("[%u] Received Text: %s\n", num, text.c_str());

            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, text);

            if (!error) {
                if (doc.containsKey("led")) {
                    bool ledState = doc["led"];
                    digitalWrite(LED_PIN, ledState);

                    String response = "{\"status\":\"ok\",\"led\":" + String(ledState) + "}";
                    webSocket.sendTXT(num, response);
                }
            }
        }
        break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    // Note: For analog inputs on ESP32, simply use analogRead without further pinMode configuration

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();

    // Check if it is time to update the sensor reading
    unsigned long currentMillis = millis();
    if (currentMillis - previousSensorMillis >= sensorInterval) {
        previousSensorMillis = currentMillis;
        // Read the soil moisture sensor value (raw analog value, typically 0-4095)
        int sensorValue = analogRead(MOISTURE_SENSOR_PIN);

        // Create a JSON object containing the sensor reading
        StaticJsonDocument<200> doc;
        doc["sensor"] = sensorValue;
        String jsonString;
        serializeJson(doc, jsonString);

        // Broadcast the sensor reading to all connected WebSocket clients
        webSocket.broadcastTXT(jsonString);

        Serial.printf("Sensor reading: %d\n", sensorValue);
    }
}
