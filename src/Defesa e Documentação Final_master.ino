#include <Adafruit_LiquidCrystal.h>
#include <Wire.h>

#define ID_MASTER 1
#define ID_GARAGEM 2
#define ID_JARDIM 3
#define ID_QUARTO 4

// lcd
Adafruit_LiquidCrystal lcd(0);

//estados
enum
{
  INICIANDO,
  ATUALIZANDO_GARAGEM,
  ESPERANDO_GARAGEM,
  ATUALIZANDO_JARDIM,
  ESPERANDO_JARDIM,
  ATUALIZANDO_QUARTO,
  ESPERANDO_QUARTO,
  ATUALIZANDO_LCD
} estado = INICIANDO;

//vars
int sensorPresenca = 0;
int ultimaPresenca = 0;
int sensorLuz = 0;
int ultimaLuz = 0;
int sensorTemperatura = 0;
int ultimaTemperatura = 0;

void setup()
{
  Wire.begin(ID_MASTER);
  Wire.onReceive(receberMensagem);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void receberMensagem(int bytes)
{
  String msg = "";
  while(Wire.available())
  {
    msg.concat((char)Wire.read());
  }
  Serial.println(msg);
  Serial.println(estado);
  msg.remove(0,1);
  switch(estado)
  {
    case ESPERANDO_GARAGEM:
      sensorPresenca = msg.toInt();
      estado = ATUALIZANDO_JARDIM;
      break;
    case ESPERANDO_JARDIM:
      sensorLuz = msg.toInt();
      estado = ATUALIZANDO_QUARTO;
      break;
    case ESPERANDO_QUARTO:
      sensorTemperatura = msg.toInt();
      estado = ATUALIZANDO_LCD;
      break;
  }
}

void enviarMensagem(byte dado, int destino)
{
  Wire.beginTransmission(destino);
  Wire.write(dado);
  Wire.endTransmission();
}

void iniciar()
{
  lcd.print("Iniciando...");
  estado = ATUALIZANDO_GARAGEM;
}

void atualizarGaragem()
{
  estado = ESPERANDO_GARAGEM;
  enviarMensagem('G', ID_GARAGEM);
}

void atualizarJardim()
{
  estado = ESPERANDO_JARDIM;
  enviarMensagem('J', ID_JARDIM);
}

void atualizarQuarto()
{
  estado = ESPERANDO_QUARTO;
  enviarMensagem('Q', ID_QUARTO);
}

void atualizarLcd()
{
  if(sensorPresenca != ultimaPresenca || sensorLuz != ultimaLuz || sensorTemperatura != ultimaTemperatura)
  {
    ultimaPresenca = sensorPresenca;
    ultimaLuz = sensorLuz;
    ultimaTemperatura = sensorTemperatura;
    lcd.clear();
    String primeiraLinha = "G: " + String(sensorPresenca==1 ? "On" : "Off") + " | J: " + String(sensorLuz==0 ? "On" : "Off");
    String segundaLinha = "Q: " + String(sensorTemperatura) + "C | " + String(sensorTemperatura>25 ? "On" : "Off");
    lcd.print(primeiraLinha);
    lcd.setCursor(0,1);
    lcd.print(segundaLinha);
  }
  estado = ATUALIZANDO_GARAGEM;
}

void loop()
{
  switch(estado)
  {
    case INICIANDO:
      iniciar();
      break;
    case ATUALIZANDO_GARAGEM:
      atualizarGaragem();
      break;
    case ESPERANDO_GARAGEM:
      delay(100);
      break;
    case ATUALIZANDO_JARDIM:
      atualizarJardim();
      break;
    case ESPERANDO_JARDIM:
      delay(100);
      break;
    case ATUALIZANDO_QUARTO:
      atualizarQuarto();
      break;
    case ESPERANDO_QUARTO:
      delay(100);
      break;
    case ATUALIZANDO_LCD:
      atualizarLcd();
      break;
  }
  delay(200);
}