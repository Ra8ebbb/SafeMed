#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Replace with your Google Apps Script web app URL
const char* googleScriptURL = "YOUR_GOOGLE_SCRIPT_WEB_APP_URL";

// Define pins
#define DHTPIN 4        // DHT11 connected to GPIO4
#define DHTTYPE DHT11   // DHT11 sensor type
#define LDR_PIN 36      // LDR connected to ADC pin (GPIO36)
#define BUZZER_PIN 26   // Buzzer connected to GPIO26

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Initialize sensors and buzzer
  dht.begin();
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Read sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LDR_PIN);

  // Print readings to Serial Monitor
  Serial.printf("Temp: %.2f °C, Humidity: %.2f%%, Light: %d\n", temperature, humidity, lightIntensity);

  // Buzzer logic
  if (temperature > 30 || humidity > 60 || lightIntensity < 150) {
    digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
  }

  // Send data to Google Spreadsheet
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(googleScriptURL);

    // Create JSON payload
    String jsonPayload = "{\"temperature\":" + String(temperature) +
                         ",\"humidity\":" + String(humidity) +
                         ",\"light\":" + String(lightIntensity) + "}";

    // Send HTTP POST request
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.printf("Data sent! Response code: %d\n", httpResponseCode);
    } else {
      Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Wi-Fi disconnected!");
  }

  // Wait 20 seconds before taking the next reading
  delay(20000);
}