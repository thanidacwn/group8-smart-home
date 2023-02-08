#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>


const String baseUrl = "";

int britgthness1;
int is_auto1;
int state1;

int britgthness2;
int is_auto2;
int state2;

int britgthness3;
int is_auto3;
int state3;

void Connect_Wifi()
{
  const char *ssid = "name";
  const char *password = "password";
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
  DynamicJsonDocument doc(65536);

  HTTPClient http;
  const String url = baseUrl + "/web_data";
  http.begin(url);

  Serial.println("get room");
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);
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
  const String url = baseUrl + "/update_web_data";
  String json;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  DynamicJsonDocument doc(2048);

  
 // value in json
  doc["room1"]["state"] = state1;
  doc["room1"]["brigthness"] = britgthness1;
  doc["room1"]["is_auto"] = is_auto1;

  doc["room2"]["state"] = state2;
  doc["room2"]["brigthness"] = britgthness2;
  doc["room2"]["is_auto"] = is_auto2;

  doc["room3"]["state"] = state3;
  doc["room3"]["brigthness"] = britgthness3;
  doc["room3"]["is_auto"] = is_auto3;
  
  
  serializeJson(doc, json);

  int httpResponseCode = http.POST(json);
  if (httpResponseCode == 200)
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