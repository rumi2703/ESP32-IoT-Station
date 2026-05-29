# IoT-System mit ESP32 – ESP-NOW, Sensorik, Webvisualisierung & Telegram Bot

## Temperatur- und Luftdruckmessung mit drahtloser Datenübertragung und Echtzeit-Visualisierung

**VerfasserInnen:** Marco Duong, Rumeysa Erkan
**Datum:** 13.05.2026

# 1. Einführung

Im Rahmen dieses Projekts wurde ein vollständiges IoT-System auf Basis von zwei ESP32 Mikrocontrollern entwickelt.

Ziel ist die Erfassung, drahtlose Übertragung und Visualisierung von Sensordaten in Echtzeit.

Dabei werden Temperatur- und Luftdruckwerte mithilfe eines BMP280 Sensors gemessen und über **ESP-NOW** an einen zweiten ESP32 übertragen.

Der Empfänger verarbeitet die Daten und stellt sie über mehrere Ausgabekanäle dar:

* Webserver mit Live-Daten
* Chart.js Graphen
* OLED Display
* RGB Status LED
* Buzzer Alarm
* Telegram Bot (/status + PIR Meldungen)

# 2. Systemaufbau

Das Projekt besteht aus zwei ESP32 Modulen:

## Sender (Sensor Node)

* BMP280 Sensor
* misst Temperatur & Luftdruck
* sendet Daten via ESP-NOW
* Deep Sleep für Energieeffizienz

## Empfänger (Control Node)

* empfängt ESP-NOW Daten
* verarbeitet & speichert Messwerte
* stellt Webserver bereit
* steuert OLED, RGB LED und Buzzer
* verarbeitet PIR + LDR Sensoren
* sendet Telegram Nachrichten

# 3. Projektziel

Ziel ist die Entwicklung eines stabilen IoT-Systems, das:

* Sensordaten zuverlässig erfasst
* drahtlos überträgt (ESP-NOW)
* mehrere Visualisierungsmöglichkeiten bietet
* Echtzeit-Überwachung ermöglicht

Ein besonderer Fokus liegt auf:

* stabiler ESP-NOW Kommunikation
* sauberer Webvisualisierung

# 4. Theorie

## 4.1 ESP32

Der ESP32 ist ein Mikrocontroller mit integrierter WLAN- und Bluetooth-Funktion und eignet sich ideal für IoT-Systeme.

## 4.2 BMP280 Sensor

Der BMP280 misst:

* Temperatur (°C)
* Luftdruck (hPa)

Die Kommunikation erfolgt über das I2C-Protokoll.

## 4.3 ESP-NOW

ESP-NOW ist ein direktes Funkprotokoll zwischen ESP-Geräten ohne Router.

Vorteile:

* geringe Latenz
* kein WLAN-Netzwerk nötig
* stabile Peer-to-Peer Kommunikation

## 4.4 Webserver & Chart.js

Der ESP32 stellt einen Webserver bereit.

Die Daten werden:

* als JSON geliefert
* im Browser mit Chart.js visualisiert

## 4.5 OLED Display

Das OLED Display zeigt:

* aktuelle Temperatur
* aktuellen Luftdruck

## 4.6 Telegram Bot

Der Telegram Bot dient zur Fernüberwachung des Systems.

Über den Befehl `/status` können aktuelle Temperatur- und Luftdruckwerte abgefragt werden. Zusätzlich werden Bewegungen des PIR Sensors automatisch als Nachricht gesendet.

## 4.7 Deep Sleep

Der Deep Sleep Modus reduziert den Stromverbrauch des Sender-ESP32 erheblich.

Nach jeder Messung und Datenübertragung wird der Mikrocontroller für einige Sekunden in einen Energiesparmodus versetzt und anschließend automatisch wieder aktiviert.

# 5. Implementierung

## 5.1 Sensorintegration

Zuerst wurde der BMP280 Sensor am Sender-ESP32 integriert und getestet. Die korrekte Ausgabe von Temperatur- und Luftdruckwerten wurde überprüft.

## 5.2 ESP-NOW Kommunikation

Die Kommunikation wurde stabil zwischen Sender und Empfänger implementiert.

Problem:

* unterschiedliche WLAN-Kanäle führten zu Verbindungsfehlern

Lösung:

* fixer Kanal (11) auf beiden Geräten

## 5.3 Webserver & Graphen

Der Empfänger stellt einen Webserver bereit.

Die Messdaten werden im Browser mithilfe von Chart.js dargestellt.

Dabei werden zwei Graphen angezeigt:

* Temperaturverlauf
* Luftdruckverlauf

## 5.4 Erweiterungen (EK Features)

Zusätzlich wurden folgende Erweiterungen implementiert:

* WiFiManager für einfache WLAN-Konfiguration
* OLED Display
* RGB Status LED (temperaturabhängig)
* Telegram Bot (/status + PIR Meldungen)
* PIR Bewegungssensor
* LDR Helligkeitssensor

# 6. Testphase

Alle Komponenten wurden einzeln getestet:

* BMP280 liefert stabile Werte
* ESP-NOW Verbindung stabil
* Webserver aktualisiert Live-Daten
* Graphen zeigen korrekte Historie
* Deep Sleep reduziert Energieverbrauch
* OLED zeigt aktuelle Werte
* RGB LED reagiert auf Temperatur
* Buzzer alarmiert bei Grenzwert
* Telegram Bot sendet PIR Meldungen
* PIR Sensor erkennt Bewegungen
* LDR Sensor reagiert auf Helligkeit
* WiFiManager ermöglicht einfache WLAN-Konfiguration

# 7. Komponentenliste

| Komponente    | Funktion                  |
| ------------- | ------------------------- |
| ESP32 (x2)    | Steuerung & Kommunikation |
| BMP280        | Temperatur & Luftdruck    |
| OLED SSD1306  | Lokale Anzeige            |
| RGB LED       | Statusanzeige             |
| KY-006 Buzzer | Alarm                     |
| PIR Sensor    | Bewegungserkennung        |
| LDR Sensor    | Helligkeit                |
| WLAN          | Webserver                 |
| ESP-NOW       | Datenübertragung          |
| Telegram Bot  | Benachrichtigung          |
| Breadboard    | Aufbau                    |
| Jumper Kabel  | Verbindung                |

# 8. Schaltungsplan

![Schaltplan](images/Schaltplan.png)

# 9. Code

Der vollständige Code ist im Repository enthalten.

## Sender

* BMP280 Messung
* ESP-NOW Übertragung
* Deep Sleep Energiesparmodus

## Receiver

* Webserver + Chart.js
* OLED Display
* RGB LED Steuerung
* Buzzer Alarm
* PIR + LDR Sensoren
* Telegram Bot Integration
* WiFiManager

# 10. Zusammenfassung

Im Projekt wurde ein vollständiges IoT-System entwickelt, das Sensordaten erfasst, drahtlos überträgt und auf mehreren Ebenen visualisiert.

Das System kombiniert:

* ESP-NOW Kommunikation
* Webvisualisierung
* Sensorintegration
* Aktorsteuerung
* Telegram Fernüberwachung

Die größte Herausforderung war die stabile ESP-NOW Kommunikation zwischen zwei ESP32 Geräten. Diese konnte durch die Synchronisation des WLAN-Kanals erfolgreich gelöst werden.

Zusätzlich wurde das System durch mehrere Erweiterungen wie RGB Statusanzeige, OLED Display, Telegram Bot sowie PIR- und LDR-Sensoren erweitert.

# 11. Quellen

[1] https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/
[2] https://www.chartjs.org/
[3] https://core.telegram.org/bots/api
[4] https://docs.espressif.com/
