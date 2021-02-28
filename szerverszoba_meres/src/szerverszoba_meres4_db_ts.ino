#include <ESP8266WiFi.h> // Wifis ESP könyvtára
#include "dht.h" // DHT11 szenzor könyvtára
#include "ThingSpeak.h"
#include "secrets.h"
#include <MySQL_Connection.h> // ARDUINO --> MYSQL kapcsolodast megvalosito konyvtar
#include <MySQL_Cursor.h>

char ssid[] = SECRET_SSID;   // SSID 
char pass[] = SECRET_PASS;   // HALOZATI JELSZO
unsigned long myChannelNumber = SECRET_CH_ID;  // CSATORNASZAM (ThingSpeak)
const char * myWriteAPIKey = SECRET_WRITE_APIKEY; // IRASRA API KULCS (ThingSpeak)

IPAddress server_addr(204,2,63,34); // IP CIM (adatbazishoz)
 char user[] = SECRET_USER; // MYSQL felhasznalonev
 char password[] = SECRET_PWD; // MYSQL jelszo a bejelentkezeshez
 char INSERT_SQL[] = "INSERT INTO  dht11_database.dht11_sensors(homerseklet, paratartalom) VALUES ('%s','%s')"; // MYSQL INSERT parancs, melyik adatbazis, melyik tabla, milyen ertekeket

#define DHTPIN 5 // PIN , ahova a szenzor csatlakozik
 
dht DHT; // Szenzor objektum
 
WiFiClient client; // Wi-Fi kliens objektum

MySQL_Connection conn(&client); // MYSQL csatlakozás a WiFi kliens segítségével
MySQL_Cursor* cursor; // MYSQL kurzor a muveletek elvegzesehez.

void setup() // Beuzemelesi fazis
{
       ThingSpeak.begin(client); // ThingSpeak kapcsolat felepitese
       Serial.begin(115200);
 
      // WiFi.begin(ssid, pass); // WiFi kapcsolat felepitese
}

void sendDataToThingSpeak(float data1, float data2) { // ket lebego pontos ertek atadasa (homerseklet, paratartalom)
  ThingSpeak.setField(1, data1); // field1-be homerseklet
  ThingSpeak.setField(2, data2); // field2-be paratartalom
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // channel szam, irasra API kulcs
  if (x == 200) { // HTTP OK
    Serial.println("Homersekleti ertekek frissitve a weboldalon.");
  }
  else {
    Serial.println("Problema lepett fel az adatok frissitese kozben. HTTP kod: " + String(x));
  }
}

void readingDatas() { // Szenzorrol olvasasi folyamat
  
 int readData = DHT.read22(DHTPIN); // Adatok leolvasása a szenzorról
      Serial.println(readData);
      float h = DHT.humidity; // Szenzor által mért páratartalom értékének eltárolása egy h nevű flot típusú változóba
      Serial.println(h);
      delay(1000);
      readData = DHT.read22(DHTPIN);
float t = DHT.temperature; // Szenzor által mért hőmérséklet értékének eltárolása egy t nevű flot típusú változóba
      Serial.println(t);

       // Rossz értékek kiszűrése
       while ((isnan(h) || ((h < 20.0) || (h > 90))) || (isnan(t) || ((t < 10.0) || (t > 40)))) {
          Serial.println("A szenzorról olvasás sikertelen volt.");
         readingDatas(); // Adatok beolvasása rekurzív hívás
}
  // ThingSpeak szerver felé adatok továbbküldése
  sendDataToThingSpeak(t, h); // ThingSpeak szerver felé az adatok elküldése

}

void WiFiConnecting() { // WiFi kapcsolodasi folyamat
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
      }

}

void MySQLConnection() { // MYSQL kapcsolodasi folyamat
      Serial.print("Csatlakozas a tavoli MYSQL szerverhez...");
     while (!conn.connect(server_addr, 19968, user, password)) {
      // connect to the database
       delay(1000);
    Serial.println("-?");   
     }
     
    Serial.println("OK.");
      cursor = new MySQL_Cursor(&conn);
}

void SendDataToDatabase() { // Adatbazis fele adatkuldesi folyamat
  char buff[128]; // Szenzor adatok tarolasa egy bufferben.
                  char temp[4];      
                  char hum[5];    

                  dtostrf(DHT.temperature,2,1,temp); // lebego pontos ertekek konvertalasa sztringge.
                  dtostrf(DHT.humidity,3,1,hum); // lebego pontos ertekek konvertalasa sztringge.
                  sprintf(buff, INSERT_SQL, temp, hum); // ADATOK elokeszitese az adatbazisba beszurashoz.
                                     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // MYSQL kurzor peldany
                                     cur_mem->execute(buff); // A kiolvasott ertekek beszurasa az adatbazisba (vegrehajtas)
                                     Serial.println("Leolvasas a szenzorrol es betoltes az adatbazisba.");
                                     delete cur_mem; // MYSQL kurzor peldany torlese a vegrehajtas utan.

                        delay(500);
                          
}

void loop() // Utasitasok ismetlese amig van tapfeszultseg
{
  WiFiConnecting(); // WiFi kapcsolodasi folyamat
  readingDatas(); // Szenzorrol olvasasi folyamat
  MySQLConnection(); // MYSQL kapcsolodasi folyamat
  SendDataToDatabase(); // Adatbazis fele adatkuldesi folyamat
  Serial.println("Várakozás a következő beolvasásig...");
  
  // ThingSpeak-nek szüksége legalább 15 másodperces késleltetésre az adatok frissítéséhez.
  delay(55000); // Nagyjabol 3 percenkenti adatkuldest valosit meg. (180-185 mp)
}
