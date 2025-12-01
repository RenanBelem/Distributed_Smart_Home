#define fan 3
#define sensor A0

#include <Wire.h>

#define ID_MASTER 1
#define ID_QUARTO 4

enum
{
  ESPERANDO,
  ENVIANDO
} estado = ESPERANDO;

// SENSOR DE TEMPERATURA -- VENTILADOR

void receberComando(int bytes)
{
  char comando = Wire.read();
  if(comando=='Q')
  {
    estado = ENVIANDO;
  }
}

void enviar(int temperature)
{
  String msg = "Q"+String(temperature);
  Wire.beginTransmission(ID_MASTER);
  Wire.write(msg.c_str());
  Wire.endTransmission();
}


void setup()
{  
  pinMode(fan, OUTPUT);
  pinMode(sensor, INPUT);
  Wire.begin(ID_QUARTO);
  Wire.onReceive(receberComando);
  Serial.begin(9600);
}

void fanOn()
{
  digitalWrite(fan, HIGH);
}

void fanOff()
{
  digitalWrite(fan, LOW);
}

int readTemp()
{
  int reading = analogRead(sensor);
  float voltage = reading * 5.0; 
  voltage /= 1024.0;
  int temperature = round((voltage - 0.5) * 100);
  return temperature;
}

void loop()
{
  int temperature = readTemp();
  if(estado==ENVIANDO)
  {
    enviar(temperature);
    estado = ESPERANDO;
  }
  if(temperature > 25)
  {
    fanOn();
  }
  else
  {
    fanOff();
  }
  delay(200);
}