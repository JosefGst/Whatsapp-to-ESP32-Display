#include <Arduino.h>
#include <WiFi.h>
#include <ThingESP.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <time.h>

ThingESP32 thing("josefgst", "espdisplay", "espdisplayforkit");

int LED = 2;
String message = "";
#define BUF_SIZE 75
char current_msg[BUF_SIZE] = {"Hello from ESP32"};
char new_msg[BUF_SIZE] = {"Hello from ESP32"};
bool deisplayOn = true;

// MAX7219 LED Matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN 18  // or SCK
#define DATA_PIN 23 // or MOSI
#define CS_PIN 5    // or SS
// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// TIME
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600; // GMT+8
const int daylightOffset_sec = 0;

// MILLIS
unsigned long startMillis; // some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 10 * 1000; // the value is a number of milliseconds

uint8_t state = 0;

void printLocalTime();

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  startMillis = millis();

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

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

String HandleResponse(String query)
{
  Serial.println(query);
  strncpy(new_msg, query.c_str(), sizeof(new_msg) - 1);
  new_msg[sizeof(new_msg) - 1] = '\0'; // Ensure null termination

  deisplayOn = true;
  P.displayClear();
  P.displayReset();

  // while (P.displayAnimate())
  //   P.displayText("Hello", PA_CENTER, 75, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  if (query == "help")
  {
    return "Available commands: \nhello, time, led blink, led on, led off, led status, on, off, display status";
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

  else if (query == "off")
  {
    deisplayOn = false;
    return "off";
  }
  else if (query == "on")
  {
    deisplayOn = true;
    return "on";
  }
  else if (query == "status")
    return P.displayAnimate() ? "ON" : "OFF";

  else
    return "";
}

void loop()
{

  thing.Handle();

  currentMillis = millis();

  if (currentMillis - startMillis >= period)
  {
    startMillis = currentMillis; // IMPORTANT to save the start time of the current LED state.
    state ^= 1;                  // Switch between 0 and 1;
  }

  if (deisplayOn)
  {
    if (P.displayAnimate())
    {

      switch (state)
      {
      // Show Whatsapp message
      case 0:
        strncpy(current_msg, new_msg, sizeof(current_msg) - 1);
        current_msg[sizeof(current_msg) - 1] = '\0'; // Ensure null termination
        break;

      // Show time
      case 1:

        printLocalTime();
        // strftime(current_msg, 3, "%H", &timeinfo);

      default:
        break;
      }

      if (P.getTextColumns(current_msg) > (MAX_DEVICES * 8))
      {
        P.displayText(current_msg, PA_LEFT, 75, 5000, PA_SCROLL_LEFT, PA_SCROLL_DOWN);
      }
      else
      {
        P.displayText(current_msg, PA_LEFT, 75, 10000, PA_SCROLL_DOWN, PA_SCROLL_DOWN);
      }
    }
  }
  else
  {
    P.displayClear();
  }
}
// void loop()

// HELPER FUNCTIONS

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  // P.print(&timeinfo, "%H:%M");
  strftime(current_msg, 6, "%H:%M", &timeinfo);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  // Serial.print("Day of week: ");
  // Serial.println(&timeinfo, "%A");
  // Serial.print("Month: ");
  // Serial.println(&timeinfo, "%B");
  // Serial.print("Day of Month: ");
  // Serial.println(&timeinfo, "%d");
  // Serial.print("Year: ");
  // Serial.println(&timeinfo, "%Y");
  // Serial.print("Hour: ");
  // Serial.println(&timeinfo, "%H");
  // Serial.print("Hour (12 hour format): ");
  // Serial.println(&timeinfo, "%I");
  // Serial.print("Minute: ");
  // Serial.println(&timeinfo, "%M");
  // Serial.print("Second: ");
  // Serial.println(&timeinfo, "%S");

  // Serial.println("Time variables");
  // char timeHour[3];
  // strftime(timeHour, 3, "%H", &timeinfo);
  // Serial.println(timeHour);
  // char timeWeekDay[10];
  // strftime(timeWeekDay, 10, "%A", &timeinfo);
  // Serial.println(timeWeekDay);
  // Serial.println();
}