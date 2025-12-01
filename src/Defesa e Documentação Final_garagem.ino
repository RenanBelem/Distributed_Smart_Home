#define led 3
#define sensor 2

#include <Wire.h>

#define ID_MASTER 1
#define ID_GARAGEM 2

enum
{
  ESPERANDO,
  ENVIANDO
} estado = ESPERANDO;

// SENSOR DE PRESENÇA -- LUZ GARAGEM

void receberComando(int bytes)
{
  char comando = Wire.read();
  if(comando=='G')
  {
    estado = ENVIANDO;
  }
}

void enviar()
{
  String msg = "G"+String(digitalRead(sensor));
  Wire.beginTransmission(ID_MASTER);
  Wire.write(msg.c_str());
  Wire.endTransmission();
}

void setup()
{
  pinMode(led, OUTPUT);     
  pinMode(sensor, INPUT); 
  Wire.begin(ID_GARAGEM);
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
  if(digitalRead(sensor) == HIGH)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  delay(200);
}