#include <ESP8266WiFi.h> // Wifis ESP könyvtara
#include "dht.h" // DHT11 szenzor könyvtara
#include "ThingSpeak.h"
#include "secrets.h"
#include <MySQL_Connection.h> // ARDUINO --> MYSQL kapcsolodast megvalosito konyvtar
#include <MySQL_Cursor.h>

#define DHTPIN 5 // PIN , ahova a szenzor csatlakozik 5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define ID 2
#define RED 4
#define GREEN 14
#define BLUE 12
#define DEBUG_MODE 0
#define myChannelNumber SECRET_CH_ID
#define SPEED_SLOW 1000
#define SPEED_FAST 300
#define SPEED_MEDIUM 500
#define REFRESH_TIME 290000
#define TS_DELAY 15000
#define TS_BEGIN 5000
#define DELAY_GENERAL 1000
#define LOW_DELAY 100
#define MAX_SENSOR_ERRORS 3
#define MAX_MYSQL_ERRORS 5
#define MAX_TS_ERRORS 3
#define TS_FIELD_T 3
#define TS_FIELD_H 4
#define TS_OK_STATUSCODE 200

char * ssid = SECRET_SSID;   // SSID 
char * pass = SECRET_PASS;   // HALOZATI JELSZO

//unsigned long myChannelNumber = SECRET_CH_ID;  // CSATORNASZAM (ThingSpeak)
char * myWriteAPIKey = SECRET_WRITE_APIKEY; // IRASRA API KULCS (ThingSpeak)
byte k = 1; // probalkozasok szama a ThingSpeak eleresere (gyakori hibakodok miatt volt ra szukseg) // csere
bool v = false;
float datat = 0.0;
float datah = 0.0;
IPAddress server_addr(204,2,63,34); // IP-CIM (adatbazishoz)
//IPAddress local_IP(192, 168, 0, 60); // Lokalis IP-cim , amit statikusan hasznalunk.
//IPAddress gateway(192, 168, 0, 1); // alapertelmezett atjaro
//IPAddress subnet(255, 255, 255, 0); // alhalozati maszk
//IPAddress primaryDNS(8, 8, 8, 8); // opcionalis
//IPAddress secondaryDNS(8, 8, 4, 4); // opcionalis

char * user = SECRET_USER; // MYSQL felhasznalonev
char * password = SECRET_PWD; // MYSQL jelszo a bejelentkezeshez
char * INSERT_SQL = "INSERT INTO  dht11_database.dht11_sensors(homerseklet, paratartalom, szenzor_azonosito) VALUES ('%s','%s','%d')"; // MYSQL INSERT parancs, melyik adatbazis, melyik tabla, milyen ertekeket

//DHT dht(DHTPIN, DHTTYPE); // Szenzor objektum, DHT22
dht DHT; // Szenzor objektum

WiFiClient client; // Wi-Fi kliens objektum

MySQL_Connection conn((Client *)&client); // MYSQL csatlakozas a WiFi kliens segitsegevel
MySQL_Cursor* cursor; // MYSQL kurzor a muveletek elvegzesehez.

void setup() // Beuzemelesi fazis
{
  if (DEBUG_MODE == 1) {
    Serial.begin(9600); // NodeMCU javasolt beallitasa miatt. (baud)
  }
  delay(LOW_DELAY);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
     // WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS); // Wifi konfiguralasa statikus IP-hez! (stabilabb mukodes)
  WiFi.mode(WIFI_STA); 
  
  pinMode(LED_BUILTIN, OUTPUT);
  setColor(173,6,2); // PIROS SZIN
  WiFiConnecting();
  setColor(6,195,2); // ZOLD SZIN
  ThingSpeak.begin(client); // ThingSpeak kapcsolat felepitese
}

void setColor(short redValue, short greenValue, short blueValue) {
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
}

void sendDataToThingSpeak(float data1, float data2) { // ket lebego pontos ertek atadasa (homerseklet, paratartalom)
  //ThingSpeak.begin(client);
  delay(TS_BEGIN);
  ThingSpeak.setField(TS_FIELD_T, data1); // field1-be homerseklet (7 napos vagy 31 napos)
  ThingSpeak.setField(TS_FIELD_H, data2); // field2-be paratartalom (7 napos vagy 31 napos)
  //ThingSpeak.setField(TS_FIELD_T, data1); // field3-ba homerseklet (heti atlaghomerseklethez)
  //ThingSpeak.setField(TS_FIELD_H, data2); // field4-be paratartalom (heti atlag paratartalomhoz)
  //ThingSpeak.setField(TS_FIELD_T, data1); // field5-be homerseklet (havi atlaghomerseklet)
  //ThingSpeak.setField(TS_FIELD_H, data2); // field6-ba homerseklet (negyedeves atlaghomerseklet - 92 nap)
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // channel szam, irasra API kulcs
    Serial.print("#D Statuszkod erteke (Thingspeak) : ");
    Serial.println(x);
  if (x == TS_OK_STATUSCODE) {
    ledBlinking(0,2,false); // 2 hosszu, 0 rovid villanas, eloszor a hosszu villanasok
  }
  else if ( x != TS_OK_STATUSCODE)  {
     if (k <= MAX_TS_ERRORS) {
       ThingSpeak.setStatus(String(x) + F(" - HIBA "));
       ledBlinking(4,0,true); // 4 rovid, 0 hosszu, eloszor a rovid villanasok
       k += 1;
       delay(TS_DELAY);
       readingDatas(MAX_SENSOR_ERRORS);
       sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
    }
    // Ha nem HTTP 200 a visszajelzes a szerveroldalrol es maximum 3-adjara probalkozunk adatot kuldeni.
  }

}

void readingDatas(unsigned int counter) { // Szenzorrol olvasasi folyamat
 byte errorCounter = 0;
 int readData = DHT.read(DHTPIN); // Adatok leolvasasa a szenzorrol
 float h = DHT.humidity; // Szenzor altal mert paratartalom ertekenek eltarolasa egy h nevu float tipusu valtozoba
 delay(DELAY_GENERAL);      
 float t = DHT.temperature; // Szenzor altal mert homerseklet ertekenek eltarolasa egy t nevu float tipusu valtozoba
 delay(DELAY_GENERAL);
 Serial.print("#D: T: ");
 Serial.print(t);
 Serial.print(" , H: ");
 Serial.println(h);  
 Serial.print("#D Check-sum erteke (DHT22) : ");
 Serial.println(readData);
  if (readData != 0) {
   ledBlinking(2,1,true);
   errorCounter += 1;
   while (errorCounter < counter) {
    readData = DHT.read(DHTPIN); // Adatok leolvasasa a szenzorrol
    h = DHT.humidity;
    delay(DELAY_GENERAL);
    t = DHT.temperature;
    delay(DELAY_GENERAL);
              
    if (readData == 0) {
     v = true;
     ledBlinking(0,1,true);
     break;
    }
   }
  
           
  }

    else {
     v = true;
    }
       // Rossz értékek kiszűrése
    if (v) {
   datat = t;
   datah = h;
  }
  // ThingSpeak szerver fele adatok tovabbkuldese
}

void WiFiConnecting() { // WiFi kapcsolodasi folyamat
     setColor(173,6,2); // PIROS SZIN
     WiFi.begin(ssid,pass);
     while (WiFi.status() != WL_CONNECTED) 
     {    
          ledBlinking(5,0,true); // 3 rovid, 0 hosszu, eloszor a rovid villanasok
     }
      
     if (WiFi.status() == WL_CONNECTED) { // Ha a WiFi kapcsolat csatlakozott statuszban van
       setColor(6,195,2); // ZOLD SZIN
        Serial.print(F("SSID: "));
        Serial.print(ssid);
        Serial.print(F(", IP-cím: "));
        Serial.println(WiFi.localIP());
  }
}

void MySQLConnection() { // MYSQL kapcsolodasi folyamat
     byte connections = 1;
     while (!conn.connect(server_addr, 19968, user, password)) {
      connections++;
      if (connections <= MAX_MYSQL_ERRORS) {
        ledBlinking(2,1,false); // 2 rovid, 1 hosszu, eloszor a hosszu villanasok
        delay(DELAY_GENERAL);
      }
      else {
        break;
      }
    }
    Serial.print("#D csatlakozasok szama (Database) : ");
    Serial.println(connections);
    cursor = new MySQL_Cursor(&conn);
}

void SendDataToDatabase() { // Adatbazis fele adatkuldesi folyamat
       char buff[200]; // Szenzor adatok tarolasa egy bufferben.
       char temp[4];      
       char  hum[5];   
       dtostrf(datat,2,1,temp); // lebego pontos ertekek konvertalasa sztringge.
       dtostrf(datah,3,1,hum); // lebego pontos ertekek konvertalasa sztringge.
       sprintf(buff, INSERT_SQL, temp, hum, ID); // ADATOK elokeszitese az adatbazisba beszurashoz.
       MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // MYSQL kurzor peldany
       cur_mem->execute(buff); // A kiolvasott ertekek beszurasa az adatbazisba (vegrehajtas)
       delete cur_mem; // MYSQL kurzor peldany torlese a vegrehajtas utan.
       conn.close();
}

void loop() // Utasitasok ismetlese amig van tapfeszultseg
{
  if (WiFi.status() != WL_CONNECTED) { // Ha a WiFi kapcsolat nincs csatlakozott statuszban
    WiFiConnecting(); // WiFi kapcsolodasi folyamat
  }
  v = false;
  readingDatas(MAX_SENSOR_ERRORS); // Szenzorrol olvasasi folyamat
  if (v == true) {
    sendDataToThingSpeak(datat, datah); // ThingSpeak szerver fele az adatok elkuldese
    ledBlinking(3,0,true);
    MySQLConnection(); // MYSQL kapcsolodasi folyamat
    SendDataToDatabase(); // Adatbazis fele adatkuldesi folyamat
    setColor(51,51,255); // KEKES SZIN
    delay(DELAY_GENERAL);
    if (WiFi.status() == WL_CONNECTED) {
      setColor(6,195,2); // ZOLD SZIN
    }
    else {
      setColor(173,6,2); // PIROS SZIN 
    }
    delay(REFRESH_TIME); // varakozas a kovetkezo beolvasasig
  // A ThingSpeak-nek szuksege van legalabb 15 masodperces kesleltetesre az adatok frissitesehez.
   }
   else {
    delay(LOW_DELAY);
   }
}

void ledBlinking(byte shortBlinks, byte longBlinks, bool firstShort) { // Allapotjelzo LED villanasokat megvalosito eljaras
  byte i = 0;
  if (firstShort) { // Ha eloszor a rovid villanasok hajtodnak vegre
    while (i < shortBlinks) {
    digitalWrite(LED_BUILTIN, LOW);   // Amikor a beepitett LED aktiv allapotban van.
    delay(SPEED_FAST);                       // Ameddig a beepitett LED aktiv allapotban van.
    digitalWrite(LED_BUILTIN, HIGH);    // Amikor a beepitett LED "pihen".
    delay(SPEED_FAST);                       // Ameddig a beepitett LED "pihen".
     i++;
  
    }
    
i = 0;

  while (i < longBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   // Amikor a beepitett LED aktiv allapotban van.
  delay(SPEED_SLOW);                       // Ameddig a beepitett LED aktiv allapotban van.
  digitalWrite(LED_BUILTIN, HIGH);    // Amikor a beepitett LED "pihen".
  delay(SPEED_SLOW);                       // // Ameddig a beepitett LED "pihen".
  i++;
}
   
  }
  else {
  while (i < longBlinks) {
    digitalWrite(LED_BUILTIN, LOW);   
    delay(SPEED_SLOW);                       
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(SPEED_SLOW); 
    i++;
  }

i = 0;

  while (i < shortBlinks) {
  digitalWrite(LED_BUILTIN, LOW);   
  delay(SPEED_FAST);                      
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(SPEED_FAST); 
  i++;
  }
 }
}
