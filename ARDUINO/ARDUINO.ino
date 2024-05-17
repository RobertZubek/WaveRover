#include <Arduino_FreeRTOS.h>
#include "semphr.h"

#define rtDelay(v) vTaskDelay(v/15)

#define triggerPINLP 5
#define echoPINLP 4

#define triggerPINPP A1
#define echoPINPP A0

#define triggerPINP A5
#define echoPINP A4

#define triggerPINL 2
#define echoPINL 3

#define infoPIN 7

int state = 1;
SemaphoreHandle_t binarySemaphore;

void setup() {

  Serial.begin(9600);
  
  xTaskCreate(run, "run", 128, NULL, 0, NULL);
  xTaskCreate(guard, "guard", 128, NULL, 2, NULL);
  binarySemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(binarySemaphore);
  

  pinMode(infoPIN, OUTPUT);

  pinMode(triggerPINLP, OUTPUT);
  pinMode(echoPINLP, INPUT);

  pinMode(triggerPINPP, OUTPUT);
  pinMode(echoPINPP, INPUT);

  pinMode(triggerPINL, OUTPUT);
  pinMode(echoPINL, INPUT);

  pinMode(triggerPINP, OUTPUT);
  pinMode(echoPINP, INPUT);
  
  

}
void run(void*){

  

  while(1){
    
    if(state==1){
      xSemaphoreTake(binarySemaphore, portMAX_DELAY);
      Serial.println("RUN");
      xSemaphoreGive(binarySemaphore);
    }
    rtDelay(50);
  }
}


void guard(void*){
  
  long distanceLP;
  long distancePP;
  long distanceP;
  long distanceL;

  while(1){
    
    distanceLP=getDistanceLP();
    distancePP=getDistancePP();
    distanceL=getDistanceL();
    distanceP=getDistanceP();
    digitalWrite(infoPIN, LOW);
  
    
    rtDelay(50);
  //Serial.println(distance);

    while((distanceLP<=40)){
      xSemaphoreTake(binarySemaphore, portMAX_DELAY);
      digitalWrite(infoPIN, HIGH);
      state=2;
      Serial.println("LP:");
      Serial.println(distanceLP);
      rtDelay(500);
      state=1;
      distanceLP=getDistanceLP();
      xSemaphoreGive(binarySemaphore);
    }
    while((distancePP<=40)){
      xSemaphoreTake(binarySemaphore, portMAX_DELAY);
      digitalWrite(infoPIN, HIGH);
      state=2;
      Serial.println("PP:");
      Serial.println(distancePP);
      rtDelay(500);
      state=1;
      distancePP=getDistancePP();
      xSemaphoreGive(binarySemaphore);
    }
    while((distanceP<=40)){
      xSemaphoreTake(binarySemaphore, portMAX_DELAY);
      digitalWrite(infoPIN, HIGH);
      state=2;
      Serial.println("P:");
      Serial.println(distanceP);
      rtDelay(500);
      state=1;
      distanceP=getDistanceP();
      xSemaphoreGive(binarySemaphore);
    }
    while((distanceL<=20)){
      xSemaphoreTake(binarySemaphore, portMAX_DELAY);
      digitalWrite(infoPIN, HIGH);
      state=2;
      Serial.println("L:");
      Serial.println(distanceL);
      rtDelay(500);
      state=1;
      distanceL=getDistanceL();
      xSemaphoreGive(binarySemaphore);
    }
    
  }
}


long getDistanceLP(void){
  long distance;
  long time;
  digitalWrite(triggerPINLP, LOW);
  rtDelay(1);
  digitalWrite(triggerPINLP, HIGH);
  rtDelay(2);
  digitalWrite(triggerPINLP, LOW);

  time = pulseIn(echoPINLP, HIGH);
  distance=time/29/2; //cm
  return distance;
}

long getDistancePP(void){
  long distance;
  long time;
  digitalWrite(triggerPINPP, LOW);
  rtDelay(1);
  digitalWrite(triggerPINPP, HIGH);
  rtDelay(2);
  digitalWrite(triggerPINPP, LOW);

  time = pulseIn(echoPINPP, HIGH);
  distance=time/29/2; //cm
  return distance;
}

long getDistanceP(void){
  long distance;
  long time;
  digitalWrite(triggerPINP, LOW);
  rtDelay(1);
  digitalWrite(triggerPINP, HIGH);
  rtDelay(2);
  digitalWrite(triggerPINP, LOW);

  time = pulseIn(echoPINP, HIGH);
  distance=time/29/2; //cm
  return distance;
}

long getDistanceL(void){
  long distance;
  long time;
  digitalWrite(triggerPINL, LOW);
  rtDelay(1);
  digitalWrite(triggerPINL, HIGH);
  rtDelay(2);
  digitalWrite(triggerPINL, LOW);

  time = pulseIn(echoPINL, HIGH);
  distance=time/29/2; //cm
  return distance;
}

void loop() {}