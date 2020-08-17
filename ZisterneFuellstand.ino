//https://github.com/Feeee23/ZisternenF-llstand/blob/master/README.md
#include <LiquidCrystal.h>
#include<SR04.h> //Ultarschallsensor
//#include<Wire.h>
#include<DS3231.h> //RTC
#include<SPI.h>
#include<SD.h>  //SD-Karte
//*********************************************************allg Declaration
File myFile;
const int CS=10; //CardSelect auf Pin 10
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
//define Pins for UltrasonicSensor
int Trig=2;
int Echo=1;
SR04 sr04= SR04(Echo, Trig);
//Initaliz the clock
DS3231 clock;
RTCDateTime dt;

int Schalter=0; //Pin für Schalter  Def
int FStand=0; //Fuellstand cm
int L=0; //Liter
int m=0; // Zaehler für falsche Messwerte umm endlossschleife zu verhindern
int Backlight=9; //HIntergundlicht über Pin 13 Steuern

//*****************************************Setup
void setup() {
  lcd.begin(16, 2); //LCD Bildschirm Starten
  pinMode(Backlight, OUTPUT); //Licht von LCD
  pinMode(Schalter, INPUT); //Taster auf GND
  digitalWrite(Schalter, HIGH);
  if(!SD.begin()){ //SD-Karte Inizalisieren und Starten
    digitalWrite(Backlight, HIGH);
    lcd.print("initalization failed");
    delay(1000);
    digitalWrite(Backlight, LOW);
   }
  clock.begin(); //Uhr Starten
  clock.setDateTime(__DATE__, __TIME__); //evtl. ändern, holebn von SD-Karte? welches Format
 }
//**********************************************Ausgabe
void Ausgabe(int hoehe, int liter){
    if(!SD.begin()){ //SD-Karte starten und initalisieren
    digitalWrite(Backlight, HIGH);
    lcd.print("initalization failed");
    delay(1000);
    digitalWrite(Backlight, LOW);
   }  
   myFile=SD.open("Daten.txt", FILE_WRITE); //Daten Datei öffen zum schreiben, dadurch kann SD-Karte jederzeit im Betrieb rein und raus genommen werden
   if(!myFile){
    digitalWrite(Backlight, HIGH);
    lcd.print("Error open File");
    delay(1000);
    lcd.clear();
    digitalWrite(Backlight, LOW);
   }
   digitalWrite(Backlight, HIGH);
   
   lcd.clear(); //Beginn der Ausgabe 
    dt=clock.getDateTime(); //Uhrzeit holen
   lcd.setCursor(0, 0);
   if(dt.day<10){ //Einheitliches Format immer DD.MM.YYYY
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.day); lcd.print("."); //Tag auf lcd auusgeben
   myFile.print(dt.day); myFile.print("."); //Tag auf SD-Karte schreiben
    if(dt.month<10){ //Einheitliches Format immer DD.MM.YYYY
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.month); lcd.print(".");
   myFile.print(dt.month); myFile.print(".");
   lcd.print(dt.year); lcd.setCursor(0, 1);
   myFile.print(dt.year); myFile.print("  ");
    if(dt.hour<10){ //Einheitliches Format immer hh:mm
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.hour); lcd.print(":");
   myFile.print(dt.hour);myFile.print(":");
   if(dt.minute<10){ //Einheitliches Format immer hh:mm
    lcd.print("0");
    myFile.print("0");
   }
   lcd.print(dt.minute);
   myFile.print(dt.minute); myFile.print("  ");
   delay(3000);
  
  lcd.clear();//Fueüllstand in cm ausgeben
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
  
  if(hoehe>155){ //Warung wenn Fuellstand zu hoch
    lcd.setCursor(0,0);
    lcd.print("Wasser laeuft ab");
    lcd.setCursor(0, 1);
    lcd.print("165cm ueber");    
    delay(3000);
  }
   
   lcd.clear(); //Ausgabe Liter Anzahl
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

  myFile.close(); //Datei schließen um jederzeit entnehemn zu können
 lcd.clear();
 digitalWrite(Backlight, LOW);
}
//**********************************************Ausgabe2 ohne Display alles gleich Ausgabe aber ohne Lcd und ohne delays
void Ausgabe2(int hoehe, int liter){
     
   if(!SD.begin()){
    lcd.print("initalization failed");
   }  
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
  
  myFile.print("Fuellstand: ");
  myFile.print(hoehe);
  myFile.print(" cm, ");
    
   myFile.print("entspricht ca.: ");
   myFile.print(liter);
   myFile.println(" l");

  myFile.close();
}
//**************************************Sensor 
int Sensor(){
  int A=sr04.Distance(); //Messe dreimal hintereinander um Feler zu minimiern
  int B=sr04.Distance();
  int C=sr04.Distance();
  int D=(A+B+C)/3; //Durchschnitt der 3 Werte ermitteln
  int F=190-D; //Die Höhe des Sensors (190cm) minus den Messwert ergibt den Füllstand

    if(A>195||B>195||C>195){ //Fehler minimierung wenn ein Messwert zu groß mach es nochmal (5cm Toleranz)
    m++;
    if(m>5){ //Nach 5mal Falsch mach einfach weiter (Endlosschleife verhindern)
      digitalWrite(Backlight, HIGH);
      lcd.print("Fehler Sensor");
      delay(500);
      lcd.clear();
      digitalWrite(Backlight, LOW);
      return;
    }
    return Sensor();
  }
  return F; //Fuellstand zurückgeben
 }
 //*****************************************Rechner
int Rechner(int FS){ //Berechnet aus dem Fuellstand die Literanzahl
  float l=0;
  if (FS<140){ //under 1,4m ist die Zisterne Zylindrisch
    l=100*3.1415*FS/10;
   }else{ //Darueber Kegelstumpf
  l=4400+(FS/10-14)*314.15-36.65*(FS/10-14)*(FS/10-14)+1.43*(FS/10-14)*(FS/10-14)*(FS/10-14); //max wert Zylindrsich plus Zylinderstumpf formel (r2 durch h ausgedrueckt)
 }
 int k=l; //von float auf int runden
  return k; //Literanzahl zurueckgeben
}
//***********************************loop
void loop() {
  dt=clock.getDateTime(); //aktuelle Uhrzeit holen
    if(dt.hour==12 & dt.minute==12 & dt.second==12){ //einmaltägliche abfrage um 12:12:12 uhr Fuellstand abfragen
      FStand=Sensor(); //Sensor anfragen
      L=Rechner(FStand); //in liter umrechen
      Ausgabe2(FStand, L); //Auf SD-Karte Schreiben 
      delay(1000); //damit die Abrage nur einemal Passiert warten eine secunde
  }

  if (digitalRead(Schalter)==LOW){ //Wenn Taster gedrueckt
    FStand=Sensor(); //Sensor Anfragen
    L=Rechner(FStand); //in Liter umrechen
    Ausgabe(FStand, L);//Ausgabe auf lcd und speichern auf SD-Karte
    }
  m=0; //Fehler zaehler vom Sensor zurücksetzten 
  }
/*Verbindungen
 *AT    Arduino -> Physical
 *D0    D4->  Taster zum Einschalten weiter auf GND
 *D2    D5->  Ultraschall TRIG
 *D1    D6->  Ultraschall ECHO
 *D3    D7->  Display RS (4v.l.)
 *D4    D8->  Dispaly E (6v.l.)
 *D5    D9->  Display D4 (11v.l.)
 *D6    D10-> Display D5 (12v.l.)
 *D7    D11-> Display D6 (13v.l.)
 *D8    D12-> Display D7 (14v.l.)
 *D9    D13-> Display A (15v.l.)
 *          Display K (16v.l.)->GND
 *          Display VSS (1v.l.)->GND
 *          Dispaly VDD (2v.l.)->5V
 *          Dispaly V0 (3v.l.)->Poti->rest plus minus
 *          Display RW (5v.l.)->GND
 *D12    D50-> SD-Karte MISO 
 *D11    D51-> SD-Karte MOSI
 *D13    D52-> SD-Karte SCK
 *D10    D53-> SD-Karte CS
 *          SD-Karte Vcc
 *          SD-Karte GND
 *A5    SCL-> RTC SCL 
 *A4    SDA-> RTC SDA
 *          RTC GND
 *          RTC VCC
 *          RTC SQW->nix
 */

/*ToDO
Neue RTC einbinden
evtl. Datum von DS-Card holen
Stromversorgung
*/
