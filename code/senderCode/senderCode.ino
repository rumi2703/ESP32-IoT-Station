// Webserver + ESP-NOW Verbiudng usw Librariers
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <esp_wifi.h>
#include <esp_sleep.h>
#include <DHT.h>

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 10

// Channel für webserver
#define ESPNOW_CHANNEL 5
Adafruit_BMP280 bmp;

#define DHTPIN 23
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define LIGHT_PIN 19
#define PIR_PIN 18

RTC_DATA_ATTR int bootCount = 0;

uint8_t broadcastAddress[] = {0x00, 0x70, 0x07, 0x7E, 0x7E, 0x20};

typedef struct struct_message {
  float temperatur;
  float druck;
  float luftfeuchtigkeit;
  bool hell;
  bool bewegung;
} struct_message;


struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);

  delay(1000);
  pinMode(LIGHT_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  bootCount++;
  Serial.print("Boot Nummer: ");
  Serial.println(bootCount);

  Wire.begin(21, 22);

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 nicht gefunden! Adresse 0x77 testen.");
    while (1);
  }
  dht.begin();

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

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
 
// Ausgabe im serial monitor
void loop() {
  myData.temperatur = bmp.readTemperature();
  myData.druck = bmp.readPressure() / 100.0F;
  myData.luftfeuchtigkeit = dht.readHumidity();
  myData.hell = !digitalRead(LIGHT_PIN);
  myData.bewegung = digitalRead(PIR_PIN);

  Serial.print("Temperatur: ");
  Serial.print(myData.temperatur);
  Serial.println(" °C");

  Serial.print("Druck: ");
  Serial.print(myData.druck);
  Serial.println(" hPa");

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(myData.luftfeuchtigkeit);
  Serial.println(" %");

  Serial.print("Helligkeit: ");
  Serial.println(myData.hell ? "Hell" : "Dunkel");

  Serial.print("Bewegung: ");
  Serial.println(myData.bewegung ? "JA" : "NEIN");

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

  // Deep sleep für 10 sekunden
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}