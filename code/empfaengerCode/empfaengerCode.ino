/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>

#define ESPNOW_CHANNEL 5
const char* ssid = "ZTE_5GCPE_CE5B";
const char* password = "7L38R7655X";

WebServer server(80);

float aktuelleTemperatur = 0;
float aktuellerDruck = 0;

#define BUZZER_PIN 27
#define TEMP_GRENZE 32.0
#define MAX_DATENPUNKTE 40
#define SPEICHER_INTERVAL 10000

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  float temperatur;
  float druck;
} struct_message;

// Create a struct_message called myData
struct_message myData;
float temperaturHistorie[MAX_DATENPUNKTE];
float druckHistorie[MAX_DATENPUNKTE];
String zeitHistorie[MAX_DATENPUNKTE];

int datenIndex = 0;
int datenAnzahl = 0;
unsigned long letzteSpeicherung = 0;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  aktuelleTemperatur = myData.temperatur;
  aktuellerDruck = myData.druck;

  Serial.print("Temperatur: ");
  Serial.print(aktuelleTemperatur);
  Serial.println(" °C");

  Serial.print("Druck: ");
  Serial.print(aktuellerDruck);
  Serial.println(" hPa");

  if (millis() - letzteSpeicherung >= SPEICHER_INTERVAL) {
    speichereMesswert();
  }

  if (aktuelleTemperatur > TEMP_GRENZE) {
    tone(BUZZER_PIN, 2000);
  } else {
    noTone(BUZZER_PIN);
  }

  Serial.println();
}
 
void handleRoot() {
  String labels = labelsAlsJSON();
  String temperaturDaten = datenAlsJSON(temperaturHistorie);
  String druckDaten = datenAlsJSON(druckHistorie);

  String html = "";
  html += "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='10'>";
  html += "<title>ESP32 Messdaten</title>";
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  html += "<style>";
  html += "body { font-family: Arial; background: #f2f2f2; text-align: center; padding: 20px; }";
  html += ".box { background: white; padding: 20px; border-radius: 10px; max-width: 900px; margin: auto; box-shadow: 0 0 10px #ccc; }";
  html += ".werte { display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; }";
  html += ".wert { font-size: 24px; margin: 15px; }";
  html += ".chartbox { margin-top: 30px; }";
  html += "canvas { max-width: 100%; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='box'>";
  html += "<h1>ESP32 Messdaten</h1>";

  html += "<div class='werte'>";
  html += "<div class='wert'>Temperatur: ";
  html += aktuelleTemperatur;
  html += " &deg;C</div>";
  html += "<div class='wert'>Druck: ";
  html += aktuellerDruck;
  html += " hPa</div>";
  html += "</div>";

  html += "<div class='chartbox'>";
  html += "<h2>Temperatur Verlauf</h2>";
  html += "<canvas id='temperaturChart'></canvas>";
  html += "</div>";

  html += "<div class='chartbox'>";
  html += "<h2>Druck Verlauf</h2>";
  html += "<canvas id='druckChart'></canvas>";
  html += "</div>";

  html += "</div>";

  html += "<script>";
  html += "const labels = ";
  html += labels;
  html += ";";

  html += "const temperaturDaten = ";
  html += temperaturDaten;
  html += ";";

  html += "const druckDaten = ";
  html += druckDaten;
  html += ";";

  html += "new Chart(document.getElementById('temperaturChart'), {";
  html += "type: 'line',";
  html += "data: { labels: labels, datasets: [{ label: 'Temperatur in °C', data: temperaturDaten, borderColor: 'red', backgroundColor: 'rgba(255,0,0,0.1)', tension: 0.3 }] },";
  html += "options: { responsive: true, scales: { y: { beginAtZero: false } } }";
  html += "});";

  html += "new Chart(document.getElementById('druckChart'), {";
  html += "type: 'line',";
  html += "data: { labels: labels, datasets: [{ label: 'Druck in hPa', data: druckDaten, borderColor: 'blue', backgroundColor: 'rgba(0,0,255,0.1)', tension: 0.3 }] },";
  html += "options: { responsive: true, scales: { y: { beginAtZero: false } } }";
  html += "});";

  html += "</script>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void speichereMesswert() {
  letzteSpeicherung = millis();

  temperaturHistorie[datenIndex] = aktuelleTemperatur;
  druckHistorie[datenIndex] = aktuellerDruck;
  zeitHistorie[datenIndex] = String(millis() / 1000) + "s";

  datenIndex++;

  if (datenIndex >= MAX_DATENPUNKTE) {
    datenIndex = 0;
  }

  if (datenAnzahl < MAX_DATENPUNKTE) {
    datenAnzahl++;
  }
}

String datenAlsJSON(float werte[]) {
  String json = "[";

  for (int i = 0; i < datenAnzahl; i++) {
    int index = (datenIndex - datenAnzahl + i + MAX_DATENPUNKTE) % MAX_DATENPUNKTE;
    json += String(werte[index], 2);

    if (i < datenAnzahl - 1) {
      json += ",";
    }
  }

  json += "]";
  return json;
}
String labelsAlsJSON() {
  String json = "[";

  for (int i = 0; i < datenAnzahl; i++) {
    int index = (datenIndex - datenAnzahl + i + MAX_DATENPUNKTE) % MAX_DATENPUNKTE;
    json += "\"";
    json += zeitHistorie[index];
    json += "\"";

    if (i < datenAnzahl - 1) {
      json += ",";
    }
  }

  json += "]";
  return json;
}
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

WiFi.begin(ssid, password);

Serial.print("Verbinde mit WLAN");
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
Serial.print("WLAN-Kanal: ");
Serial.println(WiFi.channel());
Serial.println();
Serial.print("Webserver IP-Adresse: ");
Serial.println(WiFi.localIP());

server.on("/", handleRoot);
server.begin();

Serial.println("Webserver gestartet");

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}
 
void loop() {
  server.handleClient();
}
