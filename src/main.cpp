#include <Arduino.h>
#include <Bounce2.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// #include "baramee.h"
#define GREEN 33
#define RED 26
#define YELLOW 25
#define LDR 34
TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;
int status_room1 = 0;   // standard
int status_room2 = 0;   // standard
int status_room3 = 0;   // standard
int value_light1 = 255; // standard
int value_light2 = 255; // standard
int value_light3 = 255; // standard
int auto_light1 = 1;    // standard
int auto_light2 = 0;    // standard
int auto_light3 = 0;    // standard
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
int cnt = 0;

const String baseUrl = "http://group8.exceed19.online/";

void Connect_Wifi()
{
  const char *ssid = "M";
  const char *password = "leesoome123";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}

void GET_room()
{
  DynamicJsonDocument doc(2048);
  const String url = "https://ecourse.cpe.ku.ac.th/exceed08/rooms";
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();
  Serial.print("GET");
  if (httpResponseCode >= 200 && httpResponseCode < 300)
  {
    Serial.print("HTTP ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    deserializeJson(doc, payload);
    status_room1 = doc["room1"]["state"].as<int>();
    status_room2 = doc["room2"]["state"].as<int>();
    status_room3 = doc["room3"]["state"].as<int>();
    value_light1 = doc["room1"]["brigthness"].as<int>();
    value_light2 = doc["room2"]["brigthness"].as<int>();
    value_light3 = doc["room3"]["brigthness"].as<int>();
    auto_light1 = doc["room1"]["is_auto"].as<int>();
    auto_light2 = doc["room2"]["is_auto"].as<int>();
    auto_light3 = doc["room3"]["is_auto"].as<int>();
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void POST_update_room()
{
  String json;
  DynamicJsonDocument doc(2048);
  doc["room1"]["state"] = status_room1;
  doc["room2"]["state"] = status_room2;
  doc["room3"]["state"] = status_room3;
  doc["room1"]["brightness"] = value_light1;
  doc["room2"]["brightness"] = value_light2;
  doc["room3"]["brightness"] = value_light3;
  doc["room1"]["is_auto"] = auto_light1;
  doc["room2"]["is_auto"] = auto_light2;
  doc["room3"]["is_auto"] = auto_light3;

  serializeJson(doc, json);

  const String url = "http://group8.exceed19.online/update_rooms";
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);
  if (httpResponseCode >= 200 && httpResponseCode < 300)
  {
    Serial.print("DONE");
    Serial.println();
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void gotTouch()
{
  touch1detected = true;
}

void gotTouch1()
{
  touch2detected = true;
}

int poyT1 = 0, countT1 = 0;
int poyT2 = 0, countT2 = 0;

void Touch1()
{
  debouncer.update();
  if (debouncer.fell())
  {
    status_room1 = !status_room1;
  }
}

void Touch2()
{
  if (touchRead(T2) <= 20 && poyT1 != 1)
  {
    if (countT1 == 0)
    {
      Serial.println(touchRead(T2));
      status_room2 = 255;
      countT1++;
      poyT1 = !poyT1;
    }
  }
  if (touchRead(T2) > 20)
  {
    countT1 = 0;
  }
  if (touchRead(T2) <= 20 && poyT1 == 1)
  {
    if (countT1 == 0)
    {
      Serial.println(touchRead(T2));
      status_room2 = 0;
      countT1++;
      poyT1 = !poyT1;
    }
  }
}

void Touch3()
{
  if (touchRead(T3) <= 20 && poyT2 != 1)
  {
    if (countT2 == 0)
    {
      status_room3 = 255;
      countT2++;
      poyT2 = !poyT2;
    }
  }
  if (touchRead(T3) > 20)
  {
    countT2 = 0;
  }
  if (touchRead(T3) <= 20 && poyT2 == 1)
  {
    if (countT2 == 0)
    {
      status_room3 = 0;
      countT2++;
      poyT2 = !poyT2;
    }
  }
}

void Senlight(void *param)
{
  while (1)
  {
    if (!auto_light1)
    {
      Touch1();
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    if (!auto_light2)
    {
      Touch2();
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    if (!auto_light3)
    {
      Touch3();
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    sensor_LDR = map(analogRead(LDR), 1250, 4096, 0, 255);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void GET_POST(void *param)
{
  while (1)
  {
    GET_room();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    POST_update_room();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  Connect_Wifi();
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
  sensor_LDR = map(analogRead(LDR), 0, 4095, 0, 255);
  Serial.println(analogRead(LDR));
  xTaskCreatePinnedToCore(Senlight, "Senlight", 1000, NULL, 1, &TaskA, 0);
  xTaskCreatePinnedToCore(GET_POST, "GET_POST", 10240, NULL, 1, &TaskB, 1);
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
    if (sensor_LDR <= 50)
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
    if (sensor_LDR <= 50)
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
    if (sensor_LDR <= 50)
    {
      ledcWrite(2, value_light3); // on room3
    }
    else
    {
      ledcWrite(2, 0); // off room3
    }
    delay(5);
  }
}
