#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

// https://www.electroniclinic.com/nodemcu-with-arduino-serial-communication-control-and-monitoring/

int pinValue1;
int pinValue2;
int pinValue3;
int pinValue4;
 
char ssid[] = "FreshMesh";
char pass[] = "freshmesh";
 
SimpleTimer timer;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
 
//for temperature , pressure and altitude
int firstVal, secondVal,thirdVal;
 
void setup()
{
  // Debug console
  Serial.begin(115200);
 
	timer.setInterval(1000L,sensorvalue1); 
	timer.setInterval(1000L,sensorvalue2); 
	timer.setInterval(1000L,sensorvalue3);
 
}
 
void loop()
{
   if (Serial.available() == 0 ) 
   {
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
   // Serial.print(rdata);
    if( rdata == '\n')
    {
   //  Serial.println(myString); 
  // Serial.println("fahad");
// new code
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2); 
 
 
firstVal = l.toInt();
secondVal = m.toInt();
thirdVal = n.toInt();
 
  myString = "";
// end new code
    }
  }
 
}
 
void sensorvalue1()
{
int sdata = firstVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
 
}
void sensorvalue2()
{
int sdata = secondVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, sdata);
 
}
 
void sensorvalue3()
{
int sdata = thirdVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, sdata);
 
}
 
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
 
// in Blynk app writes values to the Virtual Pin 10
BLYNK_WRITE(V10)
{
   pinValue1 = param.asInt(); // assigning incoming value from pin V10 to a variable
 
  Serial.print(pinValue1);
 
}
 
 
// in Blynk app writes values to the Virtual Pin 11
BLYNK_WRITE(V11)
{
   pinValue2 = param.asInt(); // assigning incoming value from pin V10 to a variable
 
  Serial.print(pinValue2);
 
}