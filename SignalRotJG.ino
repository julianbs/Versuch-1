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
