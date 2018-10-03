#ifdef ESP8266//Se estiver usando ESP8266, automáticamente irá adicionar as bibliotecas do ESP8266.
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#elif defined ESP32//Se estiver usando ESP32, fara a mesma operaçao.
#include <WiFi.h>
#endif
// WiFi Definitions
const char* ssid = "Node";
const char* password = "Esp8266Test"; // has to be longer than 7 chars
const char* value = "";
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);
 


int redledPin = D6; // GPIO6
int yellowledPin=D7; // GPIO7
WiFiServer server(80);

void setup() {

   Serial.begin(115200);
   WiFi.mode(WIFI_AP);
   WiFi.softAP(ssid, password, 1, 0.1);
   Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
   server.begin();
   pinMode(redledPin, OUTPUT);
   pinMode(yellowledPin, OUTPUT);
   digitalWrite(redledPin, LOW);
   digitalWrite(yellowledPin, LOW);
}

void loop() {
  // Check of client has connected
  WiFiClient client = server.available();
  if(!client) {
    return;
  }

  // Read the request line
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // Match request
  if(request.indexOf("/led/red/on") != -1) {
     digitalWrite(redledPin, HIGH);
     value = "redon";
  } else if (request.indexOf("/led/red/off") != -1) {
     digitalWrite(redledPin, LOW);
      value = "redoff";
  }
  else if (request.indexOf("/led/yellow/on") != -1) {
     digitalWrite(yellowledPin, HIGH);
      value = "yellowon";
  }
  else if (request.indexOf("/led/yellow/off") != -1) {
     digitalWrite(yellowledPin, LOW);
      value = "yellowoff";
  }
  client.flush();
   
  // JSON response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: application/json\r\n\r\n";
  s += "{\"data\":{\"message\":\"success\",\"value\":\"";
  s += value;
  s += "\"}}\r\n";
  s += "\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected when the function returns and the client object is destroyed
}
