/* Wemos D1 mini, OLED 128x64 I2C, DHT22
 * Sends the temperature value to the client(s)
 * v2: server restart after 30 sec idle time
 */
#include "DHT.h"                    // DHT22
#include <SPI.h>                    // SD
#include <Wire.h>                   // I2C->OLED
#include <Adafruit_GFX.h>           // OLED
#include <Adafruit_SSD1306.h>       // OLED
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/TomThumb.h>
#include <ESP8266WiFi.h>

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 oled(OLED_RESET);
#define DHTPIN 13                   // DHT pin (D7=13)
#define DHTTYPE DHT22

WiFiServer server(80);              // launches the server
IPAddress ip(192, 168, 0, 80);      // fix IP of the server
IPAddress gateway(192,168,0,1);     // WiFi router's IP
IPAddress subnet(255,255,255,0);
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "yourSSIDHere";
char pass[] = "yourPasswordHere";
byte xc = 1, yc = 23, dx = 1, dy = 1;
unsigned long DHTtimer = 0;
float h, t;
unsigned long clientTimer = 0;

void setup() {
//  Serial.begin(115200);                     // only for debug
  dht.begin();
  Wire.begin();                               // default SDA and SCL
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3D);     // I2C address of my b&w OLED
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.clearDisplay();
  oledre(0, 10, "Wemos DHT_server_02", 0);
  oled.display();
  server_start(0);                            // starts the WiFi server
  delay(2000);
}

void loop() {
  if (millis() > DHTtimer + 2000) {
    h = dht.readHumidity();
    t = dht.readTemperature();                // reads the DHT for temperature
    if (isnan(h) || isnan(t)) {
      return;
    } else {
      DHTtimer = millis();
    }
  }
  oled.clearDisplay();
  oledre(xc, yc, String(t, 1) + "C", 1);    // prints the temperature on the OLED
  ipOledre(xc);                             // prints the IP on the OLED
  oled.display();
  if (xc >= 20 || xc <= 0) {dx *= -1;}
  if (yc >= 50 || yc <= 22) {dy *= -1;}
  xc += dx;
  yc += dy;
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      oled.invertDisplay(true);
      String request = client.readStringUntil('\r');    // reads the message from the client
      client.flush();
      client.println(String(t, 1));        // sends the temperature to the client
      oled.invertDisplay(false);
    }
    client.stop();                         // disconnects the client
    clientTimer = millis();
  }
  if (millis() - clientTimer > 30000) {    // stops and restarts the WiFi server after 30 sec
    WiFi.disconnect();                     // idle time
    delay(500);
    server_start(1);
  }
}

void server_start(byte restart) {
  if (restart) {
    oled.clearDisplay();
    oledre(0,10, "Server restart", 0);
    oled.setCursor(0,20);
  } else {
    oledre(0,20, "Server start", 0);
    oled.setCursor(0,30);
  }
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    oled.print(".");
    oled.display();
    delay(500);
  }
  server.begin();
  delay(500);
  clientTimer = millis();
}

void ipOledre(byte xc) {
  oled.setFont(&TomThumb);
  oled.setCursor(xc+35, 63);
  oled.setTextSize(1);
  oled.print(WiFi.localIP());
}

void oledre(byte x, byte y, String text, byte font) {
  Serial.println(text);
  if (font) {
    oled.setFont(&FreeMonoBoldOblique18pt7b);
  } else {
    oled.setFont(&TomThumb);
  }
  oled.setCursor(x, y);
  oled.print(text);
}
