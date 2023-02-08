#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>


const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app";



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
  const String url = baseUrl + "/getroom";
  http.begin(url);

  Serial.println("get room");
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);
    JsonArray all = doc["all_traffic"].as<JsonArray>();
    for (JsonObject a: all){

    }
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void POST_update_room(String led)
{
  const String url = baseUrl + "/update_room";
  String json;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  DynamicJsonDocument doc(2048);

 // value in json
  doc["room"] = "td7vz";
  doc["room"] = led;
  
  serializeJson(doc, json); // convert Dynamic ---> string json

  Serial.println("POST " + led);
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