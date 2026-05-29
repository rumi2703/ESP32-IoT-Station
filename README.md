# IoT-System mit ESP32 – ESP-NOW, Sensorik, Webvisualisierung & Telegram Bot

## Temperatur- und Luftdruckmessung mit drahtloser Datenübertragung und Echtzeit-Visualisierung

**VerfasserInnen:** Marco Duong, Rumeysa Erkan  
**Datum:** 13.05.2026  

---

# 1. Einführung

Im Rahmen dieses Projekts wurde ein IoT-System auf Basis von zwei ESP32 Mikrocontrollern entwickelt.

Ziel des Systems ist die Erfassung, drahtlose Übertragung und Visualisierung von Sensordaten in Echtzeit.

Dabei werden Temperatur- und Luftdruckwerte mithilfe eines BMP280 Sensors erfasst und über ESP-NOW an einen zweiten ESP32 übertragen [3].

Der Empfänger verarbeitet diese Daten und stellt sie über mehrere Ausgabekanäle dar:

- Webserver mit Live-Daten
- Chart.js Graphen
- OLED Display
- RGB Status LED
- Buzzer Alarm
- Telegram Bot (/status + PIR Meldungen)

---

# 2. Systemaufbau

Das System besteht aus zwei ESP32 Mikrocontrollern:

## Sender (Sensor Node)
- BMP280 Sensor zur Messung von Temperatur und Luftdruck [8]
- ESP-NOW Datenübertragung
- Deep Sleep zur Energieeinsparung

## Empfänger (Control Node)
- Empfang der Sensordaten
- Verarbeitung und Speicherung
- Webserver zur Darstellung
- Steuerung von OLED, RGB LED und Buzzer
- PIR und LDR Sensorintegration
- Telegram Benachrichtigungen

---

# 3. Projektziel

Ziel ist die Entwicklung eines stabilen IoT-Systems, das:

- Sensordaten zuverlässig erfasst
- drahtlos über ESP-NOW überträgt
- mehrere Visualisierungen ermöglicht
- Echtzeitüberwachung bietet

---

# 4. Theorie

## 4.1 ESP32
Der ESP32 ist ein Mikrocontroller mit WLAN- und Bluetooth-Funktion und wird häufig in IoT-Systemen eingesetzt [4].

---

## 4.2 BMP280 Sensor
Der BMP280 misst Temperatur und Luftdruck und kommuniziert über I2C [8].

---

## 4.3 ESP-NOW
ESP-NOW ist ein drahtloses Peer-to-Peer Kommunikationsprotokoll ohne Router [3].

---

## 4.4 Webserver & Chart.js
Der ESP32 stellt Daten als JSON bereit, welche im Browser mit Chart.js visualisiert werden [2].

---

## 4.5 OLED Display
Das OLED zeigt aktuelle Sensordaten lokal am Gerät an.

---

## 4.6 Telegram Bot
Der Telegram Bot ermöglicht die Abfrage der Daten über `/status` und sendet PIR Benachrichtigungen.

---

## 4.7 Deep Sleep
Der Deep Sleep Modus reduziert den Energieverbrauch des Senders erheblich [7].

---

# 5. Arbeitsschritte

## 5.1 Projektplanung
Zu Beginn wurde das gesamte System geplant. Es wurde entschieden, ein Sender-Empfänger-System mit ESP32 Mikrocontrollern zu entwickeln. Die Sensoren und Aktoren wurden in Mess-, Verarbeitungs- und Ausgabekomponenten eingeteilt.

---

## 5.2 Sensorintegration (BMP280)
Der BMP280 Sensor wurde am Sender angeschlossen und getestet. Dabei wurde überprüft, ob stabile Temperatur- und Luftdruckwerte korrekt ausgelesen werden.

Nach erfolgreichem Test wurde der Sensor in das Hauptsystem integriert.

---

## 5.3 ESP-NOW Kommunikation
Die drahtlose Kommunikation zwischen Sender und Empfänger wurde implementiert.

Dabei traten zunächst Probleme durch unterschiedliche WLAN-Kanäle auf. Diese führten zu fehlender Datenübertragung.

Nach Synchronisation beider Geräte auf Kanal 11 konnte das Problem behoben werden.

---

## 5.4 Webserver Implementierung
Der Empfänger stellt einen Webserver bereit, der die Daten in Echtzeit darstellt.

Die Daten werden als JSON übertragen und im Browser verarbeitet.

---

## 5.5 Datenvisualisierung (Chart.js)
Es wurden zwei Graphen implementiert:

- Temperaturverlauf
- Luftdruckverlauf

Diese werden kontinuierlich aktualisiert und ermöglichen eine Live-Überwachung.

---

## 5.6 Erweiterte Ausgabegeräte
Nach erfolgreicher Grundfunktion wurden zusätzliche Komponenten integriert:

- OLED Display zur lokalen Anzeige
- RGB LED zur Statusanzeige
- Buzzer zur Alarmierung

---

## 5.7 Erweiterte Sensoren
Zusätzlich wurden folgende Sensoren integriert:

- PIR Sensor zur Bewegungserkennung
- LDR Sensor zur Messung der Helligkeit

---

## 5.8 Telegram Integration
Der Telegram Bot ermöglicht die Abfrage der aktuellen Werte über `/status`. Zusätzlich werden Bewegungen des PIR Sensors automatisch gemeldet.

---

# 6. Testphase

Alle Komponenten wurden einzeln getestet:

- BMP280 liefert stabile Werte
- ESP-NOW funktioniert zuverlässig
- Webserver aktualisiert Live-Daten
- Graphen zeigen korrekte Historie
- Deep Sleep reduziert Energieverbrauch
- OLED zeigt aktuelle Werte
- RGB LED reagiert auf Temperatur
- Buzzer löst Alarm aus
- Telegram Bot sendet PIR Meldungen
- PIR Sensor erkennt Bewegung
- LDR Sensor reagiert auf Licht

---

# 7. Komponentenliste

| Komponente | Funktion |
|------------|----------|
| ESP32 (x2) | Steuerung & Kommunikation |
| BMP280 | Temperatur & Luftdruck |
| OLED SSD1306 | Lokale Anzeige |
| RGB LED | Statusanzeige |
| KY-006 Buzzer | Alarm |
| PIR Sensor | Bewegung |
| LDR Sensor | Helligkeit |
| WLAN | Webserver |
| ESP-NOW | Kommunikation |
| Telegram Bot | Fernüberwachung |

---

# 8. Schaltungsplan

![Schaltplan](images/Schaltplan.png)

Der Schaltplan zeigt den Aufbau des gesamten Systems.

Der Sender-ESP32 liest den BMP280 Sensor aus und sendet die Daten über ESP-NOW an den Empfänger.

Der Empfänger verarbeitet die Daten und steuert OLED Display, RGB LED, Buzzer sowie zusätzliche Sensoren.

Die Schaltung zeigt klar die Trennung zwischen Messung (Sender) und Verarbeitung (Empfänger), wodurch das System modular aufgebaut ist.

---

# 9. Code

Der vollständige Code ist im GitHub Repository enthalten.

## Sender
- BMP280 Messung
- ESP-NOW Übertragung
- Deep Sleep Energiesparmodus

## Receiver
- Webserver + Chart.js
- OLED Display
- RGB LED Steuerung
- Buzzer Alarm
- PIR + LDR Sensoren
- Telegram Bot Integration

---

# 10. Zusammenfassung

Das Projekt zeigt ein vollständiges IoT-System mit drahtloser Kommunikation, Echtzeitvisualisierung und Sensorintegration.

Die größte Herausforderung war die stabile ESP-NOW Verbindung, welche durch Kanal-Synchronisation gelöst wurde.

Das System wurde erfolgreich durch zusätzliche Sensoren, Webvisualisierung und Telegram Integration erweitert.

---

# 11. Quellen

[1] Allnet Sensor Kit – Conrad  
https://www.conrad.de/

[2] Chart.js Documentation  
https://www.chartjs.org/

[3] Espressif ESP-NOW Documentation  
https://docs.espressif.com/

[4] ESP32 Technical Reference  
https://docs.espressif.com/

[7] ESP32 Deep Sleep Guide  
https://randomnerdtutorials.com/

[8] BMP280 Sensor Documentation  
https://arduinogetstarted.com/
