#include <Arduino.h>
#include <WiFi.h>
#include <ThingESP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

ThingESP32 thing("josefgst", "espdisplay", "espdisplayforkit");

int LED = 2;

LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

unsigned long previousMillis = 0;
const long INTERVAL = 6000;

void setup()
{
  Serial.begin(115200);
  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode(LED, OUTPUT);

  thing.SetWiFi("AndroidAP", "csw2tpz;");

  thing.initDevice();
}

String HandleResponse(String query)
{
  Serial.println(query);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(query.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(query.substring(16));

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
}
void loop()
{

  // if (millis() - previousMillis >= INTERVAL)
  // {
  //     previousMillis = millis();
  //     String msg = digitalRead(LED) ? "LED is ON" : "LED is OFF";
  //     thing.sendMsg("PHONE_NUMBER", msg);
  // }

  thing.Handle();
}