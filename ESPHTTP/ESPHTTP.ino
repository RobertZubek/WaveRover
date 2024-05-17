#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define infoPIN 13 //D7

const char* ssid = "WAVE_ROVER"; // Replace with your WiFi SSID
const char* password = "12345678"; // Replace with your WiFi password

int obstacles;
int state=0;
unsigned long startTime;
unsigned long stopStart;
unsigned long stopEnd;
unsigned long stopTime;
unsigned long elapsedTime;
bool obstacle;
int obstaclesCounter;

WiFiClient wifiClient;

void setup() {
  pinMode(infoPIN, INPUT_PULLUP);
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
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  if(digitalRead(infoPIN)==HIGH){obstacles=1;}else{obstacles=0;}
  StaticJsonDocument<200> forwardNormal;
  forwardNormal["T"] = 1;
  forwardNormal["L"] = 180;
  forwardNormal["R"] = 180;

  StaticJsonDocument<200> forwardSlow;
  forwardSlow["T"] = 1;
  forwardSlow["L"] = 50;
  forwardSlow["R"] = 50;

  StaticJsonDocument<200> leftFast;
  leftFast["T"] = 1;
  leftFast["L"] = -164;
  leftFast["R"] = 164;

  StaticJsonDocument<200> leftSlow1;
  leftSlow1["T"] = 1;
  leftSlow1["L"] = 153;
  leftSlow1["R"] = 187;

  StaticJsonDocument<200> leftSlow1obs;
  leftSlow1obs["T"] = 1;
  leftSlow1obs["L"] = 50;
  leftSlow1obs["R"] = 61;

  StaticJsonDocument<200> stop;
  leftSlow1obs["T"] = 1;
  leftSlow1obs["L"] = 0;
  leftSlow1obs["R"] = 0;


  String jsonStr;

  switch(state){
    case 0:
      if(obstacles==0){
        if (currentTime - startTime < 5000+stopTime){
          serializeJson(forwardNormal, jsonStr);
        }
        else {
          startTime = currentTime;
          stopTime=0;
          state++;
        }
      }
      else{
        stopStart=currentTime;
        while(obstacles==1){
          if(digitalRead(infoPIN)==HIGH){obstacles=1;}else{obstacles=0;}
          serializeJson(stop, jsonStr);
        }
        stopEnd=currentTime;
        stopTime+=stopEnd-stopStart;
      }
      break;
    case 1:
      if (currentTime - startTime < 2000) { // Zakładamy, że skręt trwa 2 sekundy
          serializeJson(leftFast, jsonStr);
        } else {
          startTime = currentTime;
          stopTime=0;
          state++;
        }
        break;
    case 2: 
      if(obstacles==0){
        if (currentTime - startTime < 4300){
          serializeJson(forwardNormal, jsonStr);
        }
        else {
          startTime = currentTime;
          stopTime=0;
          state++;
        }

      }
      else{
        stopStart=currentTime;
        while(obstacles==1){
          if(digitalRead(infoPIN)==HIGH){obstacles=1;}else{obstacles=0;}
          serializeJson(stop, jsonStr);
        }
        stopEnd=currentTime;
        stopTime+=stopEnd-stopStart;
      }
    break;
    case 3:
      if (currentTime - startTime < 2000) { // Zakładamy, że skręt trwa 2 sekundy
            serializeJson(leftFast, jsonStr);
          } else {
            startTime = currentTime;
            stopTime=0;
            state++;
          }
          break;
    case 4:
      if(obstacles==0){
        if (currentTime - startTime < 5000+stopTime){
          serializeJson(forwardNormal, jsonStr);
        }
        else {
          startTime = currentTime;
          stopTime=0;
          state++;
        }

      }
      else{
        stopStart=currentTime;
        while(obstacles==1){
          if(digitalRead(infoPIN)==HIGH){obstacles=1;}else{obstacles=0;}
          serializeJson(stop, jsonStr);
        }
        stopEnd=currentTime;
        stopTime+=stopEnd-stopStart;
      }
      break;
    case 5:
      if(obstacles==0){
        if (currentTime - startTime < 2000+stopTime) { // Zakładamy, że skręt trwa 2 sekundy
            serializeJson(leftSlow1, jsonStr);
          } else {
            startTime = currentTime;
            stopTime=0;
            state=0;
          }
      }
      else{
        stopStart=currentTime;
        while(obstacles==1){
          if(digitalRead(infoPIN)==HIGH){obstacles=1;}else{obstacles=0;}
          serializeJson(stop, jsonStr);
        }
        stopEnd=currentTime;
        stopTime+=stopEnd-stopStart;
      }
    break;

  }



  

  String ip_addr = "192.168.4.1"; // Replace with your IP address
  String url = "http://" + ip_addr + "/js?json=" + jsonStr;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    //Serial.print("Connecting to URL: ");
    //Serial.println(url);

    http.begin(wifiClient, url); // Use the new begin method
      
    int httpCode = http.GET();
    //Serial.print("HTTP response code: ");
    //Serial.println(httpCode);

    if (httpCode > 0) { // Check for the returning code
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        //Serial.println(payload);
        } 
      else {
        //Serial.print("Error: HTTP response code ");
        //Serial.println(httpCode);
      }
    } 
    else {
      //Serial.print("Error on HTTP request: ");
      //Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end(); // Free the resources
    } 
    else {
    //Serial.println("WiFi not connected");
  }
  
}
