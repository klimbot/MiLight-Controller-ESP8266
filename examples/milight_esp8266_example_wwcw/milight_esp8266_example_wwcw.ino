#include <ESP8266WiFi.h>
#include <Milight.h>
#include <WiFiUDP.h>

/*************************************************************************
This example assumes that you have:
1) a Milight / LimitlessLED Wifi gateway
2) the gateway is paired with an RGBW Milight LED strip driver
3) the gateway is connected to your wireless network
4) the wireless network is bridged to your wired network
5) and that the wired network is connected to a standard Ethernet Shield
    attached to your Arduino
Things you'll need to configure:
1) The IP address of your milight gateway
2) The port that your milight gateway speaks on
3) The SSID of your Home WiFi
4) The Password of your Home Wifi
*************************************************************************/

// wifi connection variables
const char* ssid     = "Klimovski-Local";
const char* password = "AndrewLana";
const unsigned int milight_port = 8899;
IPAddress milight_ip(192, 168, 10, 80);

boolean wifiConnected = false;

WiFiUDP UDP;

Milight ml;


boolean connectWifi();

unsigned long nextAdvance = 0;
uint8_t lighting_state = 7;
int incomingByte = 0;

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // Initialize Milight / LimitlessLED controller with the chosen IP and port, as well as
  // access to the UDP object to actually send packets with
  ml.begin(&UDP, &milight_ip, milight_port);

  // Set nextAdvance to the current time
  nextAdvance = millis();
}

void loop() {
  // check if the WiFi connection were successful
  if (wifiConnected) {
    // Milight.workQueue() must be called as often as possible
    // Its job is to send packets on prescribed time intervals without
    // stalling the main loop.  Thus, your main loop() function should be free-running
    // without any long stalls.
    ml.workQueue();

    // If it's not time to advance the state machine, just keep running around the loop
    //if (millis() < nextAdvance)
    //  return;

    incomingByte = Serial.read();
    //Serial.println(incomingByte);
    lighting_state = incomingByte - 48;
    incomingByte = incomingByte - 48;

    // This is the state machine that shows off what the library can do
    // Its job is to check if a certain amount of time has passed, and then change the light
    // configuration
    //switch (lighting_state)
    switch (incomingByte)
    {
      case 1:
        Serial.print("Lights Off WWCW\n");
        ml.offWWCW(MILIGHT_CH_ALL);
        nextAdvance = millis() + 3000;
        lighting_state = 8;
        break;

      case 2:
        Serial.print("Lights On WWCW\n");
        ml.onWWCW(MILIGHT_CH_ALL);
        nextAdvance = millis() + 3000;
        lighting_state = 9;
        break;

      case 3:
        Serial.print("Lights dimming WWCW\n");
        ml.brightnessDownWWCW(MILIGHT_CH_ALL);
        nextAdvance = millis() + 3000;
        lighting_state = 10;
        break;

      case 4:
        Serial.print("Lights brightning WWCW\n");
        ml.brightnessUpWWCW(MILIGHT_CH_ALL);
        nextAdvance = millis() + 3000;
        lighting_state = 7;
        break;

      case 5:
        Serial.print("Warmer\n");
        ml.warmerWWCW(MILIGHT_CH_1);
        nextAdvance = millis() + 3000;
        lighting_state = 7;
        break;

      case 6:
        Serial.print("Cooler\n");
        ml.coolerWWCW(MILIGHT_CH_1);
        nextAdvance = millis() + 3000;
        lighting_state = 7;
        break;

      case 7:
        Serial.print("Brightnes Full\n");
        ml.brightnessFullWWCW(MILIGHT_CH_1);
        nextAdvance = millis() + 3000;
        lighting_state = 7;
        break;

      case 8:
        Serial.print("Night Mode\n");
        ml.nightModeWWCW(MILIGHT_CH_1);
        nextAdvance = millis() + 3000;
        lighting_state = 7;
        break;
    }
  }

  else {
    wifiConnected = connectWifi();
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
