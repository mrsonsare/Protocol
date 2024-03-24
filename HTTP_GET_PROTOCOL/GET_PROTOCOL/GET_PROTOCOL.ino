
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "";
const char* password = "";

 WiFiClient client;
 HTTPClient http;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("HII MR.SONSARE");
  setup_wifi();

}

void loop() {
  // put your main code here, to run repeatedly:

if(WiFi.status() == WL_CONNECTED){
  Serial.println("\n\n PERFORMING HTTP REQUEST \n");
}
String requestURL = "http://jsonplaceholder.typicode.com/users/1";

http.begin(client,requestURL);


String body = "{\name\":\"PUT MR.SONSARE\"}";


int httpRespondCode = http.PUT(body);
Serial.print("HTTP Respond Code : ");
Serial.println(httpRespondCode);
if (httpRespondCode == 200) {
String payload = http.getString();
  
  Serial.println("Response payload : " + payload);
}
delay(5000);
 http.end();
}


void setup_wifi() {
 WiFi.begin("My_Station","myhome@123");

while(WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(200);
}
Serial.println();
Serial.println("NodeMCU is Connected !");
Serial.println(WiFi.localIP()); 
}