#include <stdlib.h>
//#include <DHT.h>
#include <ESP8266WiFi.h>
#include <dht.h> 
#define dataPin 5
 
//DHT dht(2, DHT11);
dht DHT;

WiFiClient client;

String apiKey = "5DPPFJ2NIPX7QGND";
 
void setup() {
  Serial.begin(115200);
  Serial.println("AT");
  delay(5000);
  Serial.println("AT+CWMODE=1");
  delay(2000);
  Serial.println("AT+CWJAP=\"Kati_router\",\"papo1234\""); 
  delay(5000);
}
 
void loop(){
  Serial.println("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80"); 
  delay(2000);
  int readData = DHT.read22(dataPin);
  char buffer[10];
  String temperatureC = dtostrf(DHT.temperature, 4, 1, buffer);
  Serial.println(temperatureC);
  delay(500);
  readData = DHT.read22(dataPin);
  int humidity = DHT.humidity; 
  Serial.println(humidity);
  //String cmd = "http://184.106.153.149/update?api_key=5DPPFJ2NIPX7QGND&field1=" + temperatureC + "&field2=" + String(humidity) + "\r\n";
  client.send("GET https://api.thingspeak.com/update?api_key=5DPPFJ2NIPX7QGND&field1=5");
  //Serial.println(cmd);
  Serial.print("AT+CIPSEND=");
  //Serial.println(cmd.length());
  delay(2000);
  Serial.print(cmd);
  delay(60000);
}
