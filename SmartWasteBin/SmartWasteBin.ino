#include<SoftwareSerial.h>  
#include<TinyGPS.h> //biblioteca para utilizar gps  
#include<Ultrasonic.h> 
#include "HX711.h"                    // Biblioteca HX711

#define pino_trigger 5
#define pino_echo 6
#define DOUT  A0                      // HX711 DATA OUT = pino A0 do Arduino
#define CLK  A1                       // HX711 SCK IN = pino A1 do Arduino

//SoftwareSerial SerialGPS(4,3); //D4_RX D3_TX
TinyGPS GPS;  //declaração do gps
Ultrasonic ultrasonic(pino_trigger, pino_echo);

HX711 balanca;                // define instancia balança HX711

float calibration_factor = 42130;   // fator de calibração aferido na Calibraçao para balança
String lat="-22.256611", lon="-45.695806";  //latitude e longitude inicializadas coma  localização do laboratório
float latitude, longitude;
unsigned long updatePayloadTime, sendDataTime, data, hora; //data e hora
int fillPercentage=0;
String payload;
float weight=0;
float humi=0;
float temp=0;

void setup()
{
  balanca.begin(DOUT, CLK);                          // inicializa a balança
  Serial2.begin(9600); //Inicia o serial do gps
  Serial.begin(9600); //Inicia o serial para o terminal
  Serial.println("Buscando satelites...");
  sendDataTime = millis();
  updatePayloadTime = millis();
  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
  balanca.tare();   // zera a Balança
}

void loop()
{

  /*
    A cada 20 segundos, faz um novo envio ao In.IoT
  */
  if (millis() - sendDataTime >= 30000){
    sendData(); //Função que envia os dados
    //Serial.println("enviando dado a cada 20000 milissegundos");
    sendDataTime = millis();
  }

  /*
    A cada 10 segundos faz uma atualização do payload
  */
  if (millis() - updatePayloadTime >= 4000){
    updatePayload(); //Atualiza o payload a ser enviado para o In.IoT
    //Serial.println("atualizando payload a cada 5000 milissegundos");
    updatePayloadTime = millis();
  }

  /*
    Sempre que o GPS enviar um novo dado, será atualizada a Latitude e Longitude
  */
  while (Serial2.available()) {
    if (GPS.encode(Serial2.read())) { //Verifica se o dado que chegou é válido
      GPS.f_get_position(&latitude, &longitude);//latitude e longitude
      lat = String(latitude, 6);
      lon = String(longitude, 6);
      Serial2.println(lat);
    }
  }  
  
}



void updatePayload(){
  
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  fillPercentage = map(cmMsec, 49, 0, 0, 100) ; // retornará um valor entre 48 (lixeira vazia) e 6 (lixeira cheia)
  weight = balanca.get_units();
  if(weight < 0) weight = 0;
  if(fillPercentage < 0) fillPercentage = 0;
  
  payload = "{\"lat\":";
  payload += lat;
  payload += ", \"lon\":";
  payload += lon;
  payload += ", \"cap\":\"";
  payload += fillPercentage;
  payload += "\", \"weight\":\"";
  payload += weight;
  payload += "\", \"humi\":\"";
  payload += humi;
  payload += "\", \"temp\":\"";
  payload += temp;
  payload += "\"}";
}

void sendData(){
  //Serial.println();
  Serial.println(payload);
  //Serial.println();
}
