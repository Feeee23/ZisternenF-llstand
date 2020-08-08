#include <Stepper.h>
#include <LiquidCrystal.h>
#include<SR04.h>
#include<Wire.h>
#include<DS3231.h>
#include<SPI.h>
#include<SD.h>
//*********************************************************allg Declaration
//File erstellen
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
int Backlight=13; //HIntergundlicht über Pin 13 Steuern

//*****************************************Setup
void setup() {
  Serial.begin(9600);
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
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
//**************************************Sensor
int Sensor(){
  int A=sr04.Distance();
  int F=2000-A;
  return F;
 }
 //*****************************************Rechner
int Rechner(int FS){
  int l=0;
  if (FS<50){
    l=FS;
   }
  if(FS>1800){
    l=3*FS;
  }
 else{
  l=2*FS;
 }
  return l;
}
//***********************************loop
void loop() {
  if (digitalRead(Schalter)==LOW){
    FStand=Sensor();
    L=Rechner(FStand);
    Ausgabe(FStand, L);
    }
  
}
/*Verbindungen
 * D4-> Taster zum Einschalten weiter auf GND
 * D5-> Ultraschall TRIG
 * D6-> Ultraschall ECHO
 * D7-> Display RS (4v.l.)
 * D8-> Dispaly E (6v.l.)
 * D9-> Display D4 (11v.l.)
 * D10-> Display D5 (12v.l.)
 * D11-> Display D6 (13v.l.)
 * D12-> Display D7 (14v.l.)
 * D13-> Display A (15v.l.)
 *       Display K (16v.l.)->GND
 *       Display VSS (1v.l.)->GND
 *       Dispaly VDD (2v.l.)->5V
 *       Dispaly V0 (3v.l.)->Poti->rest plus minus
 *       Display RW (5v.l.)->GND
 * D50-> SD-Karte MISO 
 * D51-> SD-Karte MOSI
 * D52-> SD-Karte SCK
 * D53-> SD-Karte CS
 *       SD-Karte Vcc
 *       SD-Karte GND
 * SCL-> RTC SCL 
 * SDA-> RTC SDA
 *       RTC GND
 *       RTC VCC
 *       RTC SQW->nix
 */

/*ToDO
Schalter über LOW laufen lassen
Maximale Tiefe ändern in Sensor
Formern ändern in Recner
arduino schlafen legen
Schaltbild
akku stand
Datum als String
Scrollen
*/
