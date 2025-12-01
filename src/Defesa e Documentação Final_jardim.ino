#define led 3
#define sensor A0

#include <Wire.h>

#define ID_MASTER 1
#define ID_JARDIM 3

enum
{
  ESPERANDO,
  ENVIANDO
} estado = ESPERANDO;

// SENSOR DE LUZ -- LUZ JARDIM

void receberComando(int bytes)
{
  char comando = Wire.read();
  if(comando=='J')
  {
    estado = ENVIANDO;
  }
}

void enviar()
{
  String msg = "J"+String(digitalRead(sensor));
  Wire.beginTransmission(ID_MASTER);
  Wire.write(msg.c_str());
  Wire.endTransmission();
}


void setup()
{
  pinMode(led, OUTPUT);     
  pinMode(sensor, INPUT); 
  Wire.begin(ID_JARDIM);
  Wire.onReceive(receberComando);
  Serial.begin(9600);
}

void loop()
{
  if(estado==ENVIANDO)
  {
    enviar();
    estado = ESPERANDO;
  }
  if(analogRead(sensor) < 100)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  delay(200);
}