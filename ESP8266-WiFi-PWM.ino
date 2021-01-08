#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "index_html.h" // This file contains the webpage to control the esp

const char* ssid = "your_wifi_ssid"; // WiFi-name
const char* password = "your_wifi_password"; // WiFi-password

#define PWMPin 4    //pin1 for PWM
#define PWMPin2 5   //pin2 for PWM

#define PWM_SCALE 1023
#define EEPROM_SIZE 2
#define switchpin1 13
#define switchpin2 2


int ch1 = 100;
int ch2 = 100;
volatile int button1;
volatile int button2;
unsigned long update_millis = 0;

ESP8266WebServer server(80);    // set server port here


void setup()
{
  Serial.begin(115200); // initialize serial port
  Serial.println("");    // new line
  
  EEPROM.begin(EEPROM_SIZE);
  ch1 = EEPROM.read(0);
  ch2 = EEPROM.read(1);
  Serial.println("ch1:" + String(ch1) + "ch2:" + String(ch2));
  pinMode(PWMPin, OUTPUT);
  pinMode(PWMPin2, OUTPUT);
  pinMode(switchpin1, INPUT_PULLUP);
  pinMode(switchpin2, INPUT_PULLUP);
  analogWriteFreq(15000);
  analogWrite(PWMPin, PWM_SCALE*ch1/100);
  analogWrite(PWMPin2, PWM_SCALE*ch2/100);
  attachInterrupt(digitalPinToInterrupt(switchpin1), button1pressISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(switchpin2), button2pressISR, FALLING);

  Serial.println("Waiting for connection");

  //WiFi.mode(WIFI_AP);      // access point mode
  //WiFi.softAP("astral", "12345678");  // name of the WiFi network and password
  //delay(500);           // wait 0,5s
  //Serial.print("IP Adresse ");  // current IP adress of the server
  //Serial.println(WiFi.softAPIP());

 
    // connect to local WiFi
    WiFi.mode(WIFI_STA);
    WiFi.hostname("your_hostname"); //set hostname
    WiFi.begin(ssid, password);
    // waiting for connection
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // END connect to local WiFi
 
//  
  server.on("/", event_index);
  server.onNotFound ( handleNotFound );
  //server.on("/favicon.ico", event_index);

  server.begin();  			// start server
  Serial.println("HTTP Server started");
}

void loop() {
  
  server.handleClient();

  if (button1) {
    stepchannel(&ch1, PWMPin);
    button1 = 0;
  }
  if (button2) {
    stepchannel(&ch2, PWMPin2);
    button2= 0;
  }

  if (0 < update_millis && update_millis < millis()){
    update_millis = 0;
    Serial.println("saving values");
    EEPROM.write(0, ch1);
    EEPROM.write(1, ch2);
    EEPROM.commit();
  }
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
    setchannel(valp, pin, val);
  }
  return;
}


void setchannel (int *valp, int pin, int val) {
  *valp = val;
    analogWrite(pin, PWM_SCALE * val / 100);
    update_millis = millis() + 1000;
}

void stepchannel (int *valp, int pin) {
 
  int val = *valp;

  if (val < 12){
    val = 25;
  } else if (val < 25+12) {
    val = 50;
  } else if (val < 50+12) {
    val = 75;
  } else if (val < 75+12) {
    val = 100;
  } else {
    val = 0;
  }
  setchannel(valp, pin, val);
}

ICACHE_RAM_ATTR
void  button1pressISR() {
  button1 = 1;
}

ICACHE_RAM_ATTR
void  button2pressISR() {
  button2 = 1;
}

void event_index()    // executed if "http://<ip address>/" is accessed
{
  // send index.html, if there are no request args
  if (0 == server.args()) {
    Serial.println("Sending Index.html");
    server.send(200, "text/html", indexHTML);
    return;
  }
  
  // else ...
  handleChannel("ch1", &ch1, PWMPin);
  handleChannel("ch2", &ch2, PWMPin2);

  if (server.arg("f")!="") { 
  const char *arg = server.arg("f").c_str();
  int val = atoi(arg);
  // c++11: int val = stoi(server.arg(ch));
  if (500 <= val && val <= 19000) {
    Serial.println("PWM Frequency " +  String(val));
   analogWriteFreq(val);
   analogWrite(PWMPin, PWM_SCALE * ch1 / 100);
   analogWrite(PWMPin2, PWM_SCALE * ch2 / 100);
  }
  }
  
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
