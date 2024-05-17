#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define infoPIN 

const char* ssid = "WAVE_ROVER"; // Replace with your WiFi SSID
const char* password = "12345678"; // Replace with your WiFi password

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Check signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}

void loop() {
  StaticJsonDocument<200> doc;
  doc["T"] = 1;
  doc["L"] = 164;
  doc["R"] = 164;
  String jsonStr;
  serializeJson(doc, jsonStr);

  String ip_addr = "192.168.4.1"; // Replace with your IP address
  String url = "http://" + ip_addr + "/js?json=" + jsonStr;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    Serial.print("Connecting to URL: ");
    Serial.println(url);

    http.begin(wifiClient, url); // Use the new begin method
      
    int httpCode = http.GET();
    Serial.print("HTTP response code: ");
    Serial.println(httpCode);

    if (httpCode > 0) { // Check for the returning code
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        } 
      else {
        Serial.print("Error: HTTP response code ");
        Serial.println(httpCode);
      }
    } 
    else {
      Serial.print("Error on HTTP request: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end(); // Free the resources
    } 
    else {
    Serial.println("WiFi not connected");
  }
  
}
