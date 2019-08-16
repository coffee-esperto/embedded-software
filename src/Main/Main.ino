// WIFI

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// AWS

#include <AmazonSNSClient.h>
#include <ESP8266AWSImplementations.h>

// LOGS 
const int LOGS_ENABLED = false;

// BUTTONS
const int BUTTON_GREEN = 16;
const int BUTTON_RED = 5;

// LEDS
const int LED_GREEN = 12;
const int LED_RED = 13;
const int LED_BLUE = 4;

// POSSIBLE STATES (CAN CHANGE TO ENUM?)
const int STATE_HAVE = 0;
const int STATE_HAVENT = 1;
const int STATE_LOADING = 2;
const int STATE_LOADED = 3;
const int STATE_ERROR = 4;

// ACTUAL STATE (STARTED AS LOADING)
int state = 2;

// STATE MONITOR
int stateChanged = false;

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
  //wifiManager.startConfigPortal("Coffee");
  wifiManager.autoConnect("", "");

  // CHANGE STATE AFTER SUCCESSFUL CONNECTION
  state = STATE_LOADED;
  
  // UPDATE LED STATE
  updateLED();
}
void loop() {
  // INITIALIZE STATE MONITOR
  stateChanged = false;
  
  // READ BUTTON STATES
  int button_have_state = digitalRead(BUTTON_GREEN);
  int button_havent_state = digitalRead(BUTTON_RED);

  // LOGS
  if (LOGS_ENABLED) {
    Serial.print("Have: ");
    Serial.println(button_have_state);
    Serial.print("Haven't: ");
    Serial.println(button_havent_state);
  }

  // UPDATE STATE ON BUTTON PRESS
  if(button_have_state == HIGH && state != STATE_HAVE)
  {
    state = STATE_HAVE;
    stateChanged = true;
  } else {
    if(button_havent_state == HIGH && state != STATE_HAVENT)
    {
      state = STATE_HAVENT;
      stateChanged = true;
    }
  }

  if (stateChanged) {
    // UPDATE LED STATE
    setLEDBlue();
    
    // SEND MESSAGE TO SNS
    sendMessage();
    
    // UPDATE LED STATE
    updateLED();
  }
}

// MESSAGING FUNCTIONS

void sendMessage() {
  Esp8266HttpClient httpClient;
  Esp8266DateTimeProvider dateTimeProvider;
  ActionError actionError;

  AmazonSNSClient snsClient;
  snsClient.setAWSRegion("sa-east-1");
  snsClient.setAWSEndpoint("amazonaws.com");
  snsClient.setAWSKeyID("");
  snsClient.setAWSSecretKey("");
  snsClient.setHttpClient(&httpClient);
  snsClient.setDateTimeProvider(&dateTimeProvider);

  PublishInput publishInput;
  publishInput.setTargetArn("arn:aws:sns:sa-east-1:371355836815:bc-coffe-notify");

  char location[10] = "Sala 1006";
  char hasCoffee[5];
  char message[256];

  if (state == STATE_HAVE) {
    strcpy(hasCoffee, "true");
  } else {
    strcpy(hasCoffee, "false");
  }

  snprintf(message, sizeof message, "{\"type\":\"BUTTON\",\"data\":{\"hasCoffee\":%s,\"location\":\"%s\"}}", hasCoffee, location);
  
  publishInput.setMessage(message);

  PublishOutput result = snsClient.publish(publishInput, actionError);

  Serial.print("Action error: ");
  Serial.println(actionError);
  Serial.print("Message ID: ");
  Serial.println(result.getMessageId().getCStr());
  Serial.print("Error type: ");
  Serial.println(result.getErrorType().getCStr());
  Serial.print("Error message: ");
  Serial.println(result.getErrorMessage().getCStr());
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
