/*
* 
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <String.h>

#define WIFI_SSID "WLL-Inatel"                 //Insert SSID Wifi
#define WIFI_PASSWORD "inatelsemfio"          //Insert Password Wifi
#define MQTT_SERVER "131.221.240.23"
#define APPLICATION "5cf5-9521-b035-ff54-517b-cb21" //You should change to your application. You can find by loggin to your user account and accessing http://131.221.240.23:18090/admin-gui/devices or http://131.221.240.23:18090/admin-gui/users
#define DEVICE_USERNAME "2ac7b659-29a3-4c76-b716-9fe5c2244dbb"
#define DEVICE_PASSWORD "rWF_BZ4I3-h5SNl"
#define SUBTOPIC "/home/coffeMaker/listen/" // Topic to subscribe
#define REPLYTOPIC "/home/coffeMaker/reply/" // Topic to publish
#define COFFEE1 "Nespresso"
#define COFFEE2 "Capuccino"
#define COFFEE3 "Java"
#define PINLED D8

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int value = 0;
String aux0 = "";
String aux1 = "";
String aux2 = "";
String replyMessage = "";
char receivedPayload[30];




//Actions to be performed when a message arrives on the subscribed topic
//The client only publishes when it receives a message
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] "); 
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    receivedPayload[i] = (char)payload[i];
    aux0.concat((char)payload[i]);
  }
  aux2 = String(APPLICATION) + String(REPLYTOPIC) + String (DEVICE_USERNAME); 
  char publishTopic[aux2.length()+1];
  aux2.toCharArray(publishTopic, aux2.length()+1);
  Serial.print(aux2);
    
    if (aux0.startsWith("coffe")){
      Serial.println("coffee selected");
      if (receivedPayload[6] == '1'){
        Serial.println("option 1 selected");
        replyMessage = "Making coffee Nespresso";
        char msg[replyMessage.length()+1];
        replyMessage.toCharArray(msg,replyMessage.length()+1); 
        if(client.publish(publishTopic,msg)){
          Serial.println("success on publish");
        }else{
          Serial.println("failed publish");
        }
        digitalWrite(PINLED, HIGH);
        delay(5000);
        digitalWrite(PINLED, LOW);
      }else if(receivedPayload[6] == '2'){
        replyMessage = "Making coffee Capuccino";
        char msg[replyMessage.length()+1];
        replyMessage.toCharArray(msg,replyMessage.length()+1); 
        if(client.publish(publishTopic,msg)){
          Serial.println("success on publish");
        }else{
          Serial.println("failed publish");
        }
        digitalWrite(PINLED, HIGH);
        delay(2000);
        digitalWrite(PINLED, LOW);
      }else{
        replyMessage = "Invalid option, I only have option 1 or 2!";
        char msg[replyMessage.length()+1];
        replyMessage.toCharArray(msg,replyMessage.length()+1);
        client.publish(publishTopic,msg);
      }
    }else{
      //Serial.println("chegou arui");
      replyMessage = "Invalid option, I only have coffee!";
      char msg[replyMessage.length()+1];
      replyMessage.toCharArray(msg,replyMessage.length()+1);
      client.publish(publishTopic,msg);
    }
    aux0 = "";
}

//Sometimes, the connection to the MQTT broker will be lost. This method ensures that the client will reconnect
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client";
    if (client.connect(clientId.c_str(), DEVICE_USERNAME, DEVICE_PASSWORD)) {     //Login and password for middleware
      Serial.println("connected");
      aux1 = String(APPLICATION) + String(SUBTOPIC) + String ( "+" ); 
      char subscribeTopic[aux1.length()+1];
      aux1.toCharArray(subscribeTopic, aux1.length()+1);
      client.subscribe(subscribeTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PINLED, OUTPUT);
  digitalWrite(PINLED, LOW);
  // connecta ao Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); 
  }
  Serial.print("Conected: ");
  Serial.println(WiFi.localIP());
  client.setServer(MQTT_SERVER, 18833);
  client.setCallback(callback);
}

void loop() 
{
   if (!client.connected()) {
      reconnect();
   }
   client.loop();
   delay(10);
}
