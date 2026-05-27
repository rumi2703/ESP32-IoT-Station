/*
  Funktionen:
  - empfängt BMP280 Daten via ESP-NOW
  - zeigt Daten im Webserver (2 Graphen)
  - OLED Anzeige
  - RGB Status LED
  - PIR Bewegung + Telegram
  - LDR Helligkeit
  - Buzzer Alarm
*/

#include <WiFi.h>
#include <WiFiManager.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ---------------- OLED DISPLAY ----------------
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ---------------- SENSOR PINS ----------------
#define PIR_PIN 14      // Bewegungssensor
#define LDR_PIN 34      // Lichtsensor
#define BUZZER 27       // Summer

// ---------------- RGB LED PINS ----------------
#define RED_PIN 32
#define GREEN_PIN 26
#define BLUE_PIN 25

// ---------------- TELEGRAM BOT ----------------
#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

// Telegram Verbindung
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ---------------- WEB SERVER ----------------
WebServer server(80);

// ---------------- ESP-NOW DATA STRUCT ----------------
typedef struct {
  float temp;       // Temperatur vom Sender
  float pressure;   // Luftdruck vom Sender
} Data;

Data incoming;

// ---------------- AKTUELLE WERTE ----------------
float lastTemp = 0;
float lastPressure = 0;

// ---------------- HISTORY FÜR GRAPHEN ----------------
#define MAX_HISTORY 40

float tempHistory[MAX_HISTORY];
float pressHistory[MAX_HISTORY];

int histIndex = 0;
int histCount = 0;

// ---------------- PIR STATUS ----------------
bool motionDetected = false;
bool lastMotion = false;

// ---------------- LDR STATUS ----------------
int lightValue = 0;
String lightState = "";

// ---------------- RGB STATUS ----------------
bool blinkState = false;
unsigned long lastBlink = 0;

// ---------------- RGB FUNKTION ----------------
void setRGB(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

// ---------------- RGB LOGIK ----------------
// Temperaturabhängige Farbanzeige
void updateRGB(float t) {

  // ALARM MODE (>30°C) → rotes Blinken
  if (t > 30.0) {
    if (millis() - lastBlink > 400) {
      blinkState = !blinkState;
      lastBlink = millis();
    }
    setRGB(blinkState, 0, 0);
    return;
  }

  // WARM (29-30°C)
  if (t > 29.0) {
    setRGB(1, 0, 0);
    return;
  }

  // NORMAL WARM (27-29°C)
  if (t > 27.0) {
    setRGB(1, 1, 0);
    return;
  }

  // NORMAL (<27°C)
  setRGB(0, 0, 1);
}

// ---------------- BUZZER ALARM ----------------
bool alarmActive = false;
unsigned long lastBeep = 0;

void updateBuzzer() {

  if (lastTemp > 30) {
    alarmActive = true;
  } else {
    alarmActive = false;
    ledcWriteTone(BUZZER, 0);
  }

  if (alarmActive && millis() - lastBeep > 800) {
    ledcAttach(BUZZER, 2000, 8);
    ledcWriteTone(BUZZER, 2000);

    delay(100);
    ledcWriteTone(BUZZER, 0);

    lastBeep = millis();
  }
}

// ---------------- ESP-NOW RECEIVE CALLBACK ----------------
// wird automatisch aufgerufen wenn Daten ankommen
void onReceive(const esp_now_recv_info_t *info,
               const uint8_t *data,
               int len) {

  // Daten in Struktur kopieren
  memcpy(&incoming, data, sizeof(incoming));

  // Werte speichern
  lastTemp = incoming.temp;
  lastPressure = incoming.pressure;

  // Historie für Graphen speichern
  tempHistory[histIndex] = lastTemp;
  pressHistory[histIndex] = lastPressure;

  histIndex++;
  if (histIndex >= MAX_HISTORY) histIndex = 0;

  if (histCount < MAX_HISTORY) histCount++;

  // Systeme aktualisieren
  updateRGB(lastTemp);
  updateBuzzer();
}

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  // Pins definieren
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  // OLED starten
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // WLAN Setup Portal
  WiFiManager wm;
  wm.autoConnect("ESP32_Setup", "12345678");

  // ESP-NOW Setup
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);

  client.setInsecure();

  esp_now_init();
  esp_now_register_recv_cb(onReceive);

  // Webserver Routes
  server.on("/", handleRoot);
  server.on("/history", handleHistory);
  server.begin();

  Serial.println("SYSTEM READY");
}

// ---------------- LOOP ----------------
void loop() {

  server.handleClient();
}
