#include <Adafruit_NeoPixel.h>

/********************************************************************/
// Meghívjuk a szükséges könyvtárakat!
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Adatkábel csatlakoztatva a 4-es lábára! 
#define ONE_WIRE_BUS 4 
//Az Arduino 4-es lábán olvas be adatot a homerotol.
/********************************************************************/
// Beállítunk egy oneWire példát a OneWire-eszközökkel való kommunikációhoz
// (nem csak a  Maxim/Dallas hőmérő IC-kkel.) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Átadjuk a oneWire hivatkozást a Dallas Temperature-re.
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

//LED Karikahoz kell.
//Argumentum 1: Hany darab LED van osszesen
//Argumentum 2: Melyik Arduino labon megy ki az adat?
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 2, NEO_GRB + NEO_KHZ800);


//For ciklusokhoz valtozok. Eleg a "byte", mert nem megy 255 fole az ertek
byte i,j,k;

  //Szinek vannak eltarolva a tombokben. Oszloposan kell nezni
  byte pirosTomb[12] = {0,   0,     0,   0,   0,  20,  40, 100, 150, 200, 235, 255};
  byte zoldTomb[12] =  {0,   120, 180, 220, 255, 255, 200, 150, 100,  80,  40,   0};
  byte kekTomb[12] =   {255, 255, 180, 180,  40,   0,   0,   0,   0,   0,   0,   0};

  //Homerseklet valtozas eszlelesehez hasznalt valtozok.
  byte lastTemp = 0;
  byte currentTemp = 1;

//A setup csak egyszer fut le. Beallitja a szenzorokat, serial azonositas, stb.
void setup(){
  strip.begin();
  strip.show();
  Serial.begin(9600); 
  sensors.begin(); // szenzorok uzembe helyezese
}

int temp1(){
  sensors.requestTemperatures(); // Homerseklet lekerese a szenzoroktol
  return (byte) sensors.getTempCByIndex(0); //Cast! Eredetileg float tipus van byte-ra alakitva, azzal ter vissza a fuggvveny.
}

int temp12(int fok){
  if (fok <=18) return 0;
  if (fok <=20) return 1;
  if (fok <=22) return 2;
  if (fok <=23) return 3;
  if (fok <=24) return 4;     // Homersekleti fokokhoz tartozo ledek
  if (fok <=25) return 5;
  if (fok <=26) return 6;
  if (fok <=27) return 7;
  if (fok <=28) return 8;
  if (fok <=29) return 9;
  if (fok <=30) return 10;
  if (fok <=32) return 11;
}


void loop(){
  /*
  for(i=0;i<=temp12( temp1() )  ;i++){
   strip.setPixelColor(i, 255, 0, 0); 
   strip.show(); 
   delay(5);
  }
      
  for(j=11;j>temp12(temp1());j--){
   strip.setPixelColor(j, 0, 0, 0); 
   strip.show();
   delay(5);
  }

  //sensors.requestTemperatures();
  //Serial.println(sensors.getTempCByIndex(0));
  Serial.println(temp1());
 */
 
  //Soros monitorra/portra megy ki az adat
  Serial.println(currentTemp);
  Serial.println(lastTemp);
  Serial.println("---");

  currentTemp = temp1();

  if (lastTemp != currentTemp){
  for(i=0;i<=temp12( temp1());i++){ // LEDEK aktivalasa a homersekletig a megfelelo szinekkel!
      for(k=0;k<=i;k++){ // Az aktualis homersekletig beallitja a szineket a ledekre!
        strip.setPixelColor(k, pirosTomb[i], zoldTomb[i], kekTomb[i]); // Beallitja a ledek szinosszetetelet a tombok RGB kodjaibol!
        
        delay(5);
      }
      
   
   }

  for(j=11;j>temp12( temp1());j--){      // Kikapcsolo funkcio, bizonyos LEDEK kikapcsolasa!
    strip.setPixelColor(j, 0, 0, 0);
      strip.show();
   }
   strip.show();
   lastTemp = currentTemp;
  }

   

/*
 for(j=11;j>=temp12( temp1());j--){
    for(k=11;k>=j;k--){
      strip.setPixelColor(k, pirosTomb[j], zoldTomb[j], kekTomb[j]); 
      strip.show();
      delay(5);
    }
   }
   */
 


/*
  strip.setPixelColor(0, 0, 0, 255); 
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 0, 120, 255);
   strip.setPixelColor(1, 0, 120, 255);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 0, 180, 180);
   strip.setPixelColor(1, 0, 180, 180);  
   strip.setPixelColor(2, 0, 180, 180);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 0, 220, 180);
   strip.setPixelColor(1, 0, 220, 180);  
   strip.setPixelColor(2, 0, 220, 180);  
   strip.setPixelColor(3, 0, 220, 180);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 0, 255, 40);
   strip.setPixelColor(1, 0, 255, 40);  
   strip.setPixelColor(2, 0, 255, 40);  
   strip.setPixelColor(3, 0, 255, 40);     
   strip.setPixelColor(4, 0, 255, 40);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 20, 255, 0);
   strip.setPixelColor(1, 20, 255, 0);  
   strip.setPixelColor(2, 20, 255, 0);  
   strip.setPixelColor(3, 20, 255, 0);     
   strip.setPixelColor(4, 20, 255, 0);  
   strip.setPixelColor(5, 20, 255, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 40, 200, 0);
   strip.setPixelColor(1, 40, 200, 0);  
   strip.setPixelColor(2, 40, 200, 0);  
   strip.setPixelColor(3, 40, 200, 0);     
   strip.setPixelColor(4, 40, 200, 0);  
   strip.setPixelColor(5, 40, 200, 0);  
   strip.setPixelColor(6, 40, 200, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 100, 150, 0);
   strip.setPixelColor(1, 100, 150, 0);  
   strip.setPixelColor(2, 100, 150, 0);  
   strip.setPixelColor(3, 100, 150, 0);     
   strip.setPixelColor(4, 100, 150, 0);  
   strip.setPixelColor(5, 100, 150, 0);  
   strip.setPixelColor(6, 100, 150, 0);  
   strip.setPixelColor(7, 100, 150, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 150, 100, 0);
   strip.setPixelColor(1, 150, 100, 0);  
   strip.setPixelColor(2, 150, 100, 0);  
   strip.setPixelColor(3, 150, 100, 0);     
   strip.setPixelColor(4, 150, 100, 0);  
   strip.setPixelColor(5, 150, 100, 0);  
   strip.setPixelColor(6, 150, 100, 0);  
   strip.setPixelColor(7, 150, 100, 0);  
   strip.setPixelColor(8, 150, 100, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 200, 80, 0);
   strip.setPixelColor(1, 200, 80, 0);  
   strip.setPixelColor(2, 200, 80, 0);  
   strip.setPixelColor(3, 200, 80, 0);     
   strip.setPixelColor(4, 200, 80, 0);  
   strip.setPixelColor(5, 200, 80, 0);  
   strip.setPixelColor(6, 200, 80, 0);  
   strip.setPixelColor(7, 200, 80, 0);  
   strip.setPixelColor(8, 200, 80, 0);  
   strip.setPixelColor(9, 200, 80, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 235, 40, 0);
   strip.setPixelColor(1, 235, 40, 0);  
   strip.setPixelColor(2, 235, 40, 0);  
   strip.setPixelColor(3, 235, 40, 0);     
   strip.setPixelColor(4, 235, 40, 0);  
   strip.setPixelColor(5, 235, 40, 0);  
   strip.setPixelColor(6, 235, 40, 0);  
   strip.setPixelColor(7, 235, 40, 0);  
   strip.setPixelColor(8, 235, 40, 0);  
   strip.setPixelColor(9, 235, 40, 0);  
   strip.setPixelColor(10, 235, 40, 0);  
   strip.show();

   delay(2000);
   strip.setPixelColor(0, 255, 0, 0);
   strip.setPixelColor(1, 255, 0, 0);  
   strip.setPixelColor(2, 255, 0, 0);  
   strip.setPixelColor(3, 255, 0, 0);     
   strip.setPixelColor(4, 255, 0, 0);  
   strip.setPixelColor(5, 255, 0, 0);  
   strip.setPixelColor(6, 255, 0, 0);  
   strip.setPixelColor(7, 255, 0, 0);  
   strip.setPixelColor(8, 255, 0, 0);  
   strip.setPixelColor(9, 255, 0, 0);  
   strip.setPixelColor(10, 255, 0, 0);  
   strip.setPixelColor(11, 255, 0, 0);  
   strip.show();

   delay(2000);
  */
   
   delay(1000);
}
