/*  NodeMCU, OLED 128x64 I2C
 *  Receives temperature data from the server and shows it on an OLED screen
 *  Server: DTH_server_02.ino
 */ 
#include <SPI.h>
#include <Wire.h>                   // I2C->OLED
#include <Adafruit_GFX.h>           // OLED
#include <Adafruit_SSD1306.h>       // OLED
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/TomThumb.h>
#include <ESP8266WiFi.h>

IPAddress server(192,168,0,80);     // fix IP of the server
WiFiClient client;

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 oled(OLED_RESET);

//int status = WL_IDLE_STATUS;
char ssid[] = "yourSSIDHere";
char pass[] = "yourPasswordHere";
byte xc = 1, yc = 23, dx = 1, dy = 1;
unsigned long askTimer = 0;
unsigned long oledTimer = 0;
String answer;
uint8_t pinD1 = 5;    // I2C Bus SCL (clock)
uint8_t pinD2 = 4;    // I2C Bus SDA (data)
//uint8_t pinD3 = 0;
//uint8_t pinD4 = 2;


void setup() {
  Wire.begin(pinD2, pinD1);                 // SDA, SCL
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // I2C address of my blue OLED
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.clearDisplay();
  oledre(0, 10, "Wemos DHTkliens_011", 0);
  oled.display();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
    delay(500);
  }
}

void loop () {
  if (millis() - askTimer > 2340) {         // time between two connection to the server
    client.connect(server, 80);             // connects to the server
    client.println("Haliho szerver!\r");    // trigger message to the server, its value is scrapped
    answer = client.readStringUntil('\r');  // received the server's answer
    client.flush();
    askTimer = millis();
  }
  if (millis() - oledTimer > 50) {          // timer to refresh the OLED
    oled.clearDisplay();
    oledre(xc, yc, answer + "C", 1);
    oled.display();
    if (xc >= 20 || xc <= 0) {dx *= -1;}
    if (yc >= 63 || yc <= 22) {dy *= -1;}
    xc += dx;
    yc += dy;
    oledTimer = millis();
  }
}

void oledre(byte x, byte y, String szoveg, byte font) {   // prints strings on the OLED
  if (font) {
    oled.setFont(&FreeMonoBoldOblique18pt7b);
  } else {
    oled.setFont(&TomThumb);
  }
  oled.setCursor(x, y);
  oled.print(szoveg);
}
