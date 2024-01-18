#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>
#include<ArduinoJson.h>

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String URL="http://api.thingspeak.com/channels/2404053/fields/1.json?api_key=HO3ECGDS0SRZLTC4&results=1";   //DON'T USE "https:" use only "http"

//              here on above we are sending random  data in constant way

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Start connection");
  WiFi.begin("My_Station","myhome@123");
   Serial.println(" ");
  while((!(WiFi.status()== WL_CONNECTED))){
      delay(200);
     
      Serial.print("..");
    }
  Serial.println("Connected");
}

DynamicJsonDocument doc(768);
void loop(){
  WiFiClient client;
  HTTPClient http;
  int randomdata = random(0,200);
 // String newUrl = URL + String(randomdata);
  http.begin(client,URL);
 
int responsecode = http.GET();
Serial.println(responsecode);
String data = http.getString();

Serial.println(data);

DeserializationError error = deserializeJson(doc, data);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

JsonObject feed :: doc["feeds"].as<JsonArray>()
String led_status = feed["field1"];
Serial.println();
  http.end();
  delay(5000);
}