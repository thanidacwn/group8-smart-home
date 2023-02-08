#include <Arduino.h>
#include <Bounce2.h>
// #include "baramee.h"
#define GREEN 33
#define RED 26
#define YELLOW 25
#define LDR 34
TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;
int status_room1 = 0;
int status_room2 = 0;
int status_room3 = 0;
int value_light1 = 255;
int value_light2 = 255;
int value_light3 = 255;
int auto_light1 = 0;
int auto_light2 = 0;
int auto_light3 = 0;
int sensor_LDR;
#define BUTTON 27
Bounce debouncer = Bounce();
int SW_status = 0;
int i = 255;
int touch;
int count = 0;
int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;

void gotTouch()
{
  touch1detected = true;
}

void gotTouch1()
{
  touch2detected = true;
}

int poyT1=0,countT1=0;
int poyT2=0,countT2=0;
void Touch2()
{ 
  if (touchRead(T2) <= 20 && poyT1!=1)
  {
    if (countT1 == 0)
    {
      Serial.println(touchRead(T2));
      status_room2=255;
      countT1++;
      poyT1=!poyT1;
    }
    countT1++;
  }
  if(touchRead(T2) > 20 ){
    
     poyT1=!poyT1;
     countT1=0;
  }
  else if (touchRead(T2) <= 20  && poyT1==1)
  {
    if (countT1 == 0)
    {
      Serial.println(touchRead(T2));
      status_room2=0;
      countT1++;
      poyT1=!poyT1;
    }
    countT1++;
  }
}

void Touch3()
{ 
  if (touchRead(T3) <= 20 && poyT2!=1)
  {
    if (countT2 == 0)
    {
      Serial.println("NO");
      status_room3=255;
      countT2++;
      poyT2=!poyT2;
    }
    countT2++;
  }
  if(touchRead(T3) > 20 ){
    
     poyT2=!poyT2;
     countT2=0;
  }
  else if (touchRead(T3) <= 20  && poyT2==1)
  {
    if (countT2== 0)
    {
      Serial.println("yes");
      status_room3=0;
      countT2++;
      poyT2=!poyT2;
    }
    countT2++;
  }
}

void Senlight(void *param)
{
  while (1)
  {   Touch2();
      vTaskDelay( 1 /portTICK_PERIOD_MS);
      Touch3();
      vTaskDelay( 1 /portTICK_PERIOD_MS);
      sensor_LDR=map(analogRead(LDR), 1250, 4096, 0, 255);
      vTaskDelay( 1 /portTICK_PERIOD_MS);
  }
}


void GET_POST(void *param)
{
  while (1)
  {


  }
}

void setup()
{
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(GREEN, 0);
  ledcAttachPin(YELLOW, 1);
  ledcAttachPin(RED, 2);
  touchAttachInterrupt(T2, gotTouch, threshold);
  touchAttachInterrupt(T3, gotTouch1, threshold);
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(27);
  sensor_LDR=map(analogRead(LDR), 0, 4095, 0, 255);
  Serial.println(analogRead(LDR));
  xTaskCreatePinnedToCore(Senlight, "Senlight", 1000, NULL, 1, &TaskA, 0);
  xTaskCreatePinnedToCore(GET_POST, "GET_POST", 1000, NULL, 1, &TaskB, 1);
}

void loop()
{
  if (!auto_light1)
    {
      if (status_room1)
      {
        ledcWrite(0, value_light1); // on room1
      }
      else
      {
        ledcWrite(0, 0); // off room1
      }
      delay(5);
    }
    else if (auto_light1) // mode auto room1
    {
      if (sensor_LDR <= 20)
      {
        ledcWrite(0, value_light1); // on room1
      }
      else
      {
        ledcWrite(0, 0); // off room1
      }
      delay(5);
    }

    if (!auto_light2)
    {
      if (status_room2)
      {
        ledcWrite(1, value_light2); // on room2
      }
      else
      {
        ledcWrite(1, 0); // off room2
      }
      delay(5);
    }
    else if (auto_light2) // mode auto room2
    {
      if (sensor_LDR <= 20)
      {
        ledcWrite(1, value_light2); // on room2
      }
      else
      {
        ledcWrite(1, 0); // off room2
      }
      delay(5);
    }
    if (!auto_light3)
    {
      if (status_room3)
      {
        ledcWrite(2, value_light3); // on room3
      }
      else
      {
        ledcWrite(2, 0); // off room3
      }
      delay(5);
    }
    else if (auto_light3) // mode auto room3
    {
      if (sensor_LDR <= 20)
      {
        ledcWrite(2, value_light3); // on room3
      }
      else
      {
        ledcWrite(2, 0); // off room3
      }
      delay(5);
    }
    Touch2();
    Touch3();
  
}