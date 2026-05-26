# IoT-System mit ESP32 – ESP-NOW, Sensorik, Webvisualisierung & Telegram Bot

## Temperatur- und Luftdruckmessung mit drahtloser Datenübertragung und Echtzeit-Visualisierung

VerfasserInnen: **Marco Duong, Rumeysa Erkan**  
Datum: **13.05.2026**

---

## 1. Einführung

Im Rahmen dieses Projekts wurde ein IoT-System auf Basis von zwei ESP32 Mikrocontrollern entwickelt. Ziel ist die Erfassung, Übertragung und Visualisierung von Sensordaten in Echtzeit.

Dabei werden Temperatur- und Luftdruckwerte mithilfe eines BMP280 Sensors gemessen und drahtlos über ESP-NOW an einen zweiten ESP32 übertragen. Dieser stellt die Daten über ein Webinterface sowie ein OLED Display dar.

Zusätzlich werden die Messwerte durch eine RGB-LED, einen Buzzer sowie einen Telegram Bot visualisiert bzw. extern abrufbar gemacht.

---

## 2. Projektbeschreibung

Das System besteht aus zwei ESP32 Mikrocontrollern:

Der Sender-ESP32 misst kontinuierlich Temperatur und Luftdruck mit einem BMP280 Sensor. Die Daten werden anschließend über ESP-NOW an den Empfänger übertragen.

Der Empfänger-ESP32 verarbeitet die empfangenen Daten und stellt sie über mehrere Ausgabekanäle dar:

- Webserver mit Live-Anzeige
- Webinterface mit Graphen (Chart.js)
- OLED Display (lokal)
- Telegram Bot (Fernabfrage)
- RGB Status-LED
- Buzzer für Warnsignale

Zusätzlich werden historische Durchschnittswerte gespeichert und im Graphen dargestellt.

---

## 3. Projektziel

Ziel des Projekts ist die Umsetzung eines stabilen IoT-Systems, das Sensordaten zuverlässig erfasst, drahtlos überträgt und auf mehreren Plattformen visualisiert.

Ein besonderer Fokus liegt auf der stabilen ESP-NOW Kommunikation zwischen zwei ESP32 Geräten. Zusätzlich soll ein energieeffizienter Betrieb durch optimierte Übertragung und optionalen Sleep-Modus erreicht werden.

---

## 4. Theorie

### 4.1 ESP32
Der ESP32 ist ein Mikrocontroller mit integrierter WLAN- und Bluetooth-Funktion. Er eignet sich besonders für IoT-Anwendungen, da er sowohl Sensoren auslesen als auch drahtlos kommunizieren kann.

---

### 4.2 BMP280 Sensor
Der BMP280 misst Temperatur und Luftdruck über das I2C-Protokoll. Er liefert stabile digitale Messwerte und ist für mobile IoT-Systeme geeignet.

---

### 4.3 ESP-NOW
ESP-NOW ist ein proprietäres Protokoll von Espressif zur direkten Kommunikation zwischen ESP-Geräten ohne WLAN-Router.

Im Projekt wird ESP-NOW verwendet, um Sensordaten vom Sender direkt an den Empfänger zu übertragen. Eine zentrale Herausforderung war die Synchronisation beider Geräte auf demselben WLAN-Kanal.

---

### 4.4 Webserver & Visualisierung
Der ESP32 stellt einen Webserver bereit, über den die Messwerte im Browser angezeigt werden. Zur Visualisierung wird Chart.js verwendet, um Temperatur- und Luftdruckverläufe darzustellen.

---

### 4.5 OLED Display
Das OLED Display zeigt lokale Echtzeitwerte direkt am Gerät an und ermöglicht eine unabhängige Kontrolle ohne Webinterface.

---

### 4.6 Telegram Bot
Der Telegram Bot ermöglicht die externe Abfrage der aktuellen Sensordaten über einen einfachen Befehl (/data).

---

## 5. Arbeitsschritte

### 5.1 Sensorintegration
Zuerst wurde der BMP280 Sensor am Sender-ESP32 angeschlossen und getestet. Die korrekte Ausgabe von Temperatur- und Luftdruckwerten wurde überprüft.

---

### 5.2 ESP-NOW Kommunikation
Anschließend wurde die ESP-NOW Kommunikation zwischen Sender und Empfänger implementiert.

Dabei traten zunächst Probleme bei der Synchronisation auf, insbesondere durch unterschiedliche WLAN-Kanäle. Diese führten zu Sendefehlern.

Nach Anpassung des Kanals konnte eine stabile Verbindung hergestellt werden.

---

### 5.3 Webserver & Graphen
Der Empfänger-ESP32 wurde mit einem Webserver ausgestattet. Die Daten werden als JSON bereitgestellt und im Browser mithilfe von Chart.js visualisiert.

---

### 5.4 Erweiterungen
Zusätzlich wurden folgende Komponenten integriert:

- OLED Display zur lokalen Anzeige
- RGB LED zur Statusanzeige
- KY-006 Buzzer zur Alarmierung
- Telegram Bot zur Fernabfrage

---

## 6. Testphase

Nach der Implementierung wurden alle Systemkomponenten einzeln getestet.

Zuerst wurde der BMP280 Sensor überprüft, um stabile Messwerte sicherzustellen.

Danach wurde die ESP-NOW Kommunikation getestet. Anfangs kam es zu Übertragungsfehlern aufgrund unterschiedlicher WLAN-Kanäle. Nach Korrektur der Kanalzuweisung funktionierte die Kommunikation stabil.

Im Anschluss wurde das Webinterface getestet, indem überprüft wurde, ob sich die Werte korrekt aktualisieren und im Graph dargestellt werden.

Abschließend wurden OLED Display, RGB LED, Buzzer und Telegram Bot erfolgreich getestet.

---

## 7. Komponentenliste

| Komponente | Funktion |
|------------|----------|
| ESP32 (x2) | Steuerung & Kommunikation |
| BMP280 | Temperatur & Luftdruck |
| OLED SSD1306 | Lokale Anzeige |
| RGB LED | Statusanzeige |
| KY-006 Buzzer | Alarm |
| WLAN | Webserver |
| ESP-NOW | Datenübertragung |
| Telegram Bot | Fernabfrage |
| Jumper Kabel | Verbindung |
| Breadboard | Aufbau |

---

## 8. Schaltungsplan

Der Schaltungsplan zeigt den Aufbau des Systems mit Sender- und Empfänger-ESP32 sowie allen angeschlossenen Sensoren und Aktoren.

---

## 9. Code

Der vollständige Code ist im GitHub Repository enthalten:

- Sender: BMP280 + ESP-NOW + Deep Sleep
- Empfänger: Webserver + OLED + RGB + Buzzer + Telegram Bot + Graph

---

## 10. Zusammenfassung

Im Projekt wurde ein funktionierendes IoT-System entwickelt, das Sensordaten erfasst, drahtlos überträgt und auf mehreren Ebenen visualisiert.

Die größte Herausforderung war die stabile Implementierung der ESP-NOW Kommunikation zwischen zwei ESP32 Geräten. Diese konnte durch die korrekte Kanal-Synchronisation erfolgreich gelöst werden.

Zusätzlich wurde das System durch Webvisualisierung, OLED Anzeige, RGB Status LEDs, akustische Warnung und Telegram Integration erweitert.

Das System erfüllt damit alle Anforderungen eines modernen IoT-Messsystems.

---

## 11. Quellen

[1] Random Nerd Tutorials – ESP32 ESP-NOW Guide  
https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

[2] Chart.js Documentation  
https://www.chartjs.org/

[3] Telegram Bot API  
https://core.telegram.org/bots/api

[4] Espressif ESP32 Documentation  
https://docs.espressif.com/
