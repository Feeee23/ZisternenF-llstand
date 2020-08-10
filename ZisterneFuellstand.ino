#include <LiquidCrystal.h>
#include<SR04.h>
#include<Wire.h>
#include<DS3231.h>
#include<SPI.h>
#include<SD.h>
#include<String.h>
//*********************************************************allg Declaration
File myFile;
const int CS=53;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
//define Pins for UltrasonicSensor
int Trig=5;
int Echo=6;
SR04 sr04= SR04(Echo, Trig);
//Initaliz the clock
DS3231 clock;
RTCDateTime dt;

int Schalter=4; //Pin für Schalter  Def
int FStand=0;
int L=0;
int k=0;
int Backlight=13; //HIntergundlicht über Pin 13 Steuern

//*****************************************Setup
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(Backlight, OUTPUT);
  pinMode(Schalter, INPUT);
  digitalWrite(Schalter, HIGH);
  if(!SD.begin()){
    digitalWrite(Backlight, HIGH);
    lcd.print("initalization failed");
    delay(1000);
    digitalWrite(Backlight, LOW);
   }
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
 }
//**********************************************Ausgabe
void Ausgabe(int hoehe, int liter){
   myFile=SD.open("Daten.txt", FILE_WRITE);
   if(!myFile){
    digitalWrite(Backlight, HIGH);
    lcd.print("Error open File");
    delay(1000);
    digitalWrite(Backlight, LOW);
   }
   digitalWrite(Backlight, HIGH);
   
   lcd.clear();
   lcd.setCursor(0, 0);
   dt=clock.getDateTime();
   lcd.setCursor(0, 0);
   if(dt.day<10){
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.day); lcd.print(".");
   myFile.print(dt.day); myFile.print(".");
    if(dt.month<10){
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.month); lcd.print(".");
   myFile.print(dt.month); myFile.print(".");
   lcd.print(dt.year); lcd.setCursor(0, 1);
   myFile.print(dt.year); myFile.print("  ");
    if(dt.hour<10){
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.hour); lcd.print(":");
   myFile.print(dt.hour);myFile.print(":");
   if(dt.minute<10){
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.minute);
   myFile.print(dt.minute); myFile.print("  ");
   delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fuellstand:");
  myFile.print("Fuellstand: ");
  lcd.setCursor(0, 1);
  lcd.print(hoehe);
  myFile.print(hoehe);
  lcd.setCursor(5, 1);
  lcd.print("cm");
  myFile.print(" cm, ");
  delay(3000);
  
  if(hoehe>155){
    lcd.setCursor(0,0);
    lcd.print("Wasser laeuft ab");
    lcd.setCursor(0, 1);
    lcd.print("165cm ueber");    
    delay(3000);
  }
   
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("entspricht ca.:");
   myFile.print("entspricht ca.: ");
   lcd.setCursor(0, 1);
   lcd.print(liter);
   myFile.print(liter);
   lcd.setCursor(5, 1);   
   lcd.print("l");
   myFile.println(" l");
  delay(3000);

  myFile.close();
 lcd.clear();
 digitalWrite(Backlight, LOW);
}
//**********************************************Ausgabe2 ohne Display
void Ausgabe2(int hoehe, int liter){
   myFile=SD.open("Daten.txt", FILE_WRITE);

   dt=clock.getDateTime();
   if(dt.day<10){
    myFile.print("0");
   }
   myFile.print(dt.day); myFile.print(".");
    if(dt.month<10){
    myFile.print("0");
   }
   myFile.print(dt.month); myFile.print(".");
   myFile.print(dt.year); myFile.print("  ");
    if(dt.hour<10){
    myFile.print("0");
   }
   myFile.print(dt.hour);myFile.print(":");
   if(dt.minute<10){
    myFile.print("0");
   }
   myFile.print(dt.minute); myFile.print("  ");
   delay(3000);
  
  myFile.print("Fuellstand: ");
  myFile.print(hoehe);
  myFile.print(" cm, ");
  delay(3000);
    
   myFile.print("entspricht ca.: ");
   myFile.print(liter);
   myFile.println(" l");
  delay(3000);

  myFile.close();
}
//**************************************Sensor
int Sensor(){
  int A=sr04.Distance();
  int B=sr04.Distance();
  int C=sr04.Distance();
  int D=(A+B+C)/3;
  int F=190-D; //Die Höhe des Sensors minus den Messwert ergibt den Füllstand
  return F;
 }
 //*****************************************Rechner
int Rechner(int FS){
  float l=0;
  if (FS<140){
    l=100*3.1415*FS/10;
   }else{
  l=4400+(FS/10-14)*314.15-36.65*(FS/10-14)*(FS/10-14)+1.43*(FS/10-14)*(FS/10-14)*(FS/10-14);
 }
 int k=l;
  return k;
}
//***********************************loop
void loop() {
  dt=clock.getDateTime();
    if(dt.second==12& k==0){ //einmaltägliche abfrage um 12 uhr
      FStand=Sensor();
      L=Rechner(FStand);
      Ausgabe2(FStand, L);
      k++;
  }
  if (dt.second==0){ //zurücksetzen den Abfrage Counters um 10Uhr
    k=0;
  }
  if (digitalRead(Schalter)==LOW){
    FStand=Sensor();
    L=Rechner(FStand);
    Ausgabe(FStand, L);  
    }
  if(dt.hour==12 & dt.minute==0){
    FStand=Sensor();
  }
  }
/*Verbindungen
 *    D4->  Taster zum Einschalten weiter auf GND
 *    D5->  Ultraschall TRIG
 *    D6->  Ultraschall ECHO
 *    D7->  Display RS (4v.l.)
 *    D8->  Dispaly E (6v.l.)
 *    D9->  Display D4 (11v.l.)
 *    D10-> Display D5 (12v.l.)
 *    D11-> Display D6 (13v.l.)
 *    D12-> Display D7 (14v.l.)
 *    D13-> Display A (15v.l.)
 *          Display K (16v.l.)->GND
 *          Display VSS (1v.l.)->GND
 *          Dispaly VDD (2v.l.)->5V
 *          Dispaly V0 (3v.l.)->Poti->rest plus minus
 *          Display RW (5v.l.)->GND
 *    D50-> SD-Karte MISO 
 *    D51-> SD-Karte MOSI
 *    D52-> SD-Karte SCK
 *    D53-> SD-Karte CS
 *          SD-Karte Vcc
 *          SD-Karte GND
 *    SCL-> RTC SCL 
 *    SDA-> RTC SDA
 *          RTC GND
 *          RTC VCC
 *          RTC SQW->nix
 */

/*ToDO
Auf ATMega ändern
*/
