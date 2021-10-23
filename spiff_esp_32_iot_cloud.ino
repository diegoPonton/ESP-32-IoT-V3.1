#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"


const char* ssid = "MALUPONTON";
const char* password = "Paulette20";

const int verde = 33;
const int blanco = 32;
const int rojo = 27;
const int amarillo = 26;
#define DHTPIN 15     // Digital pin connected to the DHT sensor

#define DHTTYPE    DHT11     // DHT 11

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
String header; // Variable para guardar el HTTP request
String estadoSalida = "off";
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") {
    return readDHTHumidity();
  }
  return String();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("");
  dht.begin();
  pinMode(verde, OUTPUT);
  pinMode(blanco, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  digitalWrite(verde, LOW);
  digitalWrite(blanco, LOW);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.println("Conectando...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  display.clearDisplay();
  display.println(WiFi.localIP());
  display.display();
  // Route for root / web page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/login.html");
  });
  // Route to load style.css file
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", readDHTTemperature().c_str());
  });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/onV", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(verde, HIGH);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });

    server.on("/offV", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(verde, LOW);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });

    server.on("/onB", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(blanco, HIGH);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
    server.on("/offB", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(blanco, LOW);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
  server.on("/onR", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(rojo, HIGH);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
    server.on("/offR", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(rojo, LOW);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
  server.on("/onA", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(amarillo, HIGH);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
    server.on("/offA", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(amarillo, LOW);
    request->send(SPIFFS, "/index.html",String(), false, processor);
  });
  


  



  //  server.on("/onV", HTTP_GET, onV);
  //  server.on("/offV", HTTP_GET, offV);
  //  server.on("/onB", HTTP_GET, onB);
  //  server.on("/offB", HTTP_GET, offB);
  //  server.on("/onR", HTTP_GET, onR);
  //  server.on("/offR", HTTP_GET, offR);
  //  server.on("/onA", HTTP_GET, onA);
  //  server.on("/offA", HTTP_GET, offA);




  // Start server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:


}


void onV() {
  digitalWrite(verde, HIGH);
}

void offV() {
  digitalWrite(verde, LOW);
}
void onB() {
  digitalWrite(blanco, HIGH);
}
void offB() {
  digitalWrite(blanco, LOW);
}
void onR() {
  digitalWrite(rojo, HIGH);
}
void offR() {
  digitalWrite(rojo, LOW);
}
void onA() {
  digitalWrite(amarillo, HIGH);
}
void offA() {
  digitalWrite(amarillo, LOW);
}
