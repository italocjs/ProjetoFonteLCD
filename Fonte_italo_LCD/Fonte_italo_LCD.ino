#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
#include <TouchScreen.h>
#include <dht.h>
#define dht_dpin 13 //Pino DATA ligado na porta 13
dht DHT; 
int tempa; //temperatura anterior
int umida; //umidade anterior

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
SWTFT tft;
#define MINPRESSURE 200
#define MAXPRESSURE 800

float tensao5v;
float tensao5va;
float tensao12v;
float tensao12va;
float tensaoPOT;
float tensaoPOTa;

//12 10 11
#define rele1pin 11
#define rele2pin 12
#define rele3pin 10
boolean rele1 = 0;
boolean rele2 = 0;
boolean rele3 = 0;
int mudou = 1;

unsigned long previousMillisDHT = 0; //TEMPO PARA O SENSOR DE TEMPERATURA
const long intervalDHT = 5000;  

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(rele1pin, OUTPUT);
  pinMode(rele2pin, OUTPUT);
  pinMode(rele3pin, OUTPUT);
  digitalWrite(rele1pin, LOW);
  digitalWrite(rele1pin, LOW);
  digitalWrite(rele1pin, LOW);

  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  Serial.print(F("Found LCD driver chip ID: "));
  Serial.println(identifier, HEX);
  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  delay (100);
  tft.fillScreen(BLACK);
  tft.setTextSize(8);
  tft.setCursor(50,40);
  tft.setTextColor(RED);
  tft.print("Italo");
  tft.setTextSize(4);
  tft.setCursor(80,120);
  tft.setTextColor(RED);
  tft.print(" Soares");
  delay(1000);
  tft.fillScreen(BLACK);
}

void loop()
{
  letouch();
  botao();
  letensao();
  escrevetensao5V();  //so escreve se mudar
  letouch();
  escrevetensao12V(); //so escreve se mudar
  letouch();
  escrevetensaoPOT(); //so escreve se mudar
  escreveDHT();
}

void escreveDHT()
{
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillisDHT >= intervalDHT) {
    // save the last time you blinked the LED 
    previousMillisDHT = currentMillis;   
    DHT.read11(dht_dpin); //Lê as informações do sensor
    int temp = DHT.temperature;  //converte float para int
    int umid = DHT.humidity;  //converte float para int
    if (umid != umida || temp != tempa)
    {
      //tft.fillRect(Xi, Yi, YF , XF, COR);
      tft.fillRect(20, 200, 320, 30, BLACK);

      tft.setCursor(20,200);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.print("TEMP");
      tft.setTextSize(4);
      if (temp <= 35)
      {
        tft.setTextColor(GREEN);
      }
      if (temp > 35)
      {
        tft.setTextColor(RED);
      }
      tft.print(temp);
      tft.print("C");

      tft.print(" ");

      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.print("UMID");
      tft.setTextSize(4);
      if (umid <= 60)
      {
        tft.setTextColor(GREEN);
      }
      if (umid > 60)
      {
        tft.setTextColor(RED);
      }
      tft.print(umid);
      tft.print("%");
      umida = umid;
      tempa = temp; 
    }
  }
}
void letensao()
{
  //  tensao5v = analogRead(A6) * (5.001 / 1023.0) ;
  //  tensao12v = analogRead(A7) * (15.001 / 1023.0);
  //  tensaoPOT = analogRead(A5) * (15.001 / 1023.0);

  for (int i = 0; i < 5; i++)
  {
    tensaoPOT = tensaoPOT + analogRead(A5);
    delay(1);
  }
  tensaoPOT = (tensaoPOT / 5) * (15.001/1023.0);  

  for (int j = 0; j < 5; j++)
  {
    tensao12v = tensao12v + analogRead(A7);
    delay(1);
  }
  tensao12v = (tensao12v / 5) * (15.001/1023.0);  

  for (int k = 0; k < 5; k++)
  {
    tensao5v = tensao5v + analogRead(A6);
    delay(1);
  }
  tensao5v = (tensao5v / 5) * (5.001/1023.0);  

}

void botao()
{
  if (mudou == 1)
  {
    if (rele1 == 1)
    {
      tft.drawRect(280 -1, 10 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 10, 30, 45, GREEN);  //desenha verde 
    }
    if (rele1 == 0)
    {
      tft.drawRect(280 -1, 10 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 10, 30, 45, RED); //desenha vermelho 
    }    

    if (rele2 == 1)
    {
      tft.drawRect(280 -1, 70 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 70, 30, 45, GREEN); //desenha verde 
    }
    if (rele2 == 0)
    {
      tft.drawRect(280 -1, 70 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 70, 30, 45, RED);  //desenha vermelho 
    }    

    if (rele3 == 1)
    {
      tft.drawRect(280 -1, 130 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 130, 30, 45, GREEN);  //desenha verde 
    }
    if (rele3 == 0)
    {
      tft.drawRect(280 -1, 130 -1, 30+2, 45+2, WHITE);
      tft.fillRect(280, 130, 30, 45, RED);   //desenha vermelho 
    }    

    mudou = 0;
  }

}

void letouch()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print("X = "); 
    Serial.print(p.x);
    Serial.print("\tY = "); 
    Serial.print(p.y);
    Serial.print("\tPressure = "); 
    Serial.println(p.z);
    if (p.y > 260 && p.y < 890)
    { 
      if (p.x > 729 && p.x < 872)
      {
        rele1 = !rele1;
        digitalWrite(rele1pin, rele1);
        mudou = 1;
      }
    }
    if (p.x > 526 && p.x < 665)
    {
      rele2 = !rele2;
      digitalWrite(rele2pin, rele2);
      mudou = 1;
    }
    if (p.x > 314 && p.x < 469)
    {
      rele3 = !rele3;
      digitalWrite(rele3pin, rele3); 
      mudou = 1;  
    } 
  }
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
}
void escrevetensao5V()
{
  if ((tensao5v - tensao5va) > 0.15  || (tensao5v - tensao5va) < -0.15 )
  {
    //tft.fillRect(Xi, Yi, YF , XF, COR);
    tft.fillRect(70, 10, 150,45, BLACK);

    tft.setCursor(10,20);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("5v ");
    tft.setCursor(80,10);
    tft.setTextSize(6);

    if (tensao5v < 4.5)
    {
      tft.setTextColor(RED);
    }
    if (tensao5v < 4.8 && tensao5v > 4.5)
    {
      tft.setTextColor(YELLOW);
    }
    if (tensao5v > 4.8 && tensao5v < 5.2)
    {
      tft.setTextColor(GREEN);
    } 
    if (tensao5v > 5.2)
    {
      tft.setTextColor(RED);
    }
    tft.print(tensao5v);
    tensao5va = tensao5v;
  }
}

void escrevetensao12V()
{

  if ((tensao12v - tensao12va) > 0.15  || (tensao12v - tensao12va) < -0.15 )
  {
    //tft.fillRect(Xi, Yi, YF , XF, COR);
    tft.fillRect(70, 70, 190,45, BLACK);

    tft.setCursor(10,80);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("12v");
    tft.setCursor(80,70);
    tft.setTextSize(6);
    if (tensao12v < 11.4)
    {
      tft.setTextColor(RED);
    }
    if (tensao12v < 11.8 && tensao12v > 11.4)
    {
      tft.setTextColor(YELLOW);
    }
    if (tensao12v > 11.8 && tensao5v < 12.4)
    {
      tft.setTextColor(GREEN);
    } 
    if (tensao5v > 12.4)
    {
      tft.setTextColor(RED);
    }
    tft.print(tensao12v);
    tensao12va = tensao12v;
  }
}


void escrevetensaoPOT()
{

  if (tensaoPOT != tensaoPOTa && ((tensaoPOT - tensaoPOTa) > 0.06  || (tensaoPOT - tensaoPOTa) < -0.06 ))
    //  if (tensaoPOT != tensaoPOTa)
  {
    //tft.fillRect(Xi, Yi, YF , XF, COR);
    tft.fillRect(70, 130, 190,45, BLACK);
    tft.setCursor(10,140);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("POT");
    tft.setTextSize(6);
    tft.setTextColor(BLUE);
    tft.setCursor(80,130);
    tft.print(tensaoPOT);
    tensaoPOTa = tensaoPOT;
  }
}




