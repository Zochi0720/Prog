#include <dht.h> // DHT szenzor könyvtára (DHT11)
#include <SPI.h>
#include <Wire.h>

// Szükséges könyvtárak
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>

#define DATAPIN 5

dht DHT;
String te;
String hu;
#define DHTPIN 5
const char* ssid = "Kati_router";
const char* password = "papo1234";
AsyncWebServer server(80);





// Szenzorértékek megtartása
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return te;
  }
  else if(var == "HUMIDITY"){
    return hu;
  }
  return String();
}

// "FIX" IP-cím beállítása a csatlakozáshoz

IPAddress local_IP(192, 168, 0, 100); // Változtatni lehet
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1); // Default Gateway az adott routernek
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional



void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200); // baud ráta

   //pinMode(LED_BUILTIN, OUTPUT);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA-t nem sikerült bekonfigurálni."); }
  

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }



  // Csatlakozás a WiFi hálózathoz
  WiFi.begin(ssid, password); // Wifi-adatok
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi-re való csatlakozás folyamatban...");
  }

  Serial.print("Sikeres csatlakozás a következő hálózati IP-címre: ");
  Serial.println(WiFi.localIP());

    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){ // Aszinkronos webszerverre GET KÉRÉS
    request->send_P(200, "text/plain", te.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", hu.c_str());
  });

  // Start server
  server.begin();



}


void loop(void) {
  // Robojax.com code for ESP8266 and DHT11 DHT22
//  server.handleClient();
  //MDNS.update();
  digitalWrite(LED_BUILTIN, LOW); // felvillanó led a lapkán
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  
   WiFi.begin(ssid,password);
      while (WiFi.status() != WL_CONNECTED) // Amíg nem sikerül csatlakozni a WiFi hálózathoz:
     {
             
            delay(500);
            Serial.print(".");
            
     }
      
      
     Serial.println("");
     if (WiFi.status() == WL_CONNECTED) {

      Serial.print("SSID: ");
      Serial.print(ssid);
      Serial.print(", IP-cím: ");
      Serial.println(WiFi.localIP());


  
  int readData = DHT.read22(DHTPIN);
  float t = DHT.temperature; // Az alapértelmezett beállítás Celsius
  delay(1000);
  readData = DHT.read22(DHTPIN);
  float h = DHT.humidity;
  //delay(500);
 // float f = DHT.temperature(true);// Ha a zárójelben true értéket adunk meg akkor Fahrenheit-ben mér.
   // Serial.println(c);
    //Serial.println(h);
    if ((isnan(h) || ((h < 20.0) || (h > 90))) || (isnan(t) || ((t < 10.0) || (t > 40)))) // Rossz értékek kiszűrése
                 {
                     Serial.println("A szenzorról olvasás sikertelen volt.");
                      return;
                 }
 te = String(t);
 hu = String(h);

    
 Serial.print("Hőmérséklet: ");
 Serial.println(te);
 Serial.print("Páratartalom: ");
 Serial.println(hu);
 Serial.println("GET kérés elküldve a webszerver felé.");
 Serial.println("**********************");
 }
  delay(60000); // Egy perc késleltetés
  Serial.println("Szenzorról olvasás megkezdődött.");
}
