#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <exception>
#include <stdexcept>
#include <FS.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>

/**
 * Definiert ob man ein WiFi Access Point erstellen soll oder sich zu einem bestehende WiFi verbinden soll
 * true  = ein eignen WiFi Access Point erstellen
 * false = mit einem bestehenden WiFi verbinden
 */
bool WiFiAccessPointMode = true;
String Hostname; // Lokale Domain
String WiFiName;
String WiFiPassword;

/* Standard IP During programming WEB Server Mode */
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

/**
 * Maximale Anzahl an Clients, die sich mit dem WiFi verbinden koennen.
 * Ist nur wichtig, wenn ein WiFi-Access-Point erstellet wird(WiFiAccessPointMode = true).
 */
unsigned short MaxWiFiCon;

ESP8266WebServer webserver(WiFi.localIP(), 80);
// https://github.com/Links2004/arduinoWebSockets
WebSocketsServer WebSocket(81);
#define DYNAMIC_WEBSITE_UPDATE_INTERVAL 5000 // 1s = 1000(ms)

//ESP8266WiFiClass WiFi;

//MDNSResponder MDNS;

#define RGB_LED_NUMPIXELS  16
#define RGB_LED_PIN        D6
Adafruit_NeoPixel RGB_LEDS(RGB_LED_NUMPIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
String  RGBColor;

#include "lib/PirSensor.hpp"
#include "lib/Relay.hpp"
Relay     relay;
PirSensor Pir_Sensor(D8);

#include "lib/Exception.hpp"
#include "lib/WiFiUtils.hpp"
#include "lib/PirSensor.hpp"
#include "lib/TouchSensor.hpp"
#include "lib/RGBRing.hpp"
#include "lib/Spiffs.hpp"

void setup() {
	Serial.begin(9600);
	Serial.println("Serial started");

	initSpiffs();
	Serial.println("SPIFFS started");

	readConfigs();
	Serial.println("Configs read");

	initWifi();
	Serial.println("WiFi started");
	Serial.print("IP-Adresse: ");
	Serial.println(WiFi.localIP().toString());

	initDNS();
	Serial.println("DNS started currently uncommented");

	initWebServer();
	Serial.println("Web-Server started");

	initWebSockets();
	Serial.println("Web-Sockets started");

	RGB_LEDS.begin();
	Serial.println("RGB-LEDS started");

	for(int i = 0; i < RGB_LEDS.numPixels(); i++) {
		RGB_LEDS.setPixelColor(i, Adafruit_NeoPixel::Color(255, 0, 0));
	}
	RGB_LEDS.show();
	delay(500);
	for(int i = 0; i < RGB_LEDS.numPixels(); i++) {
		RGB_LEDS.setPixelColor(i, Adafruit_NeoPixel::Color(0, 255, 0));
	}
	RGB_LEDS.show();
	delay(500);
	for(int i = 0; i < RGB_LEDS.numPixels(); i++) {
		RGB_LEDS.setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 255));
	}
	RGB_LEDS.show();
	delay(500);
	for(int i = 0; i < RGB_LEDS.numPixels(); i++) {
		RGB_LEDS.setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 0));
	}
	RGB_LEDS.show();

	EffektContainer.push_back(Effekt("Blink", rainbow_soft_blink));
	EffektContainer.push_back(Effekt("RainbowCycle", rainbowCycle));
	EffektContainer.push_back(Effekt("ColorWipe", colorWipe));	
	EffektContainer.push_back(Effekt("Nothing", Nothing));
	for(unsigned short i = 0; i < EffektContainer.size(); i++) {
		if(EffektContainer[i].getName() == "Nothing") {
			aktueller_Effekt = &EffektContainer[i];
			break;
		}
	}

	relay.setPin(D5);
	relay.setName("LED");
}

void loop() {

	dynamicUpdateClientWebsite(); 
	run_Effekt();
	WebSocket.loop();
	yield();
	webserver.handleClient();
	MDNS.update();
	yield();
	Pir_Sensor.check();
}
