#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <esp_wifi.h>

Adafruit_BMP280 bmp;

// 👉 MAC vom ESP2 (Empfänger)
uint8_t receiverMAC[] = {0x00,0x70,0x07,0x7E,0x7E,0x20};

typedef struct {
  float temp;
  float pressure;
} Data;

Data data;

#define SLEEP_SEC 5

void onSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SEND OK" : "SEND FAIL");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Fehler");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 11;
  peer.encrypt = false;
  peer.ifidx = WIFI_IF_STA;

  esp_now_add_peer(&peer);

  Wire.begin(21, 22);

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 Fehler");
    while (1);
  }

  data.temp = bmp.readTemperature();
  data.pressure = bmp.readPressure() / 100.0;

  Serial.print("Temp: ");
  Serial.println(data.temp);

  Serial.print("Pressure: ");
  Serial.println(data.pressure);

  Serial.println("Sende Daten...");

  esp_now_send(receiverMAC, (uint8_t*)&data, sizeof(data));

  delay(300);

  esp_sleep_enable_timer_wakeup(SLEEP_SEC * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {}