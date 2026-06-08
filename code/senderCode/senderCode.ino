/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <esp_wifi.h>
#include <esp_sleep.h>

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 10

#define ESPNOW_CHANNEL 5
Adafruit_BMP280 bmp;

RTC_DATA_ATTR int bootCount = 0;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x00, 0x70, 0x07, 0x7E, 0x7E, 0x20};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float temperatur;
  float druck;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  delay(1000);

bootCount++;
Serial.print("Boot Nummer: ");
Serial.println(bootCount);

  Wire.begin(21, 22);

if (!bmp.begin(0x76)) {
  Serial.println("BMP280 nicht gefunden! Adresse 0x77 testen.");
  while (1);
}
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  myData.temperatur = bmp.readTemperature();
myData.druck = bmp.readPressure() / 100.0F;

Serial.print("Temperatur: ");
Serial.print(myData.temperatur);
Serial.println(" °C");

Serial.print("Druck: ");
Serial.print(myData.druck);
Serial.println(" hPa");

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(300);  // kurz warten, damit ESP-NOW fertig senden kann

Serial.println("Gehe jetzt fuer 10 Sekunden schlafen...");
Serial.flush();

esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
esp_deep_sleep_start();
}
