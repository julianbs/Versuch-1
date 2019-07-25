#include <Arduino.h> 
#include <Servo.h> 
//#include <Viktor.h> 
//#include <Display_FredericV1.1.h> 
//#include <Ampel.h> 
#include <LiquidCrystal.h> 
 
 
LiquidCrystal lcd(1,2,6,5,4,3);  //Ansteuerung der Pins, welche mit Display belegt sind 
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
const int servoPin=7; 
const int sicherheitPin=8;
 
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
    Schranke.attach(servoPin); 
    blinkenAG(start,ampel,schranke); 
   
    schrankeSchliessen();

    signalrotJG();
    durchgefahren();
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

void bildschirmanzeigefrederic(char startstate, char schrankestate, char signalRotstate, char ampelstate) //Frederic Böse
 {
    if (startstate=true)
      { 
       lcd.setCursor(0,0); //Startposition
       lcd.print("Zug kommt");
      }
      if (schrankestate=true)
      {
        if (ampelstate=true)
        {
         lcd.setCursor(0,1); //Startposition
         lcd.print("Schranke zu Autoampel Rot");
        }
      }
    if (signalRotstate=true)
    {
     lcd.setCursor(0,2); //Start
     lcd.print("Zugsignal gruen");
    }
    if (signalRotstate==false&&schrankestate==false&&ampelstate==false)
    {
      lcd.setCursor(0,3);
      lcd.print("Zugsignal rot, Schranke offen");
    }
 }

 void blinkenAG(char startstate, char ampelstate, char schrankestate) //Alexander Göhmann
{
//if (startstate == true) //wenn Zug kommt muss Ampel anfangen zu blinken
//{
    for (int i=0; i<6; i++) //Ampel blinkt eine bestimmt Zeit lang
    {
      digitalWrite (13, HIGH); 
      delay (1000);
      digitalWrite (13, LOW); 
      delay (1000);
    };
    ampel =true;
    digitalWrite (13, HIGH); 
    delay (1500); 
}
// }
//  else //Ampel muss ausbleiben wenn kein Zug kommt.
  //{
   // digitalWrite (13, LOW);
 // }



void signalrotJG() //Julien Gutzeit
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

void ausgabe() //Julien Gutzeit
{
 
 lcd.setCursor(0, 0); //Startposition der Darstellung auf dem LCD festlegen. lcd.setCursor(0,0) bedeutet: Erstes Zeichen in der ersten Zeile.
 lcd.print("Schranke offen"); //Dort soll der Text „Schranke.offen“ erscheinen. Der Befehl lcd.setCursor ist dem Mikrocontrollerboard durch das Aufrufen der Bibliothek bekannt.
 lcd.setCursor(0, 1); // lcd.setCursor(0,1) bedeutet: Erstes Zeichen in der zweiten Zeile.
 lcd.print("Signal rot"); //Dort soll dann der Text „Signal.leuchtet“ auftauchen.
 
 //delay(5000);//Eine kurze Pause, in der die LEDsignal an oder aus ist
}

//Wenn nun der Sensorwert bei normaler Helligkeit bspw. nur den Wert 100 hat 
//(Der Wert ist abhängig von den verwendeten Widerständen, von der 
//Helligkeit und von der Stromrichtung), dann nimmt man anstelle des Wertes 
//512 einen wesentlich kleineren Wert, bei dem die LED zu leuchten beginnen
//soll. Bspw. nimmt man dann den Wert 90. Den aktuellen Sensorwert kann 
//man sich nun mit Hilfe des „Serial monitor“ anzeigen lassen. Dazu klickt man 
//oben auf „Tools“ und anschließend auf „serial monitor“. 

void manuelleBetaetigungSchranke()  //Fabian Riekert
{                                    
 
 // tasterstatus=;   //Hier wird der Pin (des Tasters) ausgelesen (Befehl: digitalRead). Das Ergebnis wird unter der Variable „tasterstatus“ mit dem Wert „HIGH“ für 5 Volt und „LOW“ für 0 Volt gespeichert. 
 
if (start==true)           //Verarbeitung: Wenn der Taster gedrückt ist (Das Spannungsignal ist Hoch) 
{                                   //If Schleife Anfang 

delay(1000);
Schranke.attach(servoPin);
Schranke.write(90);                 //Position 2 ansteuern mit dem Winkel 90° (Schranke ist geschlossen) 
delay(1000);
}
else 
{
  manuelleBetaetigungSchranke();
};
 
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
