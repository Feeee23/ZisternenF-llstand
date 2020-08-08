#include <Stepper.h>

// include the library code:
#include <LiquidCrystal.h>
#include<SR04.h>
#include<Wire.h>
#include<DS3231.h>

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
char dat[16];
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
  }
//**********************************************Ausgabe
void Ausgabe(int hoehe, int liter){
   digitalWrite(Backlight, HIGH);
   
   lcd.clear();
   lcd.setCursor(0, 0);
   dt=clock.getDateTime();
   lcd.setCursor(0, 0);
   if(dt.day<10){
    lcd.print("0");
   }
   lcd.print(dt.day); lcd.print(".");
    if(dt.month<10){
    lcd.print("0");
   }
   lcd.print(dt.month); lcd.print(".");
   lcd.print(dt.year); lcd.setCursor(0, 1);
    if(dt.hour<10){
    lcd.print("0");
   }
   lcd.print(dt.hour); lcd.print(":");
   if(dt.minute<10){
    lcd.print("0");
   }
   lcd.print(dt.minute);
   delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fuellstand:");
  lcd.setCursor(0, 1);
  lcd.print(hoehe);
  lcd.setCursor(5, 1);
  lcd.print("cm");
  delay(3000);
   
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("entspricht ca.:");
   lcd.setCursor(0, 1);
   lcd.print(liter);
   lcd.setCursor(5, 1);   
   lcd.print("l");
  delay(3000);

 lcd.clear();
 digitalWrite(Backlight, LOW);
}
//*****************************************Datum
/*void Datum(char* dat){
  dt=clock.getDateTime();
  itoa(dt.day,dat[0], 10);
  dat[2]=".";
  itoa(dt.month,dat[4],10);
  dat[5]=".";
  itoa(dt.year,dat[6],10);
  dat[10]=" ";
  itoa(dt.hour,dat[11],10);
  dat[13]=":";
  itoa(dt.minute,dat[14],10);
  }*/
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


/*ToDO
Schalter über LOW laufen lassen
Speichern auf karte
Maximale Tiefe ändern in Sensor
Formern ändern in Recner
arduino schlafen legen
Schaltbild
akku stand
Datum als String
Scrollen
*/
