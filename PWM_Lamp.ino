#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "index_html.h"  // In dieser Datei befindet sich die Steuerungs Webseite

const char* ssid = "your_wifi";      // WLAN-Name,
const char* password = "add_your_wifi_password_here"; // WLAN-Passwort
#define RelaisPin 4  // Relais
#define RelaisPin2 5
#define PWM_SCALE 1023

int ch1 = 100;
int ch2 = 100;

ESP8266WebServer server(80);// Serverport  hier einstellen


void setup()
{
  pinMode(RelaisPin, OUTPUT);
  pinMode(RelaisPin2, OUTPUT);
  analogWrite(RelaisPin, ch1);
  analogWrite(RelaisPin2, ch2);

  Serial.begin(115200);	// Serielle Schnittstelle initialisieren
  Serial.println("");	  // Neue Zeile
  Serial.println("Warte auf Verbindung");

  //WiFi.mode(WIFI_AP);      // access point modus
  //WiFi.softAP("astral", "12345678");  // Name des Wi-Fi Netzes und Passwort
  //delay(500);           //Abwarten 0,5s
  //Serial.print("IP Adresse ");  //Ausgabe aktueller IP des Servers
  //Serial.println(WiFi.softAPIP());

 
    // Mit Haus WLAN Verbinden
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    // Warte auf verbindung
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Verbunden mit ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // ENDE Mit Haus WLAN Verbinden
 
//  Bechandlung der Ereignissen anschliessen
  server.on("/", Ereignis_Index);
  server.onNotFound ( handleNotFound );

  server.begin();  			// Starte den Server
  Serial.println("HTTP Server gestartet");
}

void loop()
{
  server.handleClient();
}

void handleChannel(const char *ch, int *valp, int pin)
{
  if (server.arg(ch) == "")
    return;
  const char *arg = server.arg(ch).c_str();
  int val = atoi(arg);
  // c++11: int val = stoi(server.arg(ch));
  if (0 <= val && val <= 100) {
    Serial.println("Channel " + String(ch) + ": val=" + String(val) + ", pin=" + String(pin));
    *valp = val;
    analogWrite(pin, PWM_SCALE * val / 100);
  }
  return;
}

void Ereignis_Index()    // Wird ausgeführt wenn "http://<ip address>/" aufgerufen wurde
{
  // send index.html, if there are no request args
  if (0 == server.args()) {
    Serial.println("Sende Index.html");
    server.send(200, "text/html", indexHTML);
    return;
  }
  
  // else ...
  handleChannel("ch1", &ch1, RelaisPin);
  handleChannel("ch2", &ch2, RelaisPin2);
  
  // output state
  char response[200];
  snprintf(
    response,
    sizeof(response),
    "{\"ch1\":%d,\"ch2\":%d}\n",
    ch1,
    ch2);
    
  server.sendHeader("Cache-Control", "no-cache");    // wichtig! damit Daten nicht aus dem Browser cach kommen
  server.sendHeader("Access-Control-Allow-Origin", "*"); // testing with file-index-page
  server.send(200, "text/json", response); // Antwort an Internet Browser senden nur eine Zahl ("0"/"1")

}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");     // Send HTTP status 404 (Not Found)
}
