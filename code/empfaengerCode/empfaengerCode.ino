#include <WiFi.h>
#include <WiFiManager.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- TELEGRAM ----------------
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define BOT_TOKEN "8819922579:AAHs3-QT5F-F0eotEtFexeQEFh6J9vEiKzg"
#define CHAT_ID "8805115568"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ---------------- WEB ----------------
WebServer server(80);

// ---------------- BUZZER ----------------
#define BUZZER 27

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- RGB ----------------
#define RED_PIN 32
#define GREEN_PIN 33
#define BLUE_PIN 25

// ---------------- DATA ----------------
typedef struct {
  float temp;
  float pressure;
} Data;

Data incoming;

float lastTemp = 0;
float lastPressure = 0;

// ---------------- BUFFER ----------------
#define BUFFER_SIZE 5
float tempBuffer[BUFFER_SIZE];
float pressureBuffer[BUFFER_SIZE];
int indexBuf = 0;

// ---------------- BUZZER ----------------
void beep() {
  ledcWriteTone(BUZZER, 2000);
  delay(150);
  ledcWriteTone(BUZZER, 0);
}

// ---------------- RGB ----------------
void setRGB(int r, int g, int b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void blinkPink() {
  for (int i = 0; i < 3; i++) {
    setRGB(1,0,1);
    delay(200);
    setRGB(0,0,0);
    delay(200);
  }
}

// ---------------- ESP-NOW ----------------
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incoming, data, sizeof(incoming));

  tempBuffer[indexBuf] = incoming.temp;
  pressureBuffer[indexBuf] = incoming.pressure;

  indexBuf++;
  if (indexBuf >= BUFFER_SIZE) indexBuf = 0;

  float t=0,p=0;
  for(int i=0;i<BUFFER_SIZE;i++){
    t += tempBuffer[i];
    p += pressureBuffer[i];
  }

  lastTemp = t / BUFFER_SIZE;
  lastPressure = p / BUFFER_SIZE;

  setRGB(0,0,1);

  if (lastTemp > 30) {
    beep();
    blinkPink();

    // 🔥 OPTIONAL TELEGRAM ALERT
    bot.sendMessage(CHAT_ID,
      "⚠️ ALARM!\nTemp: " + String(lastTemp) +
      "\nPressure: " + String(lastPressure),
      "");
  }
}

// ---------------- TELEGRAM HANDLER ----------------
void handleTelegram() {
  int msgCount = bot.getUpdates(bot.last_message_received + 1);

  while (msgCount) {
    for (int i = 0; i < msgCount; i++) {
      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      if (text == "/data") {
        String msg = "🌡 Temp: " + String(lastTemp) + " °C\n";
        msg += "🌪 Pressure: " + String(lastPressure) + " hPa";

        bot.sendMessage(chat_id, msg, "");
      }
    }

    msgCount = bot.getUpdates(bot.last_message_received + 1);
  }
}

// ---------------- WEB DATA ----------------
void handleData() {
  String json = "{";
  json += "\"temp\":" + String(lastTemp) + ",";
  json += "\"pressure\":" + String(lastPressure);
  json += "}";
  server.send(200, "application/json", json);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  setRGB(1,0,0);

  Wire.begin(21,22);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  WiFiManager wm;
  wm.autoConnect("ESP32_Setup","12345678");

  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);

  client.setInsecure(); // 🔥 wichtig für Telegram

  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(onReceive);

  server.on("/data", handleData);
  server.begin();

  setRGB(0,1,0);
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();
  handleTelegram();

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("ESP32");
  display.println(lastTemp);
  display.println(lastPressure);
  display.display();
}