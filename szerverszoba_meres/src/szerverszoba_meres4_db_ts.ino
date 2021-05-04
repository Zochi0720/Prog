#include <ESP8266WiFi.h> // Wifis ESP könyvtara
#include "dht.h" // DHT11 szenzor könyvtara
#include "ThingSpeak.h"
#include "secrets.h"
#include <MySQL_Connection.h> // ARDUINO --> MYSQL kapcsolodast megvalosito konyvtar
#include <MySQL_Cursor.h>

char * ssid = SECRET_SSID;   // SSID 
char * pass = SECRET_PASS;   // HALOZATI JELSZO
unsigned long myChannelNumber = SECRET_CH_ID;  // CSATORNASZAM (ThingSpeak)
char * myWriteAPIKey = SECRET_WRITE_APIKEY; // IRASRA API KULCS (ThingSpeak)
byte k = 1; // probalkozasok szama a ThingSpeak eleresere (gyakori hibakodok miatt volt ra szukseg) // csere
bool v = false;
float datat = 0.0;
float datah = 0.0;
IPAddress server_addr(204,2,63,34); // IP-CIM (adatbazishoz)
IPAddress local_IP(192, 168, 0, 58); // Lokalis IP-cim , amit statikusan hasznalunk.
IPAddress gateway(192, 168, 0, 1); // alapertelmezett atjaro
IPAddress subnet(255, 255, 255, 0); // alhalozati maszk
IPAddress primaryDNS(8, 8, 8, 8); // opcionalis
IPAddress secondaryDNS(8, 8, 4, 4); // opcionalis

char * user = SECRET_USER; // MYSQL felhasznalonev
char * password = SECRET_PWD; // MYSQL jelszo a bejelentkezeshez
char * INSERT_SQL = "INSERT INTO  dht11_database.dht11_sensors(homerseklet, paratartalom) VALUES ('%s','%s')"; // MYSQL INSERT parancs, melyik adatbazis, melyik tabla, milyen ertekeket

#define DHTPIN 5 // PIN , ahova a szenzor csatlakozik 5
// #define DHTPIN 4 // PIN, ahova a szenzor csatlakozik
 
dht DHT; // Szenzor objektum
 
WiFiClient client; // Wi-Fi kliens objektum

MySQL_Connection conn((Client *)&client); // MYSQL csatlakozas a WiFi kliens segitsegevel
MySQL_Cursor* cursor; // MYSQL kurzor a muveletek elvegzesehez.

void setup() // Beuzemelesi fazis
{
  Serial.begin(9600); // NodeMCU javasolt beallitasa miatt. (baud)
  delay(20);

      WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS); // Wifi konfiguralasa statikus IP-hez! (stabilabb mukodes)
      //Serial.println("Wifi konfiguralas sikertelen.");
      WiFi.mode(WIFI_STA); 
  
       pinMode(LED_BUILTIN, OUTPUT);
       WiFiConnecting();
       ThingSpeak.begin(client); // ThingSpeak kapcsolat felepitese
       
       //WiFi.begin(ssid, pass); // WiFi kapcsolat felepitese
}

void sendDataToThingSpeak(float data1, float data2) { // ket lebego pontos ertek atadasa (homerseklet, paratartalom)
  //ThingSpeak.begin(client);
  delay(5000);
  //ThingSpeak.setField(1, data1); // field1-be homerseklet (7 napos vagy 31 napos)
  //ThingSpeak.setField(2, data2); // field2-be paratartalom (7 napos vagy 31 napos)
  ThingSpeak.setField(3, data1); // field3-ba homerseklet (heti atlaghomerseklethez)
  ThingSpeak.setField(4, data2); // field4-be paratartalom (heti atlag paratartalomhoz)
  ThingSpeak.setField(5, data1); // field5-be homerseklet (havi atlaghomerseklet)
  ThingSpeak.setField(6, data1); // field6-ba homerseklet (negyedeves atlaghomerseklet - 92 nap)
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // channel szam, irasra API kulcs

  if (x == 200) {
        Serial.println(String(x)+ F( " - Homersekleti ertekek frissitve a weboldalon."));
        delay(2000);
  }
  else if ( x != 200)  {
     if (k <= 3) {
     ThingSpeak.setStatus(String(x) + F(" - HIBA "));
       Serial.print(F("Problema lepett fel az adatok frissitese kozben. HTTP kod: "));
       Serial.print(x);
       Serial.print(F(" - "));
       Serial.println(k);
       ledBlinking(4,0,true); // 4 rovid, 0 hosszu, eloszor a rovid villanasok
       k += 1;
       delay(1000);
       readingDatas(5);
       sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
  }
  
    // Ha nem HTTP 200 a visszajelzes a szerveroldalrol es maximum 3-adjara probalkozunk adatot kuldeni.

}

}

void readingDatas(unsigned int counter) { // Szenzorrol olvasasi folyamat
 byte errorCounter = 0;
// bool validValues = false;
 int readData = DHT.read22(DHTPIN); // Adatok leolvasasa a szenzorrol
      float h = DHT.humidity; // Szenzor altal mert paratartalom ertekenek eltarolasa egy h nevu float tipusu valtozoba
      
      Serial.print(h);
      Serial.println(F(" %")); 
      delay(1000);
      
float t = DHT.temperature; // Szenzor altal mert homerseklet ertekenek eltarolasa egy t nevu float tipusu valtozoba
      Serial.print(t);
      Serial.println(F(" °C"));
      //tempReads += 1;
      delay(1000);
                  while (errorCounter < counter) {
        if (readData != 0) {
           Serial.print(F("A szenzorrol olvasas sikertelen volt. - "));
           Serial.println(counter);
           readData = DHT.read22(DHTPIN); // Adatok leolvasasa a szenzorrol
           h = DHT.humidity;
           Serial.print(h);
           Serial.println(F(" %")); 
           delay(1000);
           t = DHT.temperature;
           Serial.print(t);
           Serial.println(F(" °C"));
           delay(1000);
           errorCounter += 1;
       }
       else {
          //validValues = true;
          v = true;
          break;
       }    
}
        
       // Rossz értékek kiszűrése


  datat = t;
  datah = h;




  // ThingSpeak szerver fele adatok tovabbkuldese
 
}

void WiFiConnecting() { // WiFi kapcsolodasi folyamat
     WiFi.begin(ssid,pass);
      while (WiFi.status() != WL_CONNECTED) 
     {    
         
          ledBlinking(5,0,true); // 3 rovid, 0 hosszu, eloszor a rovid villanasok
          //digitalWrite(LED_BUILTIN, LOW);   // Amikor a LED vilagit.
          //delay(300);                       // Ameddig a LED vilagit.
          //digitalWrite(LED_BUILTIN, HIGH);    // Amikor a LED "pihen".
          //delay(300);                         // Ameddig a LED "pihen".
          //Serial.print(".");
     }
      
     Serial.println(F(""));
     if (WiFi.status() == WL_CONNECTED) { // Ha a WiFi kapcsolat csatlakozott statuszban van
      Serial.print(F("SSID: "));
      Serial.print(ssid);
      Serial.print(F(", IP-cím: "));
      Serial.println(WiFi.localIP());
      }

}

void MySQLConnection() { // MYSQL kapcsolodasi folyamat
      Serial.print(F("Csatlakozas a tavoli MYSQL szerverhez..."));
     while (!conn.connect(server_addr, 19968, user, password)) {
      
      ledBlinking(2,1,false); // 2 rovid, 1 hosszu, eloszor a hosszu villanasok

    Serial.println(F("-?"));   
     }
     
    Serial.println(F("OK."));
      cursor = new MySQL_Cursor(&conn);
}

void SendDataToDatabase() { // Adatbazis fele adatkuldesi folyamat
  char buff[88]; // Szenzor adatok tarolasa egy bufferben.
                  char temp[4];      
                  char  hum[5];    

                  dtostrf(datat,2,1,temp); // lebego pontos ertekek konvertalasa sztringge.
                  dtostrf(datah,3,1,hum); // lebego pontos ertekek konvertalasa sztringge.
                  sprintf(buff, INSERT_SQL, temp, hum); // ADATOK elokeszitese az adatbazisba beszurashoz.
                                     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // MYSQL kurzor peldany
                                     cur_mem->execute(buff); // A kiolvasott ertekek beszurasa az adatbazisba (vegrehajtas)
                                     Serial.println(F("Adatok feltoltve az adatbazisba."));
                                     delete cur_mem; // MYSQL kurzor peldany torlese a vegrehajtas utan.
                        conn.close();
}

void loop() // Utasitasok ismetlese amig van tapfeszultseg
{

  if (WiFi.status() != WL_CONNECTED) { // Ha a WiFi kapcsolat nincs csatlakozott statuszban
    WiFiConnecting(); // WiFi kapcsolodasi folyamat
  }
  v = false;
  readingDatas(5); // Szenzorrol olvasasi folyamat
  if (v == true) {
    sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
    MySQLConnection(); // MYSQL kapcsolodasi folyamat
    SendDataToDatabase(); // Adatbazis fele adatkuldesi folyamat
    ledBlinking(0,1,false); // 0 rovid, 1 hosszu, eloszor a hosszu villanasok | Homersekleti ertekek elkuldve a megfelelo helyre.
    Serial.println(F("Varakozas a kovetkezo beolvasasig..."));
    delay(290000);
  // A ThingSpeak-nek szuksege van legalabb 15 masodperces kesleltetesre az adatok frissitesehez.
  // delay(refreshTime); // Nagyjabol 5 percenkenti adatkuldest valosit meg. (~ 290-300 mp)
   }
   else {
    delay(100);
  
   }
  Serial.print(F("Free Heap Size: "));
  Serial.println(ESP.getFreeHeap(),DEC);
}

void ledBlinking(byte shortBlinks, byte longBlinks, bool firstShort) { // Allapotjelzo LED villanasokat megvalosito eljaras
  unsigned int speed = 300; // alapertelmezett sebesseg a 300 ms
  byte i = 0;
if (firstShort) { // Ha eloszor a rovid villanasok hajtodnak vegre
  while (i < shortBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   // Amikor a beepitett LED aktiv allapotban van.
 delay(speed);                       // Ameddig a beepitett LED aktiv allapotban van.
 digitalWrite(LED_BUILTIN, HIGH);    // Amikor a beepitett LED "pihen".
 delay(speed);                       // Ameddig a beepitett LED "pihen".
 i++;
  
}

speed = 1000;    // megvaltoztatjuk a sebesseget 1000 ms-re.
i = 0;

while (i < longBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   // Amikor a beepitett LED aktiv allapotban van.
  delay(speed);                       // Ameddig a beepitett LED aktiv allapotban van.
  digitalWrite(LED_BUILTIN, HIGH);    // Amikor a beepitett LED "pihen".
  delay(speed);                       // // Ameddig a beepitett LED "pihen".
  i++;
}
   
  }
else {
  
speed = 1000; // megvaltoztatjuk a sebesseget 1000 ms-re.
  
  while (i < longBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   
 delay(speed);                       
 digitalWrite(LED_BUILTIN, HIGH);   
 delay(speed); 
 i++;
}

speed = 300;  // megvaltoztatjuk a sebesseget 300 ms-re.
i = 0;

while (i < shortBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   
 delay(speed);                      
 digitalWrite(LED_BUILTIN, HIGH);   
 delay(speed); 
 i++;
  
}
 
  }

}
