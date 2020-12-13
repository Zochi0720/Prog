#include <ESP8266WiFi.h> // Wifis ESP könyvtára
#include "dht.h" // DHT11 szenzor könyvtára
 
String apiKey = "5DPPFJ2NIPX7QGND";     //  API KULCS írásra a Thingspeak-től
 
const char *ssid =  "Kati_router";     // WIFI SSID
const char *pass =  "papo1234";        // WIFI JELSZÓ
const char* server = "api.thingspeak.com";
String channelID = "1248968";
 
#define DHTPIN 5 // PIN
 
dht DHT; // Szenzor objektum
 
WiFiClient client; // Wi-Fi kliens objektum
void setup() 

{

       Serial.begin(115200);
 
      // WiFi.begin(ssid, pass);c

        Serial.println("");
  ("WiFi-re kapcsolódás folyamatban");
  
  
      

      
      

 
}
 
void loop() 
{

     WiFi.begin(ssid,pass);
      while (WiFi.status() != WL_CONNECTED) 
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
      
      
      
      int readData = DHT.read22(DHTPIN); // Adatok leolvasása a szenzorról
      float h = DHT.humidity; // Szenzor által mért páratartalom értékének eltárolása egy h nevű flot típusú változóba
      delay(1000);
      readData = DHT.read22(DHTPIN);
      float t = DHT.temperature;
      
              
                        delay(500);

                        
 
                         if (client.connect(server,80))   //   "184.106.153.149" vagy api.thingspeak.com is megadható
                      {     
                            //Serial.println("Csatlakozva a szerverre.");
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n"); // POST kérés küldése a szerver felé.
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length()); // A kérés hossza.
                             client.print("\n\n");
                             client.print(postStr);
 
                             
 
                             Serial.print("Hőmérséklet: ");
                             Serial.print(t);
                             Serial.print(" Celsius-fok, Páratartalom: ");
                             Serial.print(h);
                             Serial.println("%. Adatok küldése a szervernek...");
                        }
          client.stop();
 
          Serial.println("Várakozás a következő beolvasásig...");
  
  // ThingSpeak-nek szüksége legalább 15 másodperces késleltetésre az adatok frissítéséhez.
  delay(13000); // Két másodperc késleltetés van már összesen a folyamatok között.
      }
      
}
