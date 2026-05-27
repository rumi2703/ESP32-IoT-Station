/*
  Aufgabe:
  - misst Temperatur + Druck
  - sendet Daten an Receiver via ESP-NOW
  - spart Energie durch Deep Sleep
*/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// ---------------- SENSOR ----------------
Adafruit_BMP280 bmp;

// ---------------- DATA STRUCT ----------------
// Diese Struktur wird 1:1 an den Receiver gesendet
typedef struct {
  float temp;       // Temperatur in °C
  float pressure;   // Luftdruck in hPa
} Data;

Data dataToSend;

// ---------------- RECEIVER MAC ----------------
// Adresse des Empfänger-ESP32 (ESP-NOW Zielgerät)
uint8_t receiverMAC[] = {0x00, 0x70, 0x07, 0x7E, 0x7E, 0x20};

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  // WLAN Modus für ESP-NOW aktivieren
  WiFi.mode(WIFI_STA);

  // Fixe Funkkanalwahl (muss bei beiden Geräten gleich sein)
  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);

  // ESP-NOW initialisieren
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW INIT FAILED");
    return;
  }

  // Empfänger hinzufügen
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 11;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("PEER ADD FAILED");
    return;
  }

  // BMP280 starten (I2C Sensor für Temperatur & Druck)
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 NOT FOUND");
    while (1);
  }

  // ---------------- SENSOR READ ----------------

  // Temperatur auslesen (°C)
  float t = bmp.readTemperature();

  // Druck auslesen (Pa → hPa umrechnen)
  float p = bmp.readPressure() / 100.0F;

  // Daten in Struktur speichern
  dataToSend.temp = t;
  dataToSend.pressure = p;

  // ---------------- SEND DATA ----------------
  esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));

  Serial.println("DATA SENT:");
  Serial.print("Temp: ");
  Serial.println(t);
  Serial.print("Pressure: ");
  Serial.println(p);

  // ---------------- DEEP SLEEP ----------------
  // ESP spart Energie und wacht nach 10 Sekunden wieder auf
  esp_sleep_enable_timer_wakeup(10 * 1000000);
  esp_deep_sleep_start();
}

// Loop wird nicht benutzt (Deep Sleep System)
void loop() {}
