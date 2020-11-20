#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "index_html.h" // This file contains the webpage to control the esp

const char* ssid = "your_wifi_ssid"; // WiFi-name
const char* password = "your_wifi_password"; // WiFi-password

#define PWMPin 4    //pin1 for PWM
#define PWMPin2 5   //pin2 for PWM
#define PWM_SCALE 1023

int ch1 = 100;
int ch2 = 100;

ESP8266WebServer server(80);    // set server port here


void setup()
{
  pinMode(PWMPin, OUTPUT);
  pinMode(PWMPin2, OUTPUT);
  analogWrite(PWMPin, ch1);
  analogWrite(PWMPin2, ch2);

  Serial.begin(115200); // initialize serial port
  Serial.println("");	  // new line
  Serial.println("Warte auf Verbindung");

  //WiFi.mode(WIFI_AP);      // access point mode
  //WiFi.softAP("astral", "12345678");  // name of the WiFi network and password
  //delay(500);           // wait 0,5s
  //Serial.print("IP Adresse ");  // current IP adress of the server
  //Serial.println(WiFi.softAPIP());

 
    // connect to local WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    // waiting for connection
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
    // END connect to local WiFi
 
//  
  server.on("/", Ereignis_Index);
  server.onNotFound ( handleNotFound );

  server.begin();  			// start server
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

void Ereignis_Index()    // executed if "http://<ip address>/" is accessed
{
  // send index.html, if there are no request args
  if (0 == server.args()) {
    Serial.println("Sende Index.html");
    server.send(200, "text/html", indexHTML);
    return;
  }
  
  // else ...
  handleChannel("ch1", &ch1, PWMPin);
  handleChannel("ch2", &ch2, PWMPin2);
  
  // output state
  char response[200];
  snprintf(
    response,
    sizeof(response),
    "{\"ch1\":%d,\"ch2\":%d}\n",
    ch1,
    ch2);
    
  server.sendHeader("Cache-Control", "no-cache");    // important! for data not to get out of browser cache
  server.sendHeader("Access-Control-Allow-Origin", "*"); // testing with file-index-page
  server.send(200, "text/json", response); // response to webbrowser for sync

}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");     // Send HTTP status 404 (Not Found)
}
