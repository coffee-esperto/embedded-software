#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// BUTTONS
const int BUTTON_GREEN = 16;
const int BUTTON_RED = 5;

// LEDS
const int LED_GREEN = 12;
const int LED_RED = 13;
const int LED_BLUE = 4;

// POSSIBLE STATES
const int STATE_HAVE = 0;
const int STATE_HAVENT = 1;
const int STATE_LOADING = 2;
const int STATE_LOADED = 3;
const int STATE_ERROR = 4;

// ACTUAL STATE (STARTED AS LOADING)
int state = 2;

void setup() {
  // INITIALIZE SERIAL
  Serial.begin(9600);

  // INITIALIZE I/O
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_GREEN,INPUT);
  pinMode(BUTTON_RED,INPUT);
  pinMode(LED_GREEN,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);

  // UPDATE LED STATE
  updateLED();

  // CONNECT TO WIFI
  WiFiManager wifiManager;
  wifiManager.startConfigPortal("Coffee");
  //wifiManager.autoConnect("", "");

  // CHANGE STATE AFTER SUCCESSFUL CONNECTION
  state = STATE_LOADED;
}
void loop() {
  // READ BUTTON STATES
  int button_have_state = digitalRead(BUTTON_GREEN);
  int button_havent_state = digitalRead(BUTTON_RED);

  // LOGS
  if (false) {
    Serial.print("Have: ");
    Serial.println(button_have_state);
    Serial.print("Haven't: ");
    Serial.println(button_havent_state);
  }

  // UPDATE STATE ON BUTTON PRESS
  if(button_have_state == HIGH)
  {
    state = STATE_HAVE;
    //sendMessage()
  } else {
    if(button_havent_state == HIGH)
    {
      state = STATE_HAVENT;
      //sendMessage()
    }
  }

  // UPDATE LED STATE
  updateLED();
}

// LED FUNCTIONS

void updateLED() {
  switch (state) {
    case STATE_HAVE:
      setLEDGreen();
      break;
    case STATE_HAVENT:
      setLEDRed();
      break;
    case STATE_LOADING:
      setLEDBlue();
      break;
    case STATE_LOADED:
      setLEDOff();
      break;
    case STATE_ERROR:
      setLEDOff();
      break;
  }
}

void setLEDOff() {
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,LOW);
}

void setLEDRed() {
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,HIGH);
}

void setLEDBlue() {
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,LOW);  
}

void setLEDGreen() {
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_RED,LOW);
}
