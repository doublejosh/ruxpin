#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

https://www.electroniclinic.com/nodemcu-with-arduino-serial-communication-control-and-monitoring/

SoftwareSerial nodemcu(2,3);

// Connect VCC of the BMP180 / BMP085 sensor to 3.3V
// Connect GND to Ground
Adafruit_BMP085 bmp;

long int data; 
int relay1 = 13; 
int relay2 = 12;

int sdata1 = 0; // temperature data
long int sdata2 = 0; // pressure data
int sdata3 = 0; // altitude data

String cdata; // complete data

void setup()
{
  Serial.begin(115200); 
  nodemcu.begin(115200);
  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {}
  }
}

void loop()
{
  
if(nodemcu.available() == 0 )
{
      Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    sdata1 = bmp.readTemperature();
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    sdata2 = bmp.readPressure();
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    sdata3 = bmp.readAltitude();
    Serial.println(" meters");  
  

   cdata = cdata + sdata1+","+sdata2+","+sdata3; 
   Serial.println(cdata); 
   nodemcu.println(cdata);
   delay(1000); // 100 milli seconds
   cdata = ""; 
}

if ( nodemcu.available() > 0 ) 
{
  data = nodemcu.parseInt();
  delay(100); 
  Serial.println(data); 

  if ( data == 10 )
  {
    digitalWrite(relay1, LOW); 
  }

    if ( data == 11 )
  {
    digitalWrite(relay1, HIGH); 
  }

  // relay2 

    if ( data == 12 )
  {
    digitalWrite(relay2, LOW); 
  }

    if ( data == 13 )
  {
    digitalWrite(relay2, HIGH); 
  }
}

}