#include <dht.h> // DHT szenzor könyvtára
#include <SPI.h>
#include <Wire.h>

//For webserver
#include <ESP8266WiFi.h> //WemOS NODE-MCU
#include <WiFiClient.h> // WifiClient könyvtára
#include <ESP8266WebServer.h> 
#include <ESP8266mDNS.h>

const char* ssid = "Kati_router"; // Wi-Fi SSID
const char* password = "papo1234"; // Wi-Fi jelszó

ESP8266WebServer server(80); // WEBSERVER PORTTAL

#define dataPin 5 // Amelyik PIN-ről az adatokat lekérjük.
dht DHT; // DHT szenzor objektum

  

void handleRoot() {
  
  int readData = DHT.read22(dataPin); // Adatolvasás a szenzorról
  float t = DHT.temperature; // A leolvasott hőmérsékleti érték eltárolása
  Serial.print("Jelenlegi hőmérséklet: ");
  Serial.print(t);
  Serial.println(" *C ");
  
  delay(500); // 500 ms

  readData = DHT.read22(dataPin); //Adatolvasás a szenzorról
  float h = DHT.humidity; // A leolvasott páratartalom érték eltárolása
  delay(500);
  if ((isnan(h) || ((h < 20.0) || (h > 90))) || (isnan(t) || ((t < 10.0) || (t > 40)))) // Hubás értékek kiszűrése
                 {
                     Serial.println("A szenzorról olvasás sikertelen volt.");
                      return;
                 }
  Serial.print("Jelenlegi páratartalom: ");
  Serial.print(h);
  Serial.println(" % ");

  // MUNIN számára fenntartott mappastruktúra
  String s = "{\"temperature\":";
  s += t;
  
  s += ",\"humidity\":"; 
  s += h;
  s +="}";

  //{"temperature":21,"location":"server_room_1"}
  //{"temperature":21,"hummidity":60} --- ez a cel

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(250);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(250); 

  server.send(200, "application/json", s);
}

IPAddress local_IP(192, 168, 0, 100);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional



void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA Failed to configure"); }
 
  
WiFi.begin(ssid, password);

  // Várakozás a kapcsolódásra:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP-cím: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder elindult.");
  }

  

  server.on("/", handleRoot);
  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, HIGH); //Forditott Logika
}

// the loop function runs over and over again forever
void loop() {
/*
WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Csatlakozva a következő SSID-hoz: ");
  Serial.println(ssid);
  Serial.print("IP-cím: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder elindult.");
  }

*/
 /*handleRoot();*/


  /*server.on("/", handleRoot);
  server.onNotFound(handleRoot);

  server.begin();*/
  



  server.handleClient();
  
   delay(2000); 

}
