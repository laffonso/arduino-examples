

/*
 * 
 * InIoT credentials:

login: luisfernando_affonso@hotmail.com
senha: luis123fernando456

Username:
bb0bd82f-0379-4a57-827f-425a463754bb

Password:
jae9HBxxJTQ8hDD

Application
5cf5-0524-b035-ff54-517b-be1f

 * 
 */


#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <DHT.h>


#define WIFI_SSID "WLL-Inatel"        //Inserir SSID Wifi
#define WIFI_PASSWORD "inatelsemfio"  //Inserir Password Wifi
#define MQTT_SERVER "131.221.240.23"
#define APPLICATION "5cf5-0524-b035-ff54-517b-be1f"
#define DEVICE_USERNAME "bb0bd82f-0379-4a57-827f-425a463754bb"
#define DEVICE_PASSWORD "jae9HBxxJTQ8hDD"
#define PORT 18833




WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial UARTum(D7, D8); //D7 = RX ; D8 = TX



long lastMsg = 0;
int value = 0;
String aux = "";
char caractere;
int t, ph, orp, turbidez;
// 5cf5-0524-b035-ff54-517b-be1f/labIoT/waterSensor/bb0bd82f-0379-4a57-827f-425a463754bb
String top = String(APPLICATION) + String("/labIoT/waterSensor/") + String(DEVICE_USERNAME);



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client";
    //clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), DEVICE_USERNAME, DEVICE_PASSWORD)) {     //Login e senha para o middleware

     
      
      Serial.println("connected");
      /*
      String aux = String(APPLICATION) + String ( "/labIoT/window1/+" );
      char topic[aux.length()+1];
      aux.toCharArray(topic, aux.length()+1);
      client.subscribe(topic);
      */
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}






void setup() {
  Serial.begin(9600);
  UARTum.begin(9600);
  
  
  // connecta ao Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTT_SERVER, PORT);
  client.setCallback(callback);
  
}





void loop() 
{
   
   
   if (!client.connected()) {
      reconnect();
   }
   client.loop();
   delay(5000);
   
   
   char topic[top.length()+1];
   top.toCharArray(topic, top.length()+1);

   /*
   *  The variables below are receiving random values
   *but it could read for real sensors like LM35, DHT11 and a pH sensor
   */
    t = random(0,50);
    ph = random(0,14);
    orp = random(0,14);
    turbidez = 100;
   
  /* Example of how the JSON generated below would look like
   {
      "temperatura":"32",
      "ph":"7",
      "orp":"5",
      "turbidez":"100"
   }
   */
   String str = "{
                  \"temperatura\":" + String(t) +",
                  \"ph\":"+String(ph)+ ",
                  \"orp\":"+String(orp) + ",
                  \"turbidez\":"+String(turbidez) +
                "}";
   Serial.println("Sending...");
   char msg[str.length()+1];
   str.toCharArray(msg, str.length()+1);
   
   if(client.publish(topic,msg)){
      Serial.println("Successfully published");
   }else{
      Serial.println("Something went wrong.");
   }
}
