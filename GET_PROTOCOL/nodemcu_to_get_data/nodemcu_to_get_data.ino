#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>

String URL="http://jsonplaceholder.typicode.com/users/1";   //DON'T USE "https:" use only "http"


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

void loop(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client,URL);
 
int responsecode = http.GET();
Serial.println(responsecode);
String data = http.getString();

Serial.println(data);

  http.end();
  delay(5000);
}