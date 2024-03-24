#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "BORD NOT FOUND"
#endif

#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Ticker.h>

#define Led1 4
#define Led2 15


const int trigPin = 12;
const int echoPin = 13;
long duration ;
int distanceCm;

//---------------------------------------------------------------------------------------------------------
//                                   H T M L__C O D E
//---------------------------------------------------------------------------------------------------------
Ticker timer;

char webpage[] PROGMEM = R"=====(


<!DOCTYPE html>
<html>
<body>

<script>

var connection = new WebSocket('ws://'+location.hostname+':81/');

var button_1_status = 0 ;
var button_2_status = 0 ;

var hum_data = 0

connection.onmessage = function(event){

    var full_data = event.data;
    console.log(full_data);
    var data = JSON.parse(full_data);
    temp_data = data.distance;

    document.getElementById("temp_meter").value = temp_data;


}

function button_1_on()
{
button_1_status = 1;
console.log("LED 1 IS ON");
send_data();
}

function button_1_off()
{
button_1_status = 0;
console.log("LED 1 IS OFF");
send_data();
}

function button_2_on()
{
button_2_status = 1;
console.log("LED 2 IS ON");
send_data();
}

function button_2_off()
{
button_2_status = 0;
console.log("LED 2 IS OFF");
send_data();
}


function send_data()
{

  var full_data = '{"LED1":' + button_1_status +',"LED2":'+ button_2_status +'}';



connection.send(full_data);
}

</script>

<center>

<h1>MY HOME AUTOMATON </h1>

<h3>LED_1</h3>
<button onclick="button_1_on()"> on </button>
<button onclick="button_1_off()"> off </button>


<h3>LED_2</h3> 
<button onclick="button_2_on()"> on </button>
<button onclick="button_2_off()"> off </button>
<br>


<h3>TEMPERATURE</h3>
</center>
<div style="text-align: center;">
<meter value="50" min="0" max="100" id="temp_meter">
</meter>
</a>


</body>
</html>


)=====";

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80); //server port 80
WebSocketsServer websockets(81);


void notFound(AsyncWebServerRequest *request){
request->send(404,"text/plain","not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);


      DynamicJsonDocument doc(200);

      DeserializationError error = deserializeJson(doc,message);

      if(error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
      }

   

      int LED1_status = doc["LED1"];
      int LED2_status = doc["LED2"];
      digitalWrite(Led1,LED1_status);
      digitalWrite(Led2,LED2_status);
  }
}

void setup(void) {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(Led1,OUTPUT);
pinMode(Led2,OUTPUT);

WiFi.softAP("darkweb","darkweb@123");
Serial.println("softtap");
Serial.println("");
Serial.println(WiFi.softAPIP());

server.on("/",[](AsyncWebServerRequest * request){

request->send_P(200,"text/html",webpage);
});


server.onNotFound(notFound);

server.begin();
websockets.begin();
websockets.onEvent(webSocketEvent);

pinMode(trigPin,OUTPUT);
pinMode(echoPin,INPUT);

timer.attach(0.5,data);
}

void loop(void) {
  // put your main code here, to run repeatedly:
 websockets.loop();
 
}

void data()
{
  digitalWrite(trigPin,LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin,HIGH);
 delayMicroseconds(2);
 digitalWrite(trigPin,LOW);

 duration = pulseIn(echoPin,HIGH);
 distanceCm = duration*0.034/2;
 Serial.println(distanceCm);

 String JSON_Data = "{\"distance\":";
        JSON_Data += distanceCm;
        JSON_Data += "}";
  
  Serial.println(JSON_Data);
  websockets.broadcastTXT(JSON_Data);
}
