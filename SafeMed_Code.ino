// -------------------- Time Delay Define Start ----------------
const int timeDelay = 20; // Delay in seconds
// -------------------- Time Delay Define End ----------------

// -------------------- Wi-Fi Define Start ----------------
#include "WiFi.h"
#include <HTTPClient.h>
#define On_Board_LED_PIN  2

const char* ssid = "Bandit enters the room";  
const char* password = "handitbeeler";

String Web_App_URL = "https://script.google.com/macros/s/AKfycbwOZAX2ehqijKbzVNg-LP2hKbNG8uoRXYefr-Xe41-hjOcG4KhYyKLVfbgaWj_jsByNRg/exec";

float temp;  // Temperature value
float humid; // Humidity value
float light; // Light intensity value
// -------------------- Wi-Fi Define End ----------------

// --------------------------- DHT11 Define Start -----------------------------
#include <DHT.h>
#include <DHT_U.h>
#define DHT11_GPIO 23
DHT dht11(DHT11_GPIO, DHT11);
// --------------------------- DHT11 Define End -----------------------------

// --------------------------- LDR Define Start -----------------------------
#define LDR_PIN A0 // LDR connected to analog pin A0
// --------------------------- LDR Define End -----------------------------

// --------------------------- Buzzer Define Start ---------------------------
#define BUZZER_PIN 9 // Buzzer connected to digital pin 9
#define TEMP_THRESHOLD 25 // Temperature threshold in Celsius
#define HUMID_THRESHOLD 60 // Humidity threshold in percentage
#define LIGHT_THRESHOLD 200 // LDR threshold value
// --------------------------- Buzzer Define End -----------------------------

void setup() {
  // Serial Communication Started
  Serial.begin(9600);
  
  // ----- DHT11 Setup Start ---------
  dht11.begin();
  // ----- DHT11 Setup End ---------
  
  // ----- Wi-Fi Setup Start ---------
  pinMode(On_Board_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); 
  digitalWrite(BUZZER_PIN, LOW);

  // Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");

  // Connect to Wi-Fi (STA).
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(On_Board_LED_PIN, HIGH);
    delay(250);
    digitalWrite(On_Board_LED_PIN, LOW);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  digitalWrite(On_Board_LED_PIN, LOW);
  Serial.println("WiFi connected");
  // ----- Wi-Fi Setup End ---------
}

void loop() {
  // Store the start time of the loop
  unsigned long startTime = millis();

  // ----- Read DHT11 Data ---------
  temp = dht11.readTemperature();
  humid = dht11.readHumidity();

  // ----- Read LDR Data ---------
  light = analogRead(LDR_PIN);

  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(humid);
  Serial.print(" %, Light Intensity: ");
  Serial.println(light);

  // ----- Buzzer Alert Logic ---------
  if (temp > TEMP_THRESHOLD || humid > HUMID_THRESHOLD || light < LIGHT_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
  }

  // ----- Send Data to Google Sheets ---------
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(On_Board_LED_PIN, HIGH);

    // Create a URL for sending data
    String Send_Data_URL = Web_App_URL + "?sts=write" + "&temp=" + temp + "&humid=" + humid + "&lux=" + light;

    // Initialize HTTPClient as "http".
    HTTPClient http;
    // HTTP GET Request.
    http.begin(Send_Data_URL.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);

    // Print response payload
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Payload: " + payload);    
    }
      
    http.end();
    digitalWrite(On_Board_LED_PIN, LOW);
  }

  // ----- Enforce 20-second delay ---------
  while (millis() - startTime < timeDelay * 1000) {
    delay(100); // Small delay to avoid blocking
  }
}
