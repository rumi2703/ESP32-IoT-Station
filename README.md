# IoT-System mit ESP32 – ESP-NOW

## Temperatur- und Luftdruckmessung mit drahtloser Datenübertragung

**VerfasserInnen:** Marco Duong, Rumeysa Erkan  
**Datum:** 13.05.2026


# 1. Einführung

Im Rahmen dieses Projekts wurde ein IoT-System auf Basis von zwei ESP32-Mikrocontrollern entwickelt.

Ziel des Systems ist die Erfassung, drahtlose Übertragung und Visualisierung von Sensordaten.

Dabei werden Temperatur- und Luftdruckwerte mithilfe eines BMP280 Sensors gemessen und über ESP-NOW an einen zweiten ESP32 übertragen.

Die empfangenen Daten werden auf einem Webserver dargestellt und zusätzlich durch einen Buzzer überwacht.

Durch den Einsatz des Deep-Sleep-Modus wird der Energieverbrauch des Systems reduziert.



# 2. Projektbeschreibung

Das System besteht aus einem Sender und einem Empfänger.

Der Sender misst Temperatur und Luftdruck und überträgt diese Daten über ESP-NOW.

Anschließend geht der Sender für 10 Sekunden in den Deep-Sleep-Modus.

Der Empfänger verarbeitet die Daten, stellt sie im Webserver dar und steuert den Buzzer.



# 3. Projektziel

Ziel war die Entwicklung eines funktionierenden IoT-Systems mit:

- Sensorbasierter Messung
- Drahtloser Kommunikation
- Webserver zur Visualisierung
- Graphischer Darstellung der Daten
- Alarmfunktion über Buzzer
- Energiesparmodus (Deep Sleep)



# 4. Theorie

## 4.1 ESP32

Der ESP32 ist ein Mikrocontroller mit WLAN- und Bluetooth-Funktion.

Er wird häufig in IoT-Projekten verwendet, da er viele GPIO-Pins und Energiesparmodi besitzt.



## 4.2 BMP280 Sensor

Der BMP280 misst Temperatur und Luftdruck.

Die Kommunikation erfolgt über I2C.



## 4.3 ESP-NOW

ESP-NOW ist ein Protokoll zur direkten Kommunikation zwischen ESP-Geräten ohne Router.

Vorteile:
- Schnell
- Energieeffizient
- Kein WLAN-Router notwendig



## 4.4 Webserver

Der ESP32 stellt einen Webserver bereit, der die aktuellen Messwerte im Browser anzeigt.



## 4.5 Chart.js

Chart.js wird zur Darstellung der Messwerte als Graph verwendet.



## 4.6 Deep Sleep

Der ESP32 wird nach jeder Messung für 10 Sekunden in den Deep-Sleep-Modus versetzt.

Ablauf:
1. Aufwachen
2. Messen
3. Senden
4. Schlafen (10s)



## 4.7 KY-006 Buzzer

Der Buzzer wird aktiviert, sobald die Temperatur über 32°C steigt.

Er gibt dann dauerhaft ein Signal aus.



# 5. Systemaufbau

## Sender
- BMP280 Messung
- ESP-NOW Senden
- Deep Sleep

## Empfänger
- Daten empfangen
- Webserver
- Chart Darstellung
- Buzzer Steuerung



### 6. Arbeitsschritt

Im folgenden Abschnitt werden die einzelnen Umsetzungsschritte des Projekts detailliert beschrieben, sodass das System nachvollziehbar und reproduzierbar ist.



### 6.1 Planung und Systemdesign

Zu Beginn wurde das Gesamtsystem geplant. 
Der Sender übernimmt ausschließlich die Sensordaten-Erfassung, während der Empfänger für Verarbeitung, Speicherung und Darstellung zuständig ist.



### 6.2 Aufbau der Hardware

Die Komponenten wurden zunächst auf Breadboards aufgebaut und miteinander verbunden.

Dabei wurde besonders darauf geachtet, dass:
- der BMP280 korrekt angeschlossen ist (SDA und SCL Pins)
- der KY-006 Buzzer am digitalen GPIO-Pin angeschlossen ist
- stabile Stromversorgung (3.3V) verwendet wird

Nach dem Aufbau wurde jede Verbindung einzeln getestet.


### 6.3 Integration des BMP280 Sensors

Der BMP280 Sensor wurde über den I2C-Bus mit dem ESP32 verbunden.

Anschließend wurde getestet, ob Temperatur- und Luftdruckwerte korrekt ausgelesen werden.

Nach erfolgreichem Test wurden die Messwerte in das Hauptprogramm integriert.



### 6.4 Implementierung von ESP-NOW

Für die drahtlose Kommunikation wurde ESP-NOW verwendet.

Zunächst wurde die MAC-Adresse des Empfänger-ESP32 ausgelesen und im Sender hinterlegt.

Danach wurde:
- ein Peer hinzugefügt
- eine Datenstruktur für Temperatur und Luftdruck erstellt
- ein Testdatensatz gesendet

Die Übertragung wurde anschließend erfolgreich verifiziert.



### 6.5 Webserver Implementierung

Der Empfänger stellt einen Webserver zur Verfügung.

Nach Verbindung mit dem WLAN kann der Benutzer über einen Browser auf die aktuelle IP-Adresse zugreifen.

Die Messwerte werden im JSON-Format bereitgestellt und im Browser verarbeitet.



### 6.6 Datenvisualisierung (Chart.js)

Für die Darstellung der Messwerte wurde Chart.js verwendet.

Die empfangenen Daten werden laufend aktualisiert und in zwei Diagrammen dargestellt:
- Temperaturverlauf
- Luftdruckverlauf

Die Daten werden kontinuierlich ergänzt, wodurch ein Verlauf sichtbar wird.



### 6.7 Deep Sleep Umsetzung

Nach jeder erfolgreichen Messung wird der Sender in den Deep-Sleep-Modus versetzt.

Ablauf:
1. ESP32 startet
2. Messung von Temperatur und Luftdruck
3. Übertragung der Daten via ESP-NOW
4. Aktivierung von Deep Sleep für 10 Sekunden
5. Neustart des Zyklus

Durch diesen Mechanismus wird der Energieverbrauch deutlich reduziert.




# 7. Komponentenliste

| Komponente | Funktion |
|------------|----------|
| ESP32 (x2) | Steuerung |
| BMP280 | Sensor |
| KY-006 | Buzzer |
| WLAN | Webserver |
| ESP-NOW | Kommunikation |



# 8. Schaltungsplan

![Schaltplan](images/SchaltplanGK.png)

### BMP280 Anschluss

| Pin | ESP32 |
|-----|------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |



### Buzzer Anschluss

| Pin | ESP32 |
|-----|------|
| Signal | GPIO 27 |
| GND | GND |



# 9. Code

Der Code befindet sich im Repository.

Funktionen:
- Sensor Messung
- ESP-NOW Kommunikation
- Webserver
- Chart Darstellung
- Buzzer Steuerung
- Deep Sleep



# 10. Testphase

| Test | Ergebnis |
|------|----------|
| BMP280 | OK |
| ESP-NOW | OK |
| Webserver | OK |
| Graph | OK |
| Deep Sleep | OK |
| Buzzer | OK |

### Probleme

Problem:
- ESP-NOW und Webserver funktionierten gleichzeitig nicht stabil

Lösung:
- Fixierung auf denselben WLAN-Channel
- Danach stabile Verbindung



# 11. Zusammenfassung

Es wurde ein IoT-System entwickelt, das Sensordaten misst, drahtlos überträgt und visualisiert.

Der Einsatz von Deep Sleep reduziert den Energieverbrauch deutlich.

Das System funktioniert stabil nach der Kanal-Synchronisation.



# 12. Quellen

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
