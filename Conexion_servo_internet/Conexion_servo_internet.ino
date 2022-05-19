#include <WiFi.h>
#include <HTTPClient.h>
#include <Servo.h>
#include <ArduinoJson.h>
//—–credenciales de red
const char* ssid = "Wifi_name"; //SSID
const char* pass = "Wifi_password"; // password
const char* host = "https://url/GetData.php";

WiFiServer server(80);
WiFiClient client;
HTTPClient http;
Servo servo;

//—– Varibles de tiempo
unsigned long lastTime = 0;
unsigned long timerDelay = 0 ;

#define led 25
#define ledWifi 2
#define servoPin 12

void connectWifi(){
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
}

void setup() {
  
  pinMode(led, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  digitalWrite(led, HIGH);
    
  Serial.begin(115200);

  servo.attach(servoPin);
  
  connectWifi();    
  server.begin();
}


void loop() {
  DynamicJsonDocument doc(1024);

    if (WiFi.status() != WL_CONNECTED){
     connectWifi();
    }
    
  http.begin(host);
  http.addHeader("Content-Type","application/json");
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String entrada = http.getString();
    Serial.println(entrada);
    
    DeserializationError err = deserializeJson(doc, entrada);
    JsonObject obj = doc.as<JsonObject>();
    int posicion_servo = doc[0]["servo"];
    int estado_Led = doc[0]["led"];
    
    
    ejecutar(estado_Led,posicion_servo);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  delay(5000);    
}

void ejecutar(int estado_led,int posicion_servo){
  if(estado_led == 0){
    digitalWrite(led,HIGH);
  }else if(estado_led == 1){
    digitalWrite(led,LOW);
  }
  servo.write(posicion_servo);
}
