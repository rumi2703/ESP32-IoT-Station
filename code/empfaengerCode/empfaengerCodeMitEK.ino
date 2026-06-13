#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Internet-Verbindung
#define ESPNOW_CHANNEL 5
const char* ssid = "ZTE_5GCPE_CE5B";
const char* password = "7L38R7655X";

#define OLED_BREITE 128
#define OLED_HOEHE 64
#define OLED_ADRESSE 0x3C

Adafruit_SSD1306 display(OLED_BREITE, OLED_HOEHE, &Wire, -1);

#define BOT_TOKEN "8835609227:AAGd1nO4WETj7A3Xw3400Vs11VEBL-ZlqZM"
#define CHAT_ID "8805115568"

WiFiClientSecure telegramClient;
UniversalTelegramBot bot(BOT_TOKEN, telegramClient);

unsigned long letzteTelegramAbfrage = 0;
#define TELEGRAM_INTERVAL 1000

#define BLYNK_TEMPLATE_ID "TMPL4KTQGnCl4"
#define BLYNK_TEMPLATE_NAME "ESP23Projekt"
#define BLYNK_AUTH_TOKEN "fJAoKAjlCvhE304lDpcGjlDeANnuZFFj"
#include <BlynkSimpleEsp32.h>

WebServer server(80);

// Starttemperatur
float aktuelleTemperatur = 0;
float aktuellerDruck = 0;
float aktuelleLuftfeuchtigkeit = 0;
bool aktuelleHelligkeit = false;
bool aktuelleBewegung = false;
bool statusLedAktiv = true;

BlynkTimer timer;

// PINS + andere Werte
#define BUZZER_PIN 27
#define TEMP_GRENZE 32.0
#define MAX_DATENPUNKTE 40
#define SPEICHER_INTERVAL 10000

#define RGB_ROT_PIN 25
#define RGB_GRUEN_PIN 26
#define RGB_BLAU_PIN 33

// Struktur für die Messwerte, die über ESP-NOW übertragen werden
typedef struct struct_message {
  float temperatur;
  float druck;
  float luftfeuchtigkeit;
  bool hell;
  bool bewegung;
} struct_message;


struct_message myData;
float temperaturHistorie[MAX_DATENPUNKTE];
float druckHistorie[MAX_DATENPUNKTE];
float luftfeuchtigkeitHistorie[MAX_DATENPUNKTE];
String zeitHistorie[MAX_DATENPUNKTE];

int datenIndex = 0;
int datenAnzahl = 0;
unsigned long letzteSpeicherung = 0;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Empfangene ESP-NOW-Daten in die Datenstruktur kopieren
  memcpy(&myData, incomingData, sizeof(myData));

  // Empfangene Messwerte in globale Variablen übernehmen
  aktuelleTemperatur = myData.temperatur;
  aktuellerDruck = myData.druck;
  aktuelleLuftfeuchtigkeit = myData.luftfeuchtigkeit;
  aktuelleHelligkeit = myData.hell;
  aktuelleBewegung = myData.bewegung;
  
  // Status-LED, Display und Blynk nach neuen Messwerten aktualisieren
  aktualisiereStatusLed();
  updateDisplay();
  sendeWerteAnBlynk();

  Serial.print("Temperatur: ");
  Serial.print(aktuelleTemperatur);
  Serial.println(" °C");

  Serial.print("Druck: ");
  Serial.print(aktuellerDruck);
  Serial.println(" hPa");

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(aktuelleLuftfeuchtigkeit);
  Serial.println(" %");

  Serial.print("Helligkeit: ");
  Serial.println(aktuelleHelligkeit ? "Hell" : "Dunkel");

  Serial.print("Bewegung: ");
  Serial.println(aktuelleBewegung ? "JA" : "NEIN");


  // Messwert nur in bestimmten Abständen für den Graphen speichern
  if (millis() - letzteSpeicherung >= SPEICHER_INTERVAL) {
    speichereMesswert();
  }

  if (aktuelleTemperatur > TEMP_GRENZE) { // wenn temperatur höher als 32 grad
    tone(BUZZER_PIN, 2000);
  } else {
    noTone(BUZZER_PIN);
  }

  Serial.println();
}
 
// HTML Code
void handleRoot() {
  String labels = labelsAlsJSON();
  String temperaturDaten = datenAlsJSON(temperaturHistorie);
  String druckDaten = datenAlsJSON(druckHistorie);
  String luftfeuchtigkeitDaten = datenAlsJSON(luftfeuchtigkeitHistorie);

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
  html += "<div class='wert'>Luftfeuchtigkeit: ";
  html += aktuelleLuftfeuchtigkeit;
  html += " %</div>";
  html += "<div class='wert'>Helligkeit: ";
  html += aktuelleHelligkeit ? "Hell" : "Dunkel";
  html += "</div>";
  html += "<div class='wert'>Bewegung: ";
  html += aktuelleBewegung ? "JA" : "NEIN";
  html += "</div>";
  html += "</div>";

  html += "<p>Status LED: ";
  html += statusLedAktiv ? "Ein" : "Aus";
  html += "</p>";

  html += "<form action='/led-toggle' method='GET'>";
  html += "<button type='submit'>";
  html += statusLedAktiv ? "Status LED ausschalten" : "Status LED einschalten";
  html += "</button>";
  html += "</form>";

  html += "<div class='chartbox'>";
  html += "<h2>Temperatur Verlauf</h2>";
  html += "<canvas id='temperaturChart'></canvas>";
  html += "</div>";

  html += "<div class='chartbox'>";
  html += "<h2>Druck Verlauf</h2>";
  html += "<canvas id='druckChart'></canvas>";
  html += "</div>";

  html += "<div class='chartbox'>";
  html += "<h2>Luftfeuchtigkeit Verlauf</h2>";
  html += "<canvas id='luftfeuchtigkeitChart'></canvas>";
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

  html += "const luftfeuchtigkeitDaten = ";
  html += luftfeuchtigkeitDaten;
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

  html += "new Chart(document.getElementById('luftfeuchtigkeitChart'), {";
  html += "type: 'line',";
  html += "data: { labels: labels, datasets: [{ label: 'Luftfeuchtigkeit in %', data: luftfeuchtigkeitDaten, borderColor: 'green', backgroundColor: 'rgba(0,128,0,0.1)', tension: 0.3 }] },";
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
  luftfeuchtigkeitHistorie[datenIndex] = aktuelleLuftfeuchtigkeit;
  zeitHistorie[datenIndex] = String(millis() / 1000) + "s";

  datenIndex++;

  // Älteste Werte werden überschrieben, wenn das Array voll ist
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

// Messwerte als JSON für die API ausgeben
void handleApiData() {
  JsonDocument doc;

  doc["Temperatur"] = myData.temperatur;
  doc["Druck"] = myData.druck;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// OLED-Display mit den aktuellen Messwerten aktualisieren
void updateDisplay() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Temperatur: ");
  display.print(aktuelleTemperatur, 1);
  display.println(" C");

  display.setCursor(0, 12);
  display.print("Druck: ");
  display.print(aktuellerDruck, 1);
  display.println(" hPa");

  display.setCursor(0, 24);
  display.print("Feuchte: ");
  display.print(aktuelleLuftfeuchtigkeit, 1);
  display.println(" %");

  display.setCursor(0, 36);
  display.print("Licht: ");
  display.println(aktuelleHelligkeit ? "Hell" : "Dunkel");

  display.setCursor(0, 48);
  display.print("Bewegung: ");
  display.println(aktuelleBewegung ? "JA" : "NEIN");

  display.display();
}
String statusText() {
  String text = "Aktuelle Messwerte:\n\n";

  text += "Temperatur: ";
  text += String(aktuelleTemperatur, 1);
  text += " C\n";

  text += "Druck: ";
  text += String(aktuellerDruck, 1);
  text += " hPa\n";

  text += "Luftfeuchtigkeit: ";
  text += String(aktuelleLuftfeuchtigkeit, 1);
  text += " %\n";

  text += "Helligkeit: ";
  text += aktuelleHelligkeit ? "Hell\n" : "Dunkel\n";

  text += "Bewegung: ";
  text += aktuelleBewegung ? "JA\n" : "NEIN\n";

  return text;
}

void pruefeTelegramNachrichten() {
  int neueNachrichten = bot.getUpdates(bot.last_message_received + 1);

  while (neueNachrichten) {
    for (int i = 0; i < neueNachrichten; i++) {
      String chatId = bot.messages[i].chat_id;
      String text = bot.messages[i].text;

      if (chatId != CHAT_ID) {
        bot.sendMessage(chatId, "Keine Berechtigung.", "");
        continue;
      }

      if (text == "/status") {
        bot.sendMessage(CHAT_ID, statusText(), "");
      }
    }

    neueNachrichten = bot.getUpdates(bot.last_message_received + 1);
  }
}

void sendeWerteAnBlynk() {
  Blynk.virtualWrite(V0, aktuelleTemperatur);
  Blynk.virtualWrite(V1, aktuellerDruck);
  Blynk.virtualWrite(V2, aktuelleLuftfeuchtigkeit);
  Blynk.virtualWrite(V3, aktuelleHelligkeit ? "Hell" : "Dunkel");
  Blynk.virtualWrite(V4, aktuelleBewegung ? "JA" : "NEIN");
}

void rgbAus() {
  digitalWrite(RGB_ROT_PIN, LOW);
  digitalWrite(RGB_GRUEN_PIN, LOW);
  digitalWrite(RGB_BLAU_PIN, LOW);
}

void setStatusLed(int rot, int gruen, int blau) {
  rgbAus();

  if (!statusLedAktiv) {
    return;
  }

  digitalWrite(RGB_ROT_PIN, rot);
  digitalWrite(RGB_GRUEN_PIN, gruen);
  digitalWrite(RGB_BLAU_PIN, blau);
}

void statusLedWlanOk() {
  setStatusLed(LOW, HIGH, LOW);   // Gruen
}

void statusLedNormal() {
  setStatusLed(LOW, LOW, HIGH);   // Blau
}

void statusLedWarnung() {
  setStatusLed(HIGH, LOW, LOW);   // Rot
}

void aktualisiereStatusLed() {
  if (aktuelleTemperatur > 32.0) {
    statusLedWarnung();
  } else {
    statusLedNormal();
  }
}

void handleLedToggle() {
  statusLedAktiv = !statusLedAktiv;
  aktualisiereStatusLed();

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(RGB_ROT_PIN, OUTPUT);
  pinMode(RGB_GRUEN_PIN, OUTPUT);
  pinMode(RGB_BLAU_PIN, OUTPUT);

  rgbAus();
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADRESSE)) {
    Serial.println("OLED nicht gefunden!");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Warte auf Daten...");
    display.display();
  }

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  
  WiFi.begin(ssid, password);

  // wenn keine wlan verbindung, gebe punkte aus
  Serial.print("Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  statusLedWlanOk();
  delay(1000);

  telegramClient.setInsecure();

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  timer.setInterval(10000L, sendeWerteAnBlynk);

  // Ausgabe wlan kanal + ip adresse
  Serial.print("WLAN-Kanal: ");
  Serial.println(WiFi.channel());
  Serial.println();
  Serial.print("Webserver IP-Adresse: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/led-toggle", handleLedToggle);
  server.on("/data", handleApiData);
  server.begin();

  Serial.println("Webserver gestartet");

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // ESP-NOW-Empfangsfunktion registrieren
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}
 
void loop() {
  // Webserver, Blynk und Telegram dauerhaft bearbeiten
  server.handleClient();
  Blynk.run();
  timer.run();

  if (millis() - letzteTelegramAbfrage > TELEGRAM_INTERVAL) {
    pruefeTelegramNachrichten();
    letzteTelegramAbfrage = millis();
  }
}