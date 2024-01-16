void setup() {
  // put your setup code here, to run once:

}
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String URL="http://api.thingspeak.com/update?api_key=PWDVVUVCNVNV1KFN&field1=200";   //DON'T USE "https:" use only "http"

//              here on above we are sending 200 as data in constant way

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
void loop() {
  // put your main code here, to run repeatedly:

}
