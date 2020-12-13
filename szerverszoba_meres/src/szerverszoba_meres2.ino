const int refresh=30; // Webhely frissítési gyakoriság
#include "dht.h"
#define DHTPIN 5     // PIN 5 GPIO5 D1
dht DHT;
float tValue;
float hValue;// 
// ****** DHT settings end (Robojax.com )


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Kati_router" // Your WiFi SSID
#define STAPSK  "papo1234" //Your WiFi password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


void sendTemp() {
  String page = "<!DOCTYPE html>\n\n";
  
  page +="    <meta http-equiv='refresh' content='";
  page += String(refresh);// Milyen gyakran frissüljön be a webhely
  page +="'/>\n"; 
  page +="<html>\n";
  page +="<head>\n";
  page +="       <meta charset='UTF-8'>\n";
  page +="</head>\n";
  page +="<body>\n"; 
  page +="<h1>Tamási Zoltán Roland (DE-IK)</h1>\n";  
  page +="<p style=\"font-size:50px;\">Aktuális hőmérséklet<br/>\n"; 
  page +="<p style=\"color:green; font-size:50px;\">";
  page += String(tValue, 2);
  page +=" °C</p>\n";
  page +="<p style=\"font-size:50px;\">Aktuális páratartalom<br/>\n"; 
  page +="<p style=\"color:blue; font-size:50px;\">";
  page += String(hValue, 2);
  page +=" %</p>\n";
  page +="</body>\n"; 
  page +="</html>\n"; 
 server.send(200,  "text/html",page);
 Serial.println("Szervernek elküldve az új mérési adatok!");
 delay(1000);

}

void handleNotFound() {
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST"; // Ha a kérés típusa GET, akkor GET, amúgy meg POST kérés lesz.
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}


IPAddress local_IP(192, 168, 0, 100); // "Fix" IP-cím beállítása a WiFi csatlakozáshoz.
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void setup(void) {
  Serial.begin(115200); // vagy 9600 baud ráta
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA Failed to configure"); }

  
  WiFi.mode(WIFI_STA);
  

  server.on("/", sendTemp); // Szerverbe az elkészített HTML kód küldése HTTP kéréssel
  

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  
   WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) { // Amíg nem sikerül csatlakozni, addig újrapróbálkozás
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    uint8_t macAddr[6];
    WiFi.macAddress(macAddr);
    Serial.printf("MAC cím: %02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    
  Serial.println("");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP-cím: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxDHT")) {
    Serial.println("MDNS responder elindult.");
  }
  
  server.handleClient();
  MDNS.update();
  Serial.println("Adatok begyűjtése , leolvasás a szenzorról.");
  delay(2000);
  int readData = DHT.read22(DHTPIN); // Adatolvasás a megfelelő adatcsatornáról
  float t = DHT.temperature;// Read temperature as Celsius (the default)
  delay(500);
  readData = DHT.read22(DHTPIN);
  float h = DHT.humidity;// Reading humidity 
  delay(500);
  if ((isnan(h) || ((h < 20.0) || (h > 90))) || (isnan(t) || ((t < 10.0) || (t > 40)))) // Rossz értékek kiszűrése
                 {
                     Serial.println("A szenzorról olvasás sikertelen volt.");
                      return;
                 }
 
     Serial.print("Aktuális hőmérséklet: ");
    Serial.println(t);
     Serial.print("Aktuális páratartalom: ");
    Serial.println(h);
    Serial.println("--------------------------");

 
 tValue =t;
 hValue = h;
  
 // float f = DHT.temperature(true);// Alapértelmezetten Celsius true paraméter átadásánál Fahrenheit
    
  
  delay(1000);
}
}
