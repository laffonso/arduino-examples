#include<SoftwareSerial.h>    
#include<TinyGPS.h> //biblioteca para utilizar gps    

SoftwareSerial SerialGPS(D4,D3); //D4_RX D3_TX
TinyGPS GPS;  //declaração do gps 

float lat, lon, vel;  //latitude e longitude
unsigned long data, hora; //data e hora

void setup()
{  
  SerialGPS.begin(9600); //Inicia o serial do gps
  Serial.begin(9600); //Inicia o serial para o terminal
  Serial.println("Buscando satelites...");
}

void loop()
{
  //SerialGPS.available verifica se o gps enviou algum dado
  while (SerialGPS.available()) {
    //Verifica se o dado que chegou é válido
    if (GPS.encode(SerialGPS.read())) { 
      GPS.get_datetime(&data, &hora);
      //Imprime a hora:minuto:segundo
      Serial.print((hora / 1000000)-3);
      Serial.print(":");
      Serial.print((hora % 1000000) / 10000);
      Serial.print(":");
      Serial.println((hora % 10000) / 100);
      //Imprime a data dia/mes/ano
      Serial.print(data / 10000);
      Serial.print("/");
      Serial.print((data % 10000) / 100);
      Serial.print("/");
      Serial.println(data % 100);
      //latitude e longitude
      GPS.f_get_position(&lat, &lon);
      Serial.print("Latitude: ");
      Serial.println(lat, 6);
      Serial.print("Longitude: ");
      Serial.println(lon, 6);
    }
  }
}
