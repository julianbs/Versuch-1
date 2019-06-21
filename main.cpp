#include <Arduino.h>
#include <Servo.h>


Servo schranke;
int startButtonState;
const int startButtonPin=11;
const int ledPin=13;


void schrankeSchliessen()
{
      
    if (digitalRead(startButtonPin)==HIGH)
    {
        schranke.attach(10);
        schranke.write(90);
        delay(1000);
    };
    schranke.detach();
}

void schrankeOeffnen()
{
 
   if (digitalRead(startButtonPin)==HIGH)
    {
        schranke.attach(10);
        schranke.write(0);
        delay(1000);
    };
  schranke.detach();
}

void schrankeStatus()
{
    int val=schranke.read();
    if (val==90)
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      digitalWrite(ledPin, LOW);
    };
    
}
  
void setup() 
{
  
  schranke.write(0);
  pinMode(startButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  startButtonState= digitalRead(startButtonPin);
  schrankeSchliessen();
  schrankeStatus();
  schrankeOeffnen();
  schrankeStatus();
}