/*  Connects to the home WiFi network
    Asks some network parameters
    Sends and receives message from the server in every 2 seconds
    Communicates: wifi_server_01.ino
*/
#include <ESP8266WiFi.h>

char ssid[] = "FreshMesh";           // SSID of your home WiFi
char pass[] = "freshmesh";            // password of your home WiFi


IPAddress server(192, 168, 43, 58);    // the fix IP address of the server
// IPAddress ip(192, 168, 43, 27);
// IPAddress gateway(192, 168, 43, 129);
// IPAddress subnet(255, 255, 255, 0);
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  // WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);             // connects to the WiFi router

  if (client.connect(server, 80)) {
    // Connection to the server
    Serial.println("Connected");
  } else {
    Serial.println("Can't connect");
  }
}

void loop () {
  if (!client.connected()) {
    if (client.connect(server, 80)) {
      // Connection to the server
      Serial.println("RE - Connected");
    } else {
      Serial.println("Can't connect");
    }
  }

  if (client.connected()) {
      client.println("GET /takephoto/1 HTTP/1.0");  // sends the message to the server
    Serial.println("ok");
  }
}
