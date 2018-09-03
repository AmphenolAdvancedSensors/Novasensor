/*
  Note: This sketch requires is set to use an Adafuit SSD1306 0.96" screen or
  compatible and requires the libraries to drive that which are not included here.
*/

#include "Wire.h"

byte I2CAddress = 0x27;

float temperature = 0.00;
float pressure = 0.00;


void setup() {
  Serial.begin(19200);  // start serial for output
  Wire.begin();         //sart I2C 2 wire bus
  Serial.println("Amphenol Advanced Sensors");
  Serial.println("Arduino NPA-201 Monitor");
}


void NPA201ReadData()
{
  byte data[5];

  // Initiate Comms to device, initiate measure and read 5 bytes of data
  Wire.beginTransmission(I2CAddress);
  Wire.write(0xAC);  Wire.write(0);
  Wire.endTransmission();
  delay(21);
  Wire.requestFrom(I2CAddress, 5);
  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();
  data[4] = Wire.read();

  //Pressure Value = BridgeData/65535 * (1260-260) + 260 (hPa)
  //Temperature_Value = TempData/65535 * (85+40) - 40   (°C)
  //additional calculations to infer value commensurate with accuracy of sensor

  pressure = (((unsigned long)data[1] << 8) | (unsigned long) data[2]) * 1000 / 65535  + 260;
  pressure = round(pressure);

  temperature = (float) ( data[3] << 8 ) + data[4] ;
  temperature = temperature / 65535 * 125 - 40;
  temperature = round(temperature * 10);
  temperature = temperature / 10;
}

void loop() {
  NPA201ReadData();

  Serial.print(int (pressure)); Serial.print(" hPa,  ");
  Serial.print(temperature); Serial.println(" °C,");

  delay(3000);
}

