#include <Arduino.h>
#include <WiFi.h>
#include <ThingESP.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>



ThingESP32 thing("josefgst", "espdisplay", "espdisplayforkit");

int LED = 2;
String message = "";

// MAX7219 LED Matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   18  // or SCK
#define DATA_PIN  23  // or MOSI
#define CS_PIN    5  // or SS
// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

unsigned long previousMillis = 0;
const long INTERVAL = 6000;

void setup()
{
  Serial.begin(115200);


  pinMode(LED, OUTPUT);

  P.begin();

  thing.SetWiFi("AndroidAP", "csw2tpz;");

  thing.initDevice();
}

String HandleResponse(String query)
{
  Serial.println(query);
  message = query;

  // while (P.displayAnimate())
  //   P.displayText("Hello", PA_CENTER, 75, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  if (query == "led on")
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
  
  else 
    return "";  
}
void loop()
{

  thing.Handle();

  if (P.displayAnimate())
    P.displayText(message.c_str(), PA_LEFT, 75, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}