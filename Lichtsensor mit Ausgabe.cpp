#include <LiquidCrystal.h> //LCD-Bibliothek laden

LiquidCrystal lcd(2, 1, 6, 5, 4, 3); //In dieser Zeile wird festgelegt, welche Pins des Mikrocontrollerboards für das LCD verwendet wird (Am besten erstmal nicht verändern).


int eingang= A0; //Das Wort „eingang“ steht jetzt für den Wert „A0“ (Bezeichnung vom Analogport 0)
int LEDsignal = 12; //Das Wort „LEDsignal“ steht jetzt für den Wert 12
int sensorWert = 0; //Variable für den Sensorwert mit 0 als Startwert

void setup()//Hier beginnt das Setup.
{
Serial.begin(9600); //Die Kommunikation mit dem seriellen Port wird gestartet. Das benötigt man, um sich den tatsächlich ausgelesenen Wert später im serial monitor anzeigen zu lassen.
pinMode (LEDsignal, OUTPUT); //Der Pin mit der LEDsignal (Pin 12) ist jetzt ein Ausgang
//Der analoge Pin muss nicht definiert werden.
}

void loop()
{//Mit dieser Klammer wird der Loop-Teil geöffnet.
sensorWert =analogRead(eingang); //Die Spannung an dem Fotowiderstand auslesen und unter der Variable „sensorWert“ abspeichern.
Serial.print("Sensorwert = " ); //Ausgabe am Serial-Monitor: Das Wort „Sensorwert: „
Serial.println(sensorWert); //Ausgabe am Serial-Monitor. Mit dem Befehl Serial.print wird der Sensorwert des Fotowiderstandes in Form einer Zahl zwischen 0 und 1023 an den serial monitor gesendet.

if (sensorWert > 512 ) //Wenn der Sensorwert über 512 beträgt….
{
digitalWrite(LEDsignal, LOW); //…soll die LEDsignal nicht leuchten…
}

else //andernfalls…
{
digitalWrite(LEDsignal, HIGH); //….soll sie leuchten.



                                                  //Display Ausgabe starten
void setup() 
{
lcd.begin(15, 2); //Im Setup wird angegeben, wie viele Zeichen und Zeilen verwendet werden. Hier: 15 Zeichen in 2 Zeilen.
}

void loop() 
{
lcd.setCursor(0, 0); //Startposition der Darstellung auf dem LCD festlegen. lcd.setCursor(0,0) bedeutet: Erstes Zeichen in der ersten Zeile.
lcd.print("Schranke.offen"); //Dort soll der Text „Schranke.offen“ erscheinen. Der Befehl lcd.setCursor ist dem Mikrocontrollerboard durch das Aufrufen der Bibliothek bekannt.
lcd.setCursor(0, 1); // lcd.setCursor(0,1) bedeutet: Erstes Zeichen in der zweiten Zeile.
lcd.print("Signal.leuchtet"); //Dort soll dann der Text „Signal.leuchtet“ auftauchen.
}

                                                 //Display Ausgabe ende

}

delay (500);//Eine kurze Pause, in der die LEDsignal an oder aus ist

}//Mit dieser letzten Klammer wird der Loop-Teil geschlossen.

//Wenn nun der Sensorwert bei normaler Helligkeit bspw. nur den Wert 100 hat 
//(Der Wert ist abhängig von den verwendeten Widerständen, von der 
//Helligkeit und von der Stromrichtung), dann nimmt man anstelle des Wertes 
//512 einen wesentlich kleineren Wert, bei dem die LED zu leuchten beginnen
//soll. Bspw. nimmt man dann den Wert 90. Den aktuellen Sensorwert kann 
//man sich nun mit Hilfe des „Serial monitor“ anzeigen lassen. Dazu klickt man 
//oben auf „Tools“ und anschließend auf „serial monitor“. 
