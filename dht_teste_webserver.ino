#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
//#include "DHT.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line 


//=============================================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//=============================================================================

const char *ssid = "id";
const char *password = "senha";


#define DHTPIN 4          // Define o pino digital utilizado pelo sensor DHT11
#define DHTTYPE DHT11     // Define o tipo de sensor utilizado (DHT11)

#define BUZZER_PIN 26    // Define o pino digital utilizado pelo buzzer
#define BUTTON_PIN 19     // Define o pino digital utilizado pelo botão
//#define buzzer 26
//#define button 27
#define led    14

DHT dht(DHTPIN, DHTTYPE);
char *temperatura = "Temp:";
char *umidade = "Umid:";
int flag = 0;
float readDHTTemperature();
float readDHTHumidity();

bool estado_buzzer = 0;  // Variável que armazena o estado atual do buzzer

WebServer server(80);
float temperatur;

float humidit;


void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>ESP32 DHT Server!</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
            readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}


void setup() 
{
  Serial.begin(115200);   // Inicializa a comunicação serial
  dht.begin();          // Inicializa o sensor DHT11
  pinMode(BUZZER_PIN, OUTPUT);   // Configura o pino do buzzer como saída
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // Configura o pino do botão como entrada com resistor de pull-up interno
  pinMode(led,OUTPUT);
  lcd.begin();                      // initialize the lcd 
  lcd.backlight();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  if (MDNS.begin("esp32")) {
//    Serial.println("MDNS responder started");
//  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
//    Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
  temperatur = dht.readTemperature();   // Lê a temperatura medida pelo sensor
  humidit = dht.readHumidity();
  Serial.print("Temperatura: ");
  Serial.print(temperatur);
  Serial.println(" °C");
  
      lcd.setCursor(0,0);
      lcd.print(temperatura);  
      lcd.setCursor(strlen(temperatura),0);
      lcd.print(temperatur);  
      lcd.setCursor((strlen(temperatura)+5),0);
      lcd.print(" ");
      lcd.setCursor((strlen(temperatura)+6),0);
      lcd.print("C");

      lcd.setCursor(0,1);
      lcd.print(umidade);  
      lcd.setCursor(strlen(umidade),1);
      lcd.print(humidit);  
      lcd.setCursor((strlen(umidade)+5),1);
      lcd.print(" ");
      lcd.setCursor((strlen(umidade)+6),1);
      lcd.print("%");



  if (temperatur > 28 && estado_buzzer == 0) {     // Se a temperatura for maior que 50°C e o buzzer não estiver ligado, ativa o buzzer
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(led,HIGH);
      estado_buzzer = 1;  // Atualiza a variável de controle do buzzer
    } else if (temperatur <= 28 && estado_buzzer == 1) {  // Caso contrário, se a temperatura estiver abaixo de 50°C e o buzzer estiver ligado, desliga o buzzer
      digitalWrite(BUZZER_PIN, LOW);
      estado_buzzer = 0; // Atualiza a variável de controle do buzzer
    }
  
    if (!(digitalRead(BUTTON_PIN))) {   // Se o botão for pressionado
      if (estado_buzzer==1) {     // Se o buzzer estiver ligado, desliga o buzzer
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(led,LOW);
        estado_buzzer == 0; // Atualiza a variável de controle do buzzer
      }
      // Aguarda um curto período para evitar detecção de múltiplos pressionamentos do botão
      delay(50);
    }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);   // Aguarda 1 segundo antes de realizar a próxima leitura
}
float readDHTTemperature()
{
  float t = temperatur;
  return t;
}
float readDHTHumidity()
{
  float h = humidit;
  return h;
}
