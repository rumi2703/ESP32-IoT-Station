# IoT-System mit ESP32 – ESP-NOW

## Messdatenübertragung zwischen zwei ESP32

**VerfasserInnen:** Marco Duong, Rumeysa Erkan  
**Datum:** 13.06.2026


# 1. Einführung

In diesem Projekt wurde eine IoT-Wetterstation mit zwei ESP32-Mikrocontrollern umgesetzt. Die Messdaten werden auf einem Sender-ESP32 erfasst und über ESP-NOW an einen Empfänger-ESP32 übertragen. Am Empfänger werden die Daten über einen Webserver, ein OLED-Display, Blynk und einen Telegram-Bot angezeigt.



# 2. Projektbeschreibung

Es wurde ein System aus zwei ESP32 aufgebaut. Der erste ESP32 misst Temperatur, Luftdruck, Luftfeuchtigkeit, Helligkeit und Bewegung und sendet diese Daten über ESP-NOW an den zweiten ESP32. Der zweite ESP32 stellt die Messwerte über eine Webseite mit Graphen, eine JSON-API, ein OLED-Display, eine RGB-Status-LED, einem Buzzer, Blynk und Telegram dar.

Die Grundanforderungen ESP-NOW, Webserver, BMP280-Sensor, KY-006 passiver Buzzer, einfacher Graph und Sleep Mode wurden umgesetzt. Zusätzlich wurden ein PIR-Sensor, ein Helligkeitssensor, ein DHT11-Sensor, eine RGB-Status-LED, eine API, ein OLED-Display, ein Telegram-Bot und Blynk integriert.


## 3. Theorie

### ESP32

Der ESP32 ist ein Mikrocontroller mit integriertem WLAN und Bluetooth. Er eignet sich besonders gut für IoT-Projekte, da Sensoren und Aktoren direkt angeschlossen werden können und gleichzeitig Netzwerkfunktionen möglich sind.

### ESP-NOW

ESP-NOW ist ein Kommunikationsprotokoll von Espressif. Damit können ESP32-Mikrocontroller Daten direkt miteinander austauschen, ohne dass beide Geräte mit demselben WLAN verbunden sein müssen. In diesem Projekt wird ESP-NOW verwendet, um die Messdaten vom Sender-ESP32 an den Empfänger-ESP32 zu übertragen.

### Webserver

Auf dem Empfänger-ESP32 wurde ein Webserver eingerichtet. Über die IP-Adresse des Empfängers kann eine Webseite geöffnet werden, auf der die aktuellen Messwerte angezeigt werden. Zusätzlich werden historische Messwerte in einfachen Graphen dargestellt.

### BMP280

Der BMP280 ist ein Sensor zur Messung von Temperatur und Luftdruck. Er wird über I2C mit dem ESP32 verbunden. In diesem Projekt liefert er die Temperatur und den Luftdruck.

### DHT11

Der DHT11 misst Temperatur und Luftfeuchtigkeit. In diesem Projekt wird vor allem die Luftfeuchtigkeit verwendet, da die Temperatur bereits zusätzlich vom BMP280 gemessen wird.

### PIR-Sensor

Ein PIR-Sensor erkennt Bewegung durch Infrarotstrahlung. Im Projekt wird damit festgestellt, ob im Bereich des Sensors Bewegung erkannt wurde.

### Helligkeitssensor

Der Helligkeitssensor erkennt, ob die Umgebung hell oder dunkel ist. Der Zustand wird als Textwert auf der Webseite und in Blynk dargestellt.

### KY-006 passiver Buzzer

Der KY-006 ist ein passiver Buzzer. Er kann mit einer Frequenz angesteuert werden und gibt dadurch einen Ton aus. Im Projekt wird der Buzzer aktiviert, wenn die Temperatur über 32 °C liegt.

### RGB-Status-LED

Eine RGB-LED wird verwendet, um den aktuellen Zustand der Station darzustellen. Es wurde eine Common-Cathode-RGB-LED verwendet. Der gemeinsame Pin ist mit GND verbunden. Die einzelnen Farbpins werden über 220-Ohm-Widerstände mit den GPIO-Pins verbunden.

| Farbe | Bedeutung |
|---|---|
| Grün | WLAN verbunden / Station wurde gestartet |
| Blau | Normalbetrieb / Messdaten wurden empfangen |
| Rot | Warnung, Temperatur über 32 °C |
| Aus | Status-LED wurde über das Webinterface deaktiviert |

Die Status-LED kann zusätzlich über das Webinterface ein- und ausgeschaltet werden.

### Sleep Mode

Der Sender-ESP32 geht nach jeder Messung für 10 Sekunden in den Deep Sleep. Dadurch wird Energie gespart. Nach dem Aufwachen werden die Messwerte erneut erfasst und wieder an den Empfänger gesendet.

### OLED-Display

Das OLED-Display zeigt die aktuellen Messwerte direkt am Empfänger an. Dadurch können die Werte auch ohne Smartphone oder Webseite abgelesen werden.

### Telegram-Bot

Über einen Telegram-Bot können die aktuellen Werte abgefragt werden. Wenn der Befehl `/status` gesendet wird, antwortet der Bot mit Temperatur, Druck, Luftfeuchtigkeit, Helligkeit und Bewegung.

### Blynk

Blynk wird verwendet, um die Messwerte am Smartphone anzuzeigen. Die Werte werden über virtuelle Pins an die Blynk-App gesendet.


## 4. Arbeitsschritte

### 4.1 Aufbau des Sender-ESP32

Am Sender-ESP32 wurden die Sensoren angeschlossen. Der BMP280 wurde über I2C verbunden. Der DHT11, der Helligkeitssensor und der PIR-Sensor wurden an digitale GPIO-Pins angeschlossen.

| Bauteil | ESP32 Pin |
|---|---|
| BMP280 SDA | GPIO 21 |
| BMP280 SCL | GPIO 22 |
| DHT11 | GPIO 23 |
| Helligkeitssensor | GPIO 19 |
| PIR-Sensor | GPIO 18 |

Der Sender misst die Werte und speichert sie in einer Datenstruktur. Danach werden sie über ESP-NOW an die MAC-Adresse des Empfängers gesendet.

### 4.2 Aufbau des Empfänger-ESP32

Am Empfänger-ESP32 wurden Buzzer, RGB-LED und OLED-Display angeschlossen. Zusätzlich verbindet sich der Empfänger mit dem WLAN, damit Webserver, Telegram und Blynk funktionieren.

| Bauteil | ESP32 Pin |
|---|---|
| KY-006 Buzzer | GPIO 27 |
| RGB Rot | GPIO 25 |
| RGB Grün | GPIO 26 |
| RGB Blau | GPIO 33 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

### 4.3 Datenübertragung mit ESP-NOW

Für die Übertragung wurde beim Sender und Empfänger dieselbe Datenstruktur verwendet. Dadurch können alle Messwerte gemeinsam übertragen werden.

```cpp
typedef struct struct_message {
  float temperatur;
  float druck;
  float luftfeuchtigkeit;
  bool hell;
  bool bewegung;
} struct_message;
```

Der Sender speichert die gemessenen Werte in dieser Struktur und sendet sie anschließend mit ESP-NOW an den Empfänger. Am Empfänger werden die Daten wieder ausgelesen und in Variablen gespeichert.

### 4.4 Messung der Sensordaten

Am Sender werden die Messwerte des BMP280, DHT11, Helligkeitssensors und PIR-Sensors eingelesen.

```cpp
myData.temperatur = bmp.readTemperature();
myData.druck = bmp.readPressure() / 100.0F;
myData.luftfeuchtigkeit = dht.readHumidity();
myData.hell = !digitalRead(LIGHT_PIN);
myData.bewegung = digitalRead(PIR_PIN);
```

Die Temperatur und der Luftdruck stammen vom BMP280. Die Luftfeuchtigkeit wird mit dem DHT11 gemessen. Der Helligkeitssensor und der PIR-Sensor liefern digitale Zustände.

### 4.5 Sleep Mode

Nach dem Senden der Messwerte geht der Sender-ESP32 für 10 Sekunden in den Deep Sleep.

```cpp
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
esp_deep_sleep_start();
```

Dadurch wird Energie gespart. Nach Ablauf der Zeit startet der ESP32 automatisch neu, misst erneut und sendet wieder Daten an den Empfänger.

### 4.6 Webserver

Der Empfänger-ESP32 verbindet sich mit dem WLAN und startet einen Webserver. Über die IP-Adresse des Empfängers kann eine Webseite geöffnet werden, auf der die aktuellen Messwerte angezeigt werden.

Die Webseite zeigt folgende Werte an:

- Temperatur
- Luftdruck
- Luftfeuchtigkeit
- Helligkeit
- Bewegung

Zusätzlich werden Temperatur, Luftdruck und Luftfeuchtigkeit als einfache Graphen dargestellt. Dafür werden die letzten Messwerte in Arrays gespeichert.

```cpp
float temperaturHistorie[MAX_DATENPUNKTE];
float druckHistorie[MAX_DATENPUNKTE];
float luftfeuchtigkeitHistorie[MAX_DATENPUNKTE];
String zeitHistorie[MAX_DATENPUNKTE];
```

Es werden maximal 40 Datenpunkte gespeichert. Wenn diese Anzahl überschritten wird, werden die ältesten Werte überschrieben.

### 4.7 Status-LED

Die RGB-LED zeigt den aktuellen Zustand der Station an.

| Farbe | Bedeutung |
|---|---|
| Grün | WLAN verbunden / Station gestartet |
| Blau | Normalbetrieb / Messdaten empfangen |
| Rot | Warnung, Temperatur über 32 °C |
| Aus | Status-LED über Webinterface deaktiviert |

Die LED wurde als Common-Cathode-RGB-LED angeschlossen. Der gemeinsame Pin ist mit GND verbunden. Die Farbpins sind über 220-Ohm-Widerstände mit den GPIO-Pins verbunden.

```cpp
#define RGB_ROT_PIN 25
#define RGB_GRUEN_PIN 26
#define RGB_BLAU_PIN 33
```

Die Status-LED kann über eine Variable aktiviert oder deaktiviert werden.

```cpp
bool statusLedAktiv = true;
```

Wenn die Temperatur über 32 °C steigt, wird Rot angezeigt. Im Normalbetrieb wird Blau angezeigt. Beim Start nach erfolgreicher WLAN-Verbindung leuchtet die LED kurz Grün.

### 4.8 Status-LED im Webinterface ein- und ausschalten

Damit die Status-LED über die Webseite gesteuert werden kann, wurde ein Button im Webinterface eingefügt. Beim Drücken des Buttons wird zwischen aktiv und inaktiv gewechselt.

```cpp
void handleLedToggle() {
  statusLedAktiv = !statusLedAktiv;
  aktualisiereStatusLed();

  server.sendHeader("Location", "/");
  server.send(303);
}
```

Die Route wird im `setup()` registriert.

```cpp
server.on("/led-toggle", handleLedToggle);
```

Auf der Webseite wird angezeigt, ob die Status-LED ein- oder ausgeschaltet ist. Zusätzlich wird ein Button zum Umschalten angezeigt.

### 4.9 JSON-API

Zusätzlich zur Webseite wurde eine einfache API umgesetzt. Dadurch können Messwerte auch als JSON-Daten abgerufen werden.

```cpp
void handleApiData() {
  JsonDocument doc;

  doc["Temperatur"] = myData.temperatur;
  doc["Druck"] = myData.druck;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}
```

Die API ist über den Pfad `/data` erreichbar. Sie kann verwendet werden, um die Daten maschinell weiterzuverarbeiten.

### 4.10 Buzzer

Der passive KY-006-Buzzer wird als akustische Warnung verwendet. Wenn die Temperatur über 32 °C steigt, wird ein Ton ausgegeben.

```cpp
if (aktuelleTemperatur > TEMP_GRENZE) {
  tone(BUZZER_PIN, 2000);
} else {
  noTone(BUZZER_PIN);
}
```

Dadurch wird eine zu hohe Temperatur nicht nur optisch über die RGB-LED, sondern auch akustisch angezeigt.

### 4.11 OLED-Display

Am Empfänger wurde ein OLED-Display angeschlossen. Das Display zeigt die aktuellen Messwerte direkt am Gerät an.

Angezeigt werden:

- Temperatur
- Luftdruck
- Luftfeuchtigkeit
- Helligkeit
- Bewegung

Das Display wird nach jedem neuen Datenempfang aktualisiert. Dadurch sind die Werte auch ohne Webseite oder Smartphone sichtbar.

### 4.12 Telegram-Bot

Ein Telegram-Bot wurde integriert, damit die Messwerte per Nachricht abgefragt werden können. Wenn im Telegram-Chat der Befehl `/status` gesendet wird, antwortet der Bot mit den aktuellen Messwerten.

Beispiel:

```text
Aktuelle Messwerte:

Temperatur: 24.8 C
Druck: 1012.5 hPa
Luftfeuchtigkeit: 45.0 %
Helligkeit: Hell
Bewegung: NEIN
```

Ohne den Befehl `/status` werden keine Messwerte gesendet.

### 4.13 Blynk

Blynk wurde verwendet, um die Messwerte am Smartphone anzuzeigen. Dafür wurden virtuelle Pins angelegt.

| Blynk Pin | Wert |
|---|---|
| V0 | Temperatur |
| V1 | Druck |
| V2 | Luftfeuchtigkeit |
| V3 | Helligkeit |
| V4 | Bewegung |

Die Werte werden regelmäßig an Blynk gesendet und in der App mit passenden Widgets angezeigt.



# 4.14 Schaltungsplan

![Schaltplan](images/SchaltplanGK.png)

Der Schaltplan zeigt die Verbindung der Sensoren und Aktoren mit den GPIO-Pins der beiden ESp32.



# 4.15 Code

Der Code (Sender + Empfänger) befinden sich im Repository.


### 4.16 Komponentenliste

| Komponente | Anzahl | Verwendung |
|---|---:|---|
| ESP32 | 2 | Sender und Empfänger |
| BMP280 | 1 | Temperatur und Luftdruck |
| DHT11 | 1 | Luftfeuchtigkeit |
| PIR-Sensor | 1 | Bewegungserkennung |
| Helligkeitssensor | 1 | Erkennung von hell/dunkel |
| KY-006 passiver Buzzer | 1 | Akustische Warnung |
| RGB-LED Common Cathode | 1 | Statusanzeige |
| OLED-Display | 1 | Anzeige am Gerät |
| 220 Ohm Widerstände | 3 | Schutz der RGB-LED |
| Breadboard | 1 | Aufbau der Schaltung |
| Jumper-Kabel | mehrere | Verbindung der Bauteile |


## 5. Zusammenfassung

Das Projekt wurde erfolgreich umgesetzt. Zwei ESP32 kommunizieren über ESP-NOW miteinander. Der Sender erfasst Temperatur, Luftdruck, Luftfeuchtigkeit, Helligkeit und Bewegung und überträgt diese Daten an den Empfänger. Der Empfänger stellt die Daten über Webserver, Graphen, JSON-API, OLED-Display, Telegram und Blynk dar.

Während der Umsetzung traten kleinere Schwierigkeiten bei der RGB-LED auf. Die LED leuchtete zunächst nicht, da die Beinchen-Zuordnung geprüft werden musste. Nach dem Testen der einzelnen LED-Beinchen konnte die RGB-LED korrekt angeschlossen und als Status-LED verwendet werden.

Zusätzlich wurde festgestellt, dass Blynk für die Smartphone-Anzeige einfacher umzusetzen ist als RemoteXY. Die Messwerte konnten über virtuelle Pins in der Blynk-App dargestellt werden.


# 6. Quellen

[1] „Allnet 4duino_40in1_Kit1 Sensorkit 
https://www.conrad.de/de/p/allnet-4duino-40in1-kit1-sensorkit-1-set-passend-fuer-entwicklungskits-arduino-2233140.html 

[2] ESP-Now Verbindung 
https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/ 

[3] „Chart.js“ 
https://www.w3schools.com/js/js_graphics_chartjs.asp 

[4] ESP32Async/ESPAsyncWebServer. 
https://github.com/ESP32Async/ESPAsyncWebServer 

[5] S. Santos, „ESP32 Deep Sleep with Arduino IDE and Wake Up Sources | Random Nerd Tutorials“. 
https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/

[6] Blynk: a low-code IoT software platform for businesses and developers 
[https://blynk.io]

[7] ESP32 - OLED
[https://esp32io.com/tutorials/esp32-oled]

[8] S. Santos, „ESP32 with DHT11/DHT22 Temperature and Humidity Sensor using Arduino IDE | Random Nerd Tutorials
[https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/]
