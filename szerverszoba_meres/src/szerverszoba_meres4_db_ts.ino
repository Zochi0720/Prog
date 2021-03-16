#include <ESP8266WiFi.h> // Wifis ESP konyvtara
#include "dht.h" // DHT11 szenzor konyvtara
#include "ThingSpeak.h"
#include "secrets.h"
#include <MySQL_Connection.h> // ARDUINO --> MYSQL kapcsolodast megvalosito konyvtar
#include <MySQL_Cursor.h>

#define ONE_HOUR 3600000UL

char ssid[] = SECRET_SSID;   // SSID 
char pass[] = SECRET_PASS;   // HALOZATI JELSZO
unsigned long myChannelNumber = SECRET_CH_ID;  // CSATORNASZAM (ThingSpeak)
const char * myWriteAPIKey = SECRET_WRITE_APIKEY; // IRASRA API KULCS (ThingSpeak)
const int refreshTime = 290000;   // Nagyjabol 5 percenkent olvassuk le a homersekleti ertekeket.
const short readingTime = 1000; // A szenzorrol olvasasi ido ms-ben megadva.
unsigned long prevMillis = 0;  // elozo beolvasas idobelyege
byte k = 1; // probalkozasok szama a ThingSpeak eleresere (gyakori hibakodok miatt volt ra szukseg) // csere
byte r = 1; // olvasasi probalkozasok szama (3) // csere
float datat = 0;
float datah = 0;
unsigned int tempReads = 0; // szenzorrol olvasasok szama
IPAddress server_addr(204,2,63,34); // IP-CIM (adatbazishoz)
IPAddress local_IP(192, 168, 0, 58); // Lokalis IP-cim , amit statikusan hasznalunk.
IPAddress gateway(192, 168, 0, 1); // alapertelmezett atjaro
IPAddress subnet(255, 255, 255, 0); // alhalozati maszk
IPAddress primaryDNS(8, 8, 8, 8); // opcionalis
IPAddress secondaryDNS(8, 8, 4, 4); // opcionalis

 char user[] = SECRET_USER; // MYSQL felhasznalonev
 char password[] = SECRET_PWD; // MYSQL jelszo a bejelentkezeshez
 char INSERT_SQL[] = "INSERT INTO  dht11_database.dht11_sensors(homerseklet, paratartalom) VALUES ('%s','%s')"; // MYSQL INSERT parancs, melyik adatbazis, melyik tabla, milyen ertekeket

#define DHTPIN 5 // PIN , ahova a szenzor csatlakozik
 
dht DHT; // Szenzor objektum
 
WiFiClient client; // Wi-Fi kliens objektum

MySQL_Connection conn(&client); // MYSQL csatlakozas a WiFi kliens segitsegevel
MySQL_Cursor* cursor; // MYSQL kurzor a muveletek elvegzesehez.

void setup() // Beuzemelesi fazis
{
  Serial.begin(9600); // NodeMCU javasolt beallitasa miatt. (baud)
  delay(20);

      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { // Wifi konfiguralasa statikus IP-hez! (stabilabb mukodes)
      Serial.println("Wifi konfiguralas sikertelen.");
}

  
       pinMode(LED_BUILTIN, OUTPUT);
       WiFiConnecting();
       ThingSpeak.begin(client); // ThingSpeak kapcsolat felepitese
       
       //WiFi.begin(ssid, pass); // WiFi kapcsolat felepitese
}

void sendDataToThingSpeak(float data1, float data2) { // ket lebego pontos ertek atadasa (homerseklet, paratartalom)
  ThingSpeak.begin(client);
  delay(5000);
  ThingSpeak.setField(1, data1); // field1-be homerseklet (7 napos vagy 31 napos)
  ThingSpeak.setField(2, data2); // field2-be paratartalom (7 napos vagy 31 napos)
  ThingSpeak.setField(3, data1); // field3-ba homerseklet (heti atlaghomerseklethez)
  ThingSpeak.setField(4, data2); // field4-be paratartalom (heti atlag paratartalomhoz)
  ThingSpeak.setField(5, data1); // field5-be homerseklet (havi atlaghomerseklet)
  ThingSpeak.setField(6, data1); // field6-ba homerseklet (negyedeves atlaghomerseklet - 92 nap)
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // channel szam, irasra API kulcs

  if (x == 200) {
        Serial.println(String(x) + " - Homersekleti ertekek frissitve a weboldalon.");
        delay(2*readingTime);
  }
  else if ( x != 200)  {
     if (k <= 3) {
     ThingSpeak.setStatus(String(x) + " - HIBA ");
       Serial.print("Problema lepett fel az adatok frissitese kozben. HTTP kod: " + String(x));
       Serial.print(" - ");
       Serial.println(k);
       ledBlinking(4,0,true); // 4 rovid, 0 hosszu, eloszor a rovid villanasok
       k += 1;
       delay(15*readingTime);
       readingDatas(r);
       sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
  }
  
    // Ha nem HTTP 200 a visszajelzes a szerveroldalrol es maximum 3-adjara probalkozunk adatot kuldeni.

}

}

void readingDatas(unsigned int counter) { // Szenzorrol olvasasi folyamat
 int readData = DHT.read22(DHTPIN); // Adatok leolvasasa a szenzorrol
      float h = DHT.humidity; // Szenzor altal mert paratartalom ertekenek eltarolasa egy h nevu float tipusu valtozoba
      Serial.print(h);
      Serial.println(" %"); 
      delay(readingTime);
float t = DHT.temperature; // Szenzor altal mert homerseklet ertekenek eltarolasa egy t nevu float tipusu valtozoba
      Serial.print(t);
      Serial.println(" °C");
      tempReads += 1;
      Serial.println("Szenzorrol olvasasok szama: " + String(tempReads));
       // Rossz értékek kiszűrése
       while (counter <= 3) {
        if (readData != 0) {
          Serial.println("A szenzorrol olvasas sikertelen volt. - " + String(counter));
          delay(readingTime);
         counter += 1;
         readingDatas(counter); // Adatok beolvasasa (rekurzív hivas)
         
       }
       else {
          break;
       }    
}

if (counter > 3) {
  loop(); // az eszkoz loop programreszletet meghivjuk.
}
else {
  datat = t;
  datah = h;
}

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
      
     Serial.println("");
     if (WiFi.status() == WL_CONNECTED) { // Ha a WiFi kapcsolat csatlakozott statuszban van
      Serial.print("SSID: ");
      Serial.print(ssid);
      Serial.print(", IP-cím: ");
      Serial.println(WiFi.localIP());
      }

}

void MySQLConnection() { // MYSQL kapcsolodasi folyamat
      Serial.print("Csatlakozas a tavoli MYSQL szerverhez...");
     while (!conn.connect(server_addr, 19968, user, password)) {
      
      ledBlinking(2,1,false); // 2 rovid, 1 hosszu, eloszor a hosszu villanasok

    Serial.println("-?");   
     }
     
    Serial.println("OK.");
      cursor = new MySQL_Cursor(&conn);
}

void SendDataToDatabase() { // Adatbazis fele adatkuldesi folyamat
  char buff[128]; // Szenzor adatok tarolasa egy bufferben.
                  char temp[4];      
                  char hum[5];    

                  dtostrf(datat,2,1,temp); // lebego pontos ertekek konvertalasa sztringge.
                  dtostrf(datah,3,1,hum); // lebego pontos ertekek konvertalasa sztringge.
                  sprintf(buff, INSERT_SQL, temp, hum); // ADATOK elokeszitese az adatbazisba beszurashoz.
                                     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // MYSQL kurzor peldany
                                     cur_mem->execute(buff); // A kiolvasott ertekek beszurasa az adatbazisba (vegrehajtas)
                                     Serial.println("Adatok feltoltve az adatbazisba.");
                                     delete cur_mem; // MYSQL kurzor peldany torlese a vegrehajtas utan.
                       
}

void loop() // Utasitasok ismetlese amig van tapfeszultseg
{
  unsigned long currentMillis = millis();
  if ( ((unsigned long)(currentMillis - prevMillis) >= refreshTime ) || (tempReads == 0) ) { // elozo beolvasast hasonlitjuk a mostani idoponthoz (milliszekundumokban)
      k = 1;
      r = 1;
  if (WiFi.status() != WL_CONNECTED) { // Ha a WiFi kapcsolat nincs csatlakozott statuszban
    WiFiConnecting(); // WiFi kapcsolodasi folyamat
  }
  prevMillis = currentMillis; // Az elozo beolvasas "idopillanatat" frissitjuk a jelenlegi "idopillanatra".
  readingDatas(r); // Szenzorrol olvasasi folyamat
  sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
  MySQLConnection(); // MYSQL kapcsolodasi folyamat
  SendDataToDatabase(); // Adatbazis fele adatkuldesi folyamat
  ledBlinking(0,1,false); // 0 rovid, 1 hosszu, eloszor a hosszu villanasok | Homersekleti ertekek elkuldve a megfelelo helyre.
  Serial.println("Varakozas a kovetkezo beolvasasig...");
  // A ThingSpeak-nek szuksege van legalabb 15 masodperces kesleltetesre az adatok frissitesehez.
  // delay(refreshTime); // Nagyjabol 5 percenkenti adatkuldest valosit meg. (~ 290-300 mp)
   }
  
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
