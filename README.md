# IoT-System mit ESP32 und ESP-NOW
## Temperatur- und Luftdruckmessung mit Webvisualisierung und Sleep Mode

VerfasserInnen: **Marco Duong, Rumeysa Erkan**

Datum: **13.05.2026**

## 1.  Einführung

Im Rahmen dieses Projekts wurde ein IoT-System auf Basis von zwei ESP32-Mikrocontrollern entwickelt. Ziel ist die Erfassung, Übertragung und Visualisierung von Umweltdaten wie Temperatur und Luftdruck.

Das Projekt zeigt den praktischen Einsatz von Sensorik, drahtloser Kommunikation (ESP-NOW) sowie Webtechnologien zur Datenanzeige. Zusätzlich wird ein Energiesparmodus eingesetzt, um den Stromverbrauch des Systems zu reduzieren.


## 2.  Projektbeschreibung

Es wurde ein verteiltes IoT-System umgesetzt, das aus zwei ESP32 besteht. Ein Mikrocontroller erfasst Sensordaten mit einem BMP280 Sensor und sendet diese drahtlos an einen zweiten ESP32.

Der zweite ESP32 verarbeitet die Daten, speichert sie und stellt sie über ein Webinterface sowie einen einfachen Graphen dar. Zusätzlich wird ein KY-006 Buzzer zur akustischen Warnung bei Grenzwertüberschreitungen verwendet.


## 3.  Theorie

Für die Umsetzung des Projekts sind mehrere technische Grundlagen notwendig.

Der BMP280 ist ein digitaler Sensor zur Messung von Temperatur und Luftdruck. Die Kommunikation erfolgt über I2C. Der ESP32 dient als Mikrocontroller mit integrierter WLAN-Funktion.

Die Datenübertragung erfolgt über ESP-NOW, ein von Espressif entwickeltes Protokoll für direkte Kommunikation zwischen ESP-Geräten ohne Router.

Zur Darstellung der Daten wird ein Webserver auf dem ESP32 betrieben. Die Visualisierung historischer Daten erfolgt über einen einfachen Graphen, der Messwerte zeitlich darstellt.

Der Sleep Mode wird verwendet, um den ESP32 zwischen Messzyklen in einen stromsparenden Zustand zu versetzen.

## 4.   Arbeitsschritt

Zuerst wurde der BMP280 Sensor mit dem ESP32 verbunden und konfiguriert. Danach wurde die regelmäßige Messung von Temperatur und Luftdruck implementiert.

Anschließend wurde die ESP-NOW Kommunikation eingerichtet, um die Sensordaten an den zweiten ESP32 zu übertragen.

Am Empfangsgerät werden die Daten gespeichert und über einen Webserver bereitgestellt. Zusätzlich wurde ein einfacher Graph zur Visualisierung historischer Daten implementiert.

Der KY-006 Buzzer wurde integriert, um bei Überschreitung definierter Temperaturgrenzen ein akustisches Signal auszugeben.

Zum Abschluss wurde der Sleep Mode implementiert, um den Energieverbrauch zwischen den Messzyklen zu reduzieren.

### Code

Sehr wichtig ist es den verwendeten Code zu dokumentieren.

```c++
unsigned long myTime;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  myTime = millis();
  Serial.println(myTime);// Gibt die Zeit seit dem Programmstart aus
  delay(1000);         // Eine Sekunde warten, um keine riesigen Datenmengen zu senden
}
```


### Bilder und Schaltungen

<img src="Schaltplan-SYTProjekt.png" alt="Schaltplan" style="zoom:55%;" />


### Komponentenliste

| Komponente | Modell | Funktion im System |
|------------|--------|-------------------|
| Mikrocontroller | ESP32 (x2) | Steuerung, Datenverarbeitung und ESP-NOW Kommunikation |
| Sensor | BMP280 | Messung von Temperatur und Luftdruck |
| Aktor | KY-006 Buzzer | Akustische Warnsignale bei Grenzwertüberschreitung |
| Display | OLED SSD1306 (I2C) | Anzeige von Messwerten und Systemstatus |
| Verbindung | Jumper Kabel | Elektrische Verbindung der Bauteile |
| Stecksystem | Breadboard | Aufbau der Schaltung ohne Löten |
| Stromversorgung | USB-Kabel (5V) | Versorgung der ESP32 Module |
| Software | Arduino IDE | Programmierung der Mikrocontroller |
| Kommunikation | ESP-NOW | Drahtlose Datenübertragung zwischen den ESP32 |
| Netzwerk | WLAN | Bereitstellung des Webinterfaces |

### Ausformulierte Aussage der Tabelle

Die Komponentenliste zeigt alle im Projekt verwendeten Bauteile sowie deren jeweilige Funktion im Gesamtsystem.

Es wurden zwei ESP32 Mikrocontroller eingesetzt, die gemeinsam die Datenerfassung, Verarbeitung und Darstellung übernehmen. Der BMP280 Sensor dient zur Messung von Temperatur und Luftdruck, während der KY-006 Buzzer zur akustischen Warnung bei Überschreitung definierter Grenzwerte verwendet wird.

Das OLED-Display stellt aktuelle Messwerte und Systemzustände direkt am Gerät dar. Die Verbindung der einzelnen Komponenten erfolgt über Jumper-Kabel auf einem Breadboard, wodurch ein flexibler und übersichtlicher Aufbau ermöglicht wird.

Die Stromversorgung der ESP32 Module erfolgt über USB. Die Kommunikation zwischen den beiden Mikrocontrollern wird über ESP-NOW realisiert, während zusätzlich ein Webserver über WLAN zur Visualisierung der Daten bereitgestellt wird.

### Text

Die technische Dokumentation beschreibt alle Schritte zur Entwicklung und Umsetzung eines technischen Systems. Sie dient dazu, Aufbau, Funktion und Umsetzung nachvollziehbar darzustellen und eine spätere Wartung oder Erweiterung zu ermöglichen.

## 5.  Zusammenfassung

Im Projekt wurde ein funktionierendes IoT-System mit zwei ESP32 umgesetzt. Die Sensordaten werden zuverlässig erfasst, übertragen und im Webinterface dargestellt.

Als Herausforderungen traten vor allem die Synchronisation der ESP-NOW Kommunikation sowie die Optimierung des Sleep Modes auf. Diese konnten durch Anpassungen in der Programmstruktur gelöst werden.

Insgesamt erfüllt das System die Anforderungen zur Datenerfassung, Übertragung und Visualisierung.

## 6.  Quellen

[1] Random Nerd Tutorials, „Getting Started with ESP-NOW (ESP32 with Arduino IDE)“. [Online]. Verfügbar unter: https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/ [Zugriff: 13. Mai 2026].

[2] W3Schools, „Chart.js Tutorial“. [Online]. Verfügbar unter: https://www.w3schools.com/js/js_graphics_chartjs.asp [Zugriff: 13. Mai 2026].
