#include <WiFi.h>
#include <DHT.h>
#include "ThingSpeak.h"

//—–credenciales de red
const char* ssid = "Wifi_name"; //SSID
const char* pass = "Wifi_password"; // password

WiFiServer server(80);
WiFiClient client;

//—–ThingSpeak detalles del canal
unsigned long myChannelNumber = 00000000;
const char * myWriteAPIKey = "apikey";

//—– Varibles de tiempo
unsigned long lastTime = 0;
unsigned long timerDelay = 15000 ;

//—-DHT declaraciones 
#define DHTTYPE DHT11
#define DHTPIN 12

//Inicializar el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

//--Pines del led RGB
#define led_R 27
#define led_G 26
#define led_B 25
#define ledWifi 2

void setup() {
  //LED RGB
  pinMode(led_R, OUTPUT);
  pinMode(led_G, OUTPUT);
  pinMode(led_B, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  Serial.begin(115200);
  
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      digitalWrite(ledWifi,HIGH);
      delay(1000);
      digitalWrite(ledWifi,LOW);
      delay(1000);
  }
  // Imprimir local IP address y inicializar servidor web
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(ledWifi,LOW);    
  server.begin();
  //-Inicializar dht11
  dht.begin();
  ThingSpeak.begin(client); // ThingSpeak

}


void loop() {

  if ((millis() - lastTime) > timerDelay) {

      if (WiFi.status() != WL_CONNECTED){
        Serial.print("Volviendo a conectar");
        Serial.println(ssid);
        WiFi.begin(ssid, pass);
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            digitalWrite(ledWifi,HIGH);
            delay(1000);
            digitalWrite(ledWifi,LOW);
            delay(1000);
        }
        // Imprimir local IP address y inicializar servidor web
        Serial.println("");
        Serial.println("WiFi conectado.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        digitalWrite(ledWifi,LOW);
      }
      
      //lecturas
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float s = dht.computeHeatIndex(t,h,false);
      brillar((int)t);
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Error al leer datos del sensor!"));
        return;
      }else{
        Serial.print("Temperatura: ");
        Serial.print(t);
        Serial.println(" ºC");
        Serial.print("Humedad: ");
        Serial.print(h);
        Serial.println(" %");
        Serial.print("Sensación Térmica: ");
        Serial.print(s);
        Serial.println(" ºC");
        
        ThingSpeak.setField(1, t);
        ThingSpeak.setField(2, h);
        ThingSpeak.setField(3, s);
    
        int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
    
        if(x == 200){
          Serial.println("DATOS SUBIDOS CORRECTAMENTE");
        }else{
          Serial.println("ERROR AL SUBIR DATOS. HTTP error code " + String(x));
        }
      }

  
  lastTime = millis();

  }
  
}

void brillar(float t){
  
  if(t <= 5.0)
  {//BLANCO
   digitalWrite(led_R, LOW);
   digitalWrite(led_G, LOW);
   digitalWrite(led_B, LOW);
  }
  else if(t > 5.0 && t <= 10.0)
  {//CIAN
    digitalWrite(led_R, HIGH);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, LOW);
  }
  else if(t > 10.0 && t <= 15.0)
  {//MORADO
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, LOW);
  }
  else if(t > 15.0 && t <= 20.0)
  {//AZUL
    digitalWrite(led_R, HIGH);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, LOW);
  }
  else if(t > 20.0 && t <= 25.0)
  {//VERDE
    digitalWrite(led_R, HIGH);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, HIGH);
  }
  
  else if(t > 25.0 && t <= 35.0)
  {//AMARILLO
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, HIGH);
  }
  else if(t > 35.0 ){
    //ROJO 
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, HIGH);
  }
  
}
