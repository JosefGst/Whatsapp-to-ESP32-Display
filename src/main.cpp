#include <Arduino.h>
#include <WiFi.h>
#include <ThingESP.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

ThingESP32 thing("josefgst", "espdisplay", "espdisplayforkit");

int LED = 2;
String message = "";
bool deisplayOn = true;

// MAX7219 LED Matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN 18  // or SCK
#define DATA_PIN 23 // or MOSI
#define CS_PIN 5    // or SS
// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  P.begin();

  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("ESP32AP", "password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  // comment this line because wifi connection is done by WiFiManager
  // thing.SetWiFi("AndroidAP", "csw2tpz;");
  thing.initDevice();
}

String HandleResponse(String query)
{
  Serial.println(query);
  message = query;

  // while (P.displayAnimate())
  //   P.displayText("Hello", PA_CENTER, 75, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  if (query == "help")
  {
    return "Available commands: hello, time, led blink, led on, led off, led status, display on, display off, display status";
  }

  else if (query == "hello")
  {
    return "Hello from ESP32";
  }

  else if (query == "time")
  {
    return String(millis());
  }

  else if (query == "led blink")
  {
    digitalWrite(LED, 1);
    delay(1000);
    digitalWrite(LED, 0);
    return "led blinked";
  }
  else if (query == "led on")
  {
    digitalWrite(LED, 1);
    return "led is on";
  }

  else if (query == "led off")
  {
    digitalWrite(LED, 0);
    return "led is off";
  }

  else if (query == "led status")
    return digitalRead(LED) ? "LED is ON" : "LED is OFF";

  else if (query == "display off")
  {
    deisplayOn = false;
    return "Display is off";
  }
  else if (query == "display on")
  { 
    deisplayOn = true;
    return "Display is on";
  }
  else if (query == "display status")
    return P.displayAnimate() ? "Display is ON" : "Display is OFF";


  else
    deisplayOn = true;
    return "";
}
void loop()
{

  thing.Handle();

  if( !deisplayOn)
  {
    P.displayClear();
  }
  else 
  {
    if (P.displayAnimate())
    P.displayText(message.c_str(), PA_LEFT, 75, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }