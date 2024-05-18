#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define infoPIN 13 //D7
#define versionPIN 4 //D2

const char* ssid = "WAVE_ROVER"; 
const char* password = "12345678"; 

int obstacles;
int state=0;
int state1=0;
unsigned long startTime;
unsigned long stopStart;
unsigned long stopEnd;
unsigned long stopTime;
unsigned long elapsedTime;
bool obstacle;
int obstaclesCounter;
int version;

WiFiClient wifiClient;

void setup() {
  pinMode(infoPIN, INPUT_PULLUP);
  pinMode(versionPIN, INPUT_PULLUP);
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
  if(digitalRead(versionPIN)==HIGH){version=0;}else{version=1;}
  StaticJsonDocument<200> forwardNormal;
  forwardNormal["T"] = 1;
  forwardNormal["L"] = 140;
  forwardNormal["R"] = 170;

  StaticJsonDocument<200> forwardSlow;
  forwardSlow["T"] = 1;
  forwardSlow["L"] = 50;
  forwardSlow["R"] = 50;

  StaticJsonDocument<200> leftFast;
  leftFast["T"] = 1;
  leftFast["L"] = -255;
  leftFast["R"] = 255;

  StaticJsonDocument<200> leftFast1;
  leftFast1["T"] = 1;
  leftFast1["L"] = 50;
  leftFast1["R"] = 220;

  StaticJsonDocument<200> leftSlow1;
  leftSlow1["T"] = 1;
  leftSlow1["L"] = 90;
  leftSlow1["R"] = 220;
  

  StaticJsonDocument<200> leftSlow1obs;
  leftSlow1obs["T"] = 1;
  leftSlow1obs["L"] = 50;
  leftSlow1obs["R"] = 61;

  StaticJsonDocument<200> stop;
  leftSlow1obs["T"] = 0;


  String jsonStr;
  if(version==0){
    switch(state){
      case 0:
      Serial.println("wtf");
        if(obstacles==0){
          if (currentTime - startTime < 10000+stopTime){
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
        if (currentTime - startTime < 540) { 
            serializeJson(leftFast, jsonStr);
          } else {
            startTime = currentTime;
            stopTime=0;
            state++;
          }
          break;
      case 2: 
        if(obstacles==0){
          if (currentTime - startTime < 8500){
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
        if (currentTime - startTime < 540) { 
              serializeJson(leftFast, jsonStr);
            } else {
              startTime = currentTime;
              stopTime=0;
              state++;
            }
            break;
      case 4:
        if(obstacles==0){
          if (currentTime - startTime < 10000+stopTime){
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
          if (currentTime - startTime < 17000+stopTime) { 
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
  }
  if(version==1){
    switch(state1){
      case 0:
      Serial.println("1");
        if(obstacles==0){
            if (currentTime - startTime < 22610+stopTime){
              serializeJson(forwardNormal, jsonStr);
            }
            else {
              startTime = currentTime;
              stopTime=0;
              state1++;
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
      Serial.println("2");
        if(obstacles==0){
            if (currentTime - startTime < 13050+stopTime) { 
                serializeJson(leftFast1, jsonStr);
                Serial.println(jsonStr);
              } else {
                startTime = currentTime;
                stopTime=0;
                state1=0;
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
  }


  

  String ip_addr = "192.168.4.1"; 
  String url = "http://" + ip_addr + "/js?json=" + jsonStr;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    

    http.begin(wifiClient, url);
      
    int httpCode = http.GET();
    

    if (httpCode > 0) { 
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        } 
      else {}
    } 
    else {}
    http.end(); 
  } 
  else {} 
}
