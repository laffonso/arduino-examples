/*
InIoT credentials:
login: luisfernando_affonso@hotmail.com
senha: luis123fernando456


window monitoring

Username:
ae996d2c-8e5a-426c-8186-36c1f123f801

Password:
mqib2Sa7raP3**V

application:
5ccb-47bb-ca7c-7d0d-8a60-72ed
 * 
 */


#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial Janela(D7, D8); //D7 = RX ; D8 = TX

#define WIFI_SSID "WLL-Inatel"                 //Inserir SSID Wifi
#define WIFI_PASSWORD "inatelsemfio"          //Inserir Password Wifi
#define mqtt_server "131.221.240.23"
#define application "5ccb-47bb-ca7c-7d0d-8a60-72ed"

#define DELAY 1000
#define LED D0
WiFiClient espClient;
PubSubClient client(espClient);

#define device_username "ae996d2c-8e5a-426c-8186-36c1f123f801"
#define device_password "mqib2Sa7raP3**V"

long lastMsg = 0;



int value = 0;

String aux = "";
char caractere;



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Janela.print((char)payload[i]);
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
    if (client.connect(clientId.c_str(), device_username, device_password)) {     //Login e senha para o middleware

      //Serial.println("UCHU/E1K3R6h4bW");
      
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      String aux = String(application) + String ( "/labIoT/window1/+" );
      char topic[aux.length()+1];
      aux.toCharArray(topic, aux.length()+1);
      client.subscribe(topic);
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
  Janela.begin(9600);
  pinMode(LED, OUTPUT);
  
  
  // connecta ao Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 18833);
  client.setCallback(callback);
  
}





void loop() 
{
   
   
   if (!client.connected()) {
      reconnect();
   }
   client.loop();
   delay(10);
   /*
   String top = "5be5-b07b-ca7c-7d0e-db66-fdd1/iot_dev/b7b88cfd-6dd5-4e67-8b97-ccfbb258a98d";
   char topic[top.length()+1];
   snprintf(topic, top.length()+1, top);
   top.toCharArray(topic, top.length()+1);

   String str = "{\"temperature\":" + String(temperatura) +" }";
   int lng = str.length();
   char msg[lng+1];
   long now = millis();
   if (now - lastMsg > 2000) {
      lastMsg = now;
      //++value;
      snprintf (msg, lng+1, "{\"temperature\":%.2f }", temperatura);
      Serial.println();
      Serial.print("Publicando temperatura: ");
      Serial.println(str);
      Serial.println(msg);
      client.publish(topic,msg);
   }
   */
   
   

}
