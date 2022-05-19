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

void reconnect() {
 mqttClient.setServer(server, port);
 
  while (!mqttClient.connected()) {
    Serial.print("Intentando conectarse MQTT...");
     String client_id = "PruebaEsp32";
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
  
}

void loop()
{
   if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  
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
      brillar((int)t);
        Serial.print("Temperatura (ºC): ");
        Serial.print(t);
        Serial.println("ºC");
        Serial.print("Humedad %: ");
        Serial.println(h);

  char dist[20];
  char temp[20];
  char hum[20];
  
  dtostrf(distancia,2,2,dist);
  dtostrf(t,2,2,temp);
  dtostrf(h,2,2,hum);


  mqttClient.publish("Carro/SensorReversa/Distancia",dist);
  mqttClient.publish("Casa/Patio/Humedad",hum);
  mqttClient.publish("Casa/Cuarto/Temperatura",temp);
  
  delay(15000);
  
  
}
void brillar(float t){
  
  if(t <= 5.0)
  {//BLANCO
   digitalWrite(ledR, LOW);
   digitalWrite(ledG, LOW);
   digitalWrite(ledB, LOW);
  }
  else if(t > 5.0 && t <= 10.0)
  {//CIAN
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
  }
  else if(t > 10.0 && t <= 15.0)
  {//MORADO
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
  }
  else if(t > 15.0 && t <= 20.0)
  {//AZUL
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
  }
  else if(t > 20.0 && t <= 25.0)
  {//VERDE
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
  }
  
  else if(t > 25.0 && t <= 35.0)
  {//AMARILLO
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
  }
  else if(t > 35.0 ){
    //ROJO 
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
  }
  
}
