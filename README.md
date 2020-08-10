# ZisternenF-llstand

zur Füllstandsüberwachnug der Zisterne mittels eines ATMega328 und eines Ultraschallsensors

Die Zisterne hat die Form eines Zylinders mit darauf sitzendem Stumpen Kegel
Der Zylinder hat die Maße r=10dm h=20dm
der Stumpe Kegal r_1=10dm, r_2=3dm, h=6dm, der Überlauf kommt nach 2,5dm (16,5dm vom Boden)

Der Sensor wird in einem Rohr d__ auf der Höhe 1,90m angebracht werden

Versionshistorie
1. Master (Arduino)
2. TimeasString (Arduino)
3. SD-Card (Arduino)
4. Arduino_finish (Arduino)
5. ATMega_1 (ATMega)
...

/*Verbindungen
 *AT    Arduino -> Physical
 *D0    D4->  Taster zum Einschalten weiter auf GND
 *D1    D5->  Ultraschall TRIG
 *D2    D6->  Ultraschall ECHO
 *D3    D7->  Display RS (4v.l.)
 *D4    D8->  Dispaly E (6v.l.)
 *D5    D9->  Display D4 (11v.l.)
 *D6    D10-> Display D5 (12v.l.)
 *D7    D11-> Display D6 (13v.l.)
 *D8    D12-> Display D7 (14v.l.)
 *D9    D13-> Display A (15v.l.)
 *.         Display K (16v.l.)->GND
 *.         Display VSS (1v.l.)->GND
 *.         Dispaly VDD (2v.l.)->5V
 *.         Dispaly V0 (3v.l.)->Poti->rest plus minus
 *.         Display RW (5v.l.)->GND
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
