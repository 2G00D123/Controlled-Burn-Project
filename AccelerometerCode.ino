#include <LIS3DHTR.h> //Accelerometer Library
#include <SD.h> //SD card library
#include <SPI.h> //Serial peripheral interface for SD card
#include <TinyGPS++.h> //Basic GPS Library
#include <SoftwareSerial.h> //Allows for the transmitting and recieving of data for GPS
#include <Wire.h> //I2C Communication library
#include <SoftwareWire.h> //Similar to Wire

#ifdef SOFTWAREWIRE
SoftwareWire myWire(3, 2);
LIS3DHTR<SoftwareWire> LIS;
#define WIRE myWire
#else
LIS3DHTR<TwoWire> LIS;
#define WIRE Wire
#endif

// Acceletrometer Variables
float z = 0; // Rotation Variable
float zThresh; // Rotation Variable
int buttonPin = 3; // Button Location
int calibrate = 0; // 1: Calibrated 0: Not
int ledPin = 7; // LED Location

void setup()
{
  Serial.begin(9600);
  while (!Serial) {};
  //Accelerometer
  LIS.begin(WIRE, 0x19);
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  pinMode(ledPin, OUTPUT);
}
void loop()
{
  z = LIS.getAccelerationZ();
  Serial.print("Angle is: ");
  Serial.println(z);
  if (digitalRead(3) == 1 && calibrate == 0)
  {
    calibrateAccel();
  }
  if (z < zThresh && calibrate == 1)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Activated");
    delay(500);
  }
  if (z > zThresh && calibrate == 1)
  {
    digitalWrite(ledPin, LOW);
    Serial.println("Deactivated");
    delay(500);
  }
  if (calibrate == 0)
  {
    delay(250); //random delay lol
  }
}

void calibrateAccel() //Calibrates Accelerometer
{
  Serial.println("Calibrating in 3 Seconds...");
  digitalWrite(ledPin, HIGH);
  delay(3000);
  calibrate = 1;
  zThresh = z;
  Serial.print("The threshold is: ");
  Serial.println(zThresh);
  digitalWrite(ledPin, LOW);
}
