#include <Arduino.h> 
#include <Servo.h> 
#include <LiquidCrystal.h> 
 
 
LiquidCrystal lcd(1,2,3,4,5,6);  //Ansteuerung der Pins, welche mit Display belegt sind 
Servo Schranke; 
bool start=false; 
bool fehler=false; 
bool schranke=false; 
bool ampel=false; 
bool signalRot=false; 
const int startPin=10; 
const int fehlerPin=9; 
const int ampelPin=13; 
const int signalRotPin=12; 
const int signalGruenPin=11; 
const int lichtSensorPin= 8; 
const int servoPin=7; 
 
int sensorWert=0; 
int eingang= A0; 
const int taster=14;               // Manuelle Betätigung der Schranke? 
int tasterstatus=0;                //Manuelle Betätigung der Schranke? 

 
void setup()  
{ 
  pinMode(startPin, INPUT); 
  pinMode(fehlerPin, OUTPUT); 
  pinMode(ampelPin, OUTPUT); 
  pinMode(signalRotPin, OUTPUT); 
  pinMode(signalGruenPin, OUTPUT); 
  pinMode(sicherheitPin, INPUT);
  pinMode(taster,INPUT); 
  Serial.begin(9600); 
  lcd.begin(16, 2); 
  setSchrankeStart();
  setSignalRot();

} 
 
void loop()  
{ 
  lcd.clear();
  if (digitalRead(startPin)==true) 
  { 
    ausgabe() ;
    delay(2500);
    setStartVar();
    display_Zugkommt(start);
    Schranke.attach(servoPin); 
    blinkenAG(start,ampel,schranke); 
    
    schrankeSchliessen();
    display_schrankegeschlosssen_ampelRot(schranke,ampel)
    signalrotJG();
    display_signalRot(signalRot);
    durchgefahren();
    display_abschlussnachricht(signalRot,schranke,ampel);
  } 
  else if (digitalRead(fehlerPin)==true)
  { 
    blinkenAG(start,ampel,schranke);
    setFehlerVar();
    manuelleBetaetigungSchranke();
    
    signalrotJG();
    durchgefahren();
  }; 
 
}


void schrankeSchliessen() //Viktor Rein
{
    Schranke.write(90);
    delay(1000);
    schranke= true;
}
void schrankeOeffnen() //Viktor Rein
{
    Schranke.write(0);
    delay(1000);
    schranke=false;
}


void blinkenAG(char startstate, char ampelstate, char schrankestate) //Alexander Göhmann
{
 for (int i = 0; i < 6; i++) //Ampel blinkt eine bestimmt Zeit lang
  {
    digitalWrite (13, HIGH);
    delay (1000);
    digitalWrite (13, LOW);
    delay (1000);
  }
  ampel = true; //Ampel leuchtet dauerhaft
  digitalWrite (13, HIGH);
  delay (1500);
}

void signalrotJG() //Julian Gutzeit
{
if (digitalRead(sicherheitPin)==HIGH)
{
  digitalWrite(signalGruenPin, HIGH);
  digitalWrite(signalRotPin, LOW); 
  delay(5000);
}
else 
{
  signalrotJG();
};
}

void ausgabe() 
{
 lcd.setCursor(0, 0); //Startposition der Darstellung auf dem LCD festlegen. lcd.setCursor(0,0) bedeutet: Erstes Zeichen in der ersten Zeile.
 lcd.print("Schranke.offen"); //Dort soll der Text „Schranke.offen“ erscheinen. Der Befehl lcd.setCursor ist dem Mikrocontrollerboard durch das Aufrufen der Bibliothek bekannt.
 lcd.setCursor(0, 1); // lcd.setCursor(0,1) bedeutet: Erstes Zeichen in der zweiten Zeile.
 lcd.print("Signal.leuchtet"); //Dort soll dann der Text „Signal.leuchtet“ auftauchen.
 
 delay(500);//Eine kurze Pause, in der die LEDsignal an oder aus ist
}
//Wenn nun der Sensorwert bei normaler Helligkeit bspw. nur den Wert 100 hat 
//(Der Wert ist abhängig von den verwendeten Widerständen, von der 
//Helligkeit und von der Stromrichtung), dann nimmt man anstelle des Wertes 
//512 einen wesentlich kleineren Wert, bei dem die LED zu leuchten beginnen
//soll. Bspw. nimmt man dann den Wert 90. Den aktuellen Sensorwert kann 
//man sich nun mit Hilfe des „Serial monitor“ anzeigen lassen. Dazu klickt man 
//oben auf „Tools“ und anschließend auf „serial monitor“. 

void manuelleBetaetigungSchranke()  
{                                    
 
  tasterstatus=digitalRead(taster);   //Hier wird der Pin (des Tasters) ausgelesen (Befehl: digitalRead). Das Ergebnis wird unter der Variable „tasterstatus“ mit dem Wert „HIGH“ für 5 Volt und „LOW“ für 0 Volt gespeichert. 
 
  if (tasterstatus == HIGH)           //Verarbeitung: Wenn der Taster gedrückt ist (Das Spannungsignal ist Hoch) 
{                                   //If Schleife Anfang 
Schranke.write(0);                  //Position 1 ansteuern mit dem Winkel 0° (Schranke ist offen) 
delay(3000);                        //Das Programm stoppt für 3 Sekunden 
Schranke.write(90);                 //Position 2 ansteuern mit dem Winkel 90° (Schranke ist geschlossen) 
delay(300000);                      //5min warten, Zug fährt vorbei 
Schranke.write(0);                  //Position 1 ansteuern mit dem Winkel 0° (Schranke ist offen) 
}                                   //If Schleife Ende 
 
else 
{                                   //Else Anfang 
Schranke.write(0);                  //Position 1 ansteuern mit dem Winkel 0° 
} 
} 


void LeuchtiodeAuslesenFuerGeschlosseneSchranke()
{
  
sensorWert=analogRead(eingang); //Die Spannung an dem Fotowiderstand auslesen und unter der Variable „sensorWert“ abspeichern.
//Serial.print(„Sensorwert= „);   //Ausgabe am Serial-Monitor: Das Wort „Sensorwert: „  (Wahrscheinlich nicht Notwendig, Team Mate)
//Serial.println(sensorWert);     //Ausgabe am Serial-Monitor. Mit diesem Befehl „Serial.print“ wird der Sensorwert des Fotowiderstandes in der Form einer Zahl zwischen 0 und 1023 an den serial monitor gesendet. (Wahrscheinlich nicht Notwendig, Team Mate)

if (sensorWert > 512)           //Wenn der Sensorwert über 512 beträgt… . (Zahl abhängig von Widerstand)
{
digitalWrite(signalGruenPin, HIGH);       //soll die Grüne Signal LED leuchten
digitalWrite(signalRotPin, LOW);        //soll die Rote Signal LED nicht leuchten
}

else
{
digitalWrite(signalGruenPin, LOW);        //soll die Grüne Signal LED nicht leuchten 
digitalWrite(signalRotPin, HIGH);       //soll die Rote Signal LED leuchten 
}

}




void setStartVar () //Fabian Riekert
{
  start=true;
  
  
}

void setSchrankeStart()
{
  Schranke.attach(servoPin);
  Schranke.write(0);        //Startposition des Servos ist offen
  delay(1000);
  Schranke.detach();
}

void setSignalRot()
{
  digitalWrite(signalRotPin, HIGH);
}

void durchgefahren()
{
  digitalWrite(signalGruenPin, LOW);
  digitalWrite(signalRotPin, HIGH);
  delay(2000);
  digitalWrite(ampelPin, LOW);
  delay(3000);
  schrankeOeffnen();
}

void setFehlerVar(){
 
    if (digitalRead(startPin)==HIGH){
      
      start=true;
    }
    else
    {
      setFehlerVar();
    }
}

void display_schrankegeschlosssen_ampelRot (char schrankestate, char ampelstate) //Funktion, welche auf Diplay anzeigt wenn die Ampel rot und die Schranke geschlosssen ist
																														     //zur Kontrolle durch Personal 
{
	if (schrankestate==true)
	{
		if(ampelstate==true)
		{
			lcd.setCursor(0,0); //Startposition
			lcd.print("Schranke zu"); //Ausgabe Schranke zu
			lcd.setCursor(0,1);//Startposition 2.Satz
			lcd.print("Ampel rot"); //Ausgabe Ampel rot
			delay(3000);//Verzögerung
			lcd.clear();//Löschen der Nachricht auf Display
		}
	}
}

void diplay_Zugkommt (char startstate) //Funktion die anzeigt,auf Display, wenn ein Zug kommt, zur Kontrolle durch Personal
{
	if(startstate==true)
	{
		lcd.setCursor(0,0); //Startposition 
		lcd.print("!ACHTUNG!"); //Ausgabe einer Warnung
		lcd.setCursor(0,1); //Startposition 2.Satz
		lcd.print("Zug kommt"); //Ausgabe Zug kommt
		delay(3000); //Verzögerung
		lcd.clear(); //Löschen der Nachricht auf Display
	}
}

void display_signalRot(char signalRotstate) //Funktion die Nachricht auf dem Display ausgibt wenn das Signal Rot ist
{
	if(signalRotstate==true)
	{
		lcd.setCursor(0,0) //Startpostition
		lcd.print("Signal rot"); //Ausgabe Signal rot
		delay(3000); //Verzögerung
		lcd.clear(); //Löschen der Nachricht auf Display 
	}
}

void display_abschlussnachricht(char signalRotstate, char schrankestate, char ampelstate) //Funktion die Nachricht auf Display ausgibt wenn BÜ wieder frei ist
{
	if(signalRotstate==false&&schrankestate==false&&ampelstate==false)
	{
		lcd.setCursor(0,0); //Startposition
		lcd.print("Signal rot, Ampel grün"); //Ausgabe Signal rot und Ampel grün
		lcd.setCursor(0,1);//Start 2.Satz
		lcd.print("Schranke offen"); //Ausgabe Schranke offen
		delay(3000); //Verzögerung
		lcd.clear//Löschen der NAchricht auf Diplay
	}
}
