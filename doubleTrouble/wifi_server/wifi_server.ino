/*  Connects to the home WiFi network
    Asks some network parameters
    Starts WiFi server with fix IP and listens
    Receives and sends messages to the client
    Communicates: wifi_client_01.ino
*/
#include <ESP8266WiFi.h>

char ssid[] = "FreshMesh";           // SSID of your home WiFi
char pass[] = "freshmesh";            // password of your home WiFi
WiFiServer server(80);

IPAddress ip(192, 168, 43, 58);            // IP address of the server
IPAddress gateway(192, 168, 43, 129);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) { // print "." until connected to wifi
    delay(50);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());

  // Starts the server
  server.begin();
  Serial.println("Server started");
}

//Moves servomotor to activate the remote control
void takePhoto() {
    delay(1000);
}

WiFiClient client;

void loop () {
  if(!client)
    client = server.available();
  if (client) {
    Serial.println("Client connected");
    if (client.connected()) {
      Serial.println("client is connected");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: ");
      Serial.println(request);

      int val;
      //filter the client request
      if (request.indexOf("/takephoto/1") != -1) { 
        takePhoto();
        Serial.println("taking photo");
      } else {
        Serial.print("invalid request: ");
        Serial.println(request);
      }
      
      client.stop();
    }
  }
}