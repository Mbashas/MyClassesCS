#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Define sensor pin and type
#define DHTPIN 13        // DHT11 connected to digital pin 4 (adjust if needed)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Replace with your WiFi network credentials
const char* ssid = "ssid";
const char* password = "password";

// Replace with your Google Apps Script URL (the web app deployment URL)
const char* googleScriptUrl = "https://script.google.com/macros/s/SHEET_ID/exec";

// Temperature threshold (for example purposes, the Apps Script can trigger an email if the reading exceeds this)
const float tempThreshold = 30.0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  // Read temperature from the DHT11 sensor 
  float temperature = dht.readTemperature();
  
  // Check if the reading is valid
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Construct the GET request URL. The parameter name "reading" is used by the Apps Script.
  String url = String(googleScriptUrl) + "?reading=" + String(temperature, 2);
  Serial.print("Sending data to: ");
  Serial.println(url);

  // Check WiFi connection and send the HTTP GET request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();  // Send the GET request

    if (httpResponseCode > 0) {
      // Get the response payload (for debugging)
      String payload = http.getString();
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(payload);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
  
  // The Apps Script will check the reading and, if it exceeds the threshold,
  // trigger an email alert.
  
  // Wait before the next reading (adjust as needed)
  delay(500);
}