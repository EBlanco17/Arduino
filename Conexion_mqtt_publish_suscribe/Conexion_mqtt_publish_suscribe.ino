#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
//Ingresar libreria PubSubClient

WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

const char* ssid     = "Wifi_name";
const char* password = "Wifi_pass";

char *server = "broker.emqx.io";
int port = 1883;

int Trigger =13;
int Echo = 12;

int ledR= 27;
int ledG= 26;
int ledB= 25;


int var = 0;

char datos[40];
char datos2[40];
char datos3[40];
char datos4[40];
String resultS = "";

float tiempo;
float distancia;

//—-DHT declaraciones 
#define DHTTYPE DHT11
#define DHTPIN 32

//Inicializar el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

void wifiInit() {
    Serial.print("Conectándose a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
        delay(500);  
    }
    Serial.println("");
    Serial.println("Conectado a WiFi");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
  }

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  char payload_string[length + 1];
  
  int resultI;

  memcpy(payload_string, payload, length);
  payload_string[length];
  resultI = atoi(payload_string);
  
  var = resultI;

  resultS = "";
  
  for (int i=0;i<length;i++) {
    resultS= resultS + (char)payload[i];
  }
  Serial.println();
}



void reconnect() {
 mqttClient.setServer(server, port);
 mqttClient.setCallback(callback);
  while (!mqttClient.connected()) {
    Serial.print("Intentando conectarse MQTT...");
     String client_id = "nombreServerMqtt";
     client_id += String(WiFi.macAddress());
    if (mqttClient.connect(client_id.c_str())) {
     Serial.println("Conectado");

     
      
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" intentar de nuevo en 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  mqttClient.subscribe("Entrada/colorLed");
}

void setup()
{
 
  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(ledB,OUTPUT);
  pinMode(Trigger,OUTPUT);
  pinMode(Echo,INPUT);
   //-Inicializar dht11
  dht.begin();

  digitalWrite(ledR,HIGH);
  digitalWrite(ledG,HIGH);
  digitalWrite(ledB,HIGH);
  
  Serial.begin(115200);
  delay(10);
  wifiInit();
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
}

void loop()
{
   if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  Serial.print("Color Led: ");
  Serial.println(resultS);
  luces(resultS);

digitalWrite(Trigger,LOW);
delayMicroseconds(4);
digitalWrite(Trigger,HIGH);
delayMicroseconds(10);
digitalWrite(Trigger,LOW);
tiempo = pulseIn(Echo,HIGH);
distancia = tiempo/58.03;
Serial.print("Distancia: ");
  Serial.println(distancia);

//lecturas
      float h = dht.readHumidity();
      float t = dht.readTemperature();

        Serial.print("Temperatura (ºC): ");
        Serial.print(t);
        Serial.println("ºC");
        Serial.print("Humedad %: ");
        Serial.println(h);




  sprintf(datos2,"Valor de distancia: %lf",distancia);
  sprintf(datos3,"Valor de humedad: %lf",h);
  sprintf(datos4,"Valor de temperatura: %lf",t);
  
  mqttClient.publish("Carro/SensorReversa/Distancia",datos2);
  mqttClient.publish("Casa/Patio/Humedad",datos3);
  mqttClient.publish("Casa/Cuarto/Temperatura",datos4);
  
  delay(5000);
  
  
}
void luces (String color){
Serial.println(color);
if (color == "BLANCO"){
  
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,LOW);
  digitalWrite(ledB,LOW);

}else if (color == "CELESTE"){
  
  digitalWrite(ledR,HIGH);
  digitalWrite(ledG,LOW);
  digitalWrite(ledB,LOW);

}else if (color == "AZUL"){
  
  digitalWrite(ledR,HIGH);
  digitalWrite(ledG,HIGH);
  digitalWrite(ledB,LOW);

}else if (color == "MORADO"){
  
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,HIGH);
  digitalWrite(ledB,LOW);

}else if (color == "VERDE"){
  
  digitalWrite(ledR,HIGH);
  digitalWrite(ledG,LOW);
  digitalWrite(ledB,HIGH);

}else if (color == "AMARILLO"){
  
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,LOW);
  digitalWrite(ledB,HIGH);

}else if (color == "ROJO"){
  
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,HIGH);
  digitalWrite(ledB,HIGH);

}else if (color == "NEGRO"){
  
  digitalWrite(ledR,HIGH);
  digitalWrite(ledG,HIGH);
  digitalWrite(ledB,HIGH);

}

  
}
