/*
Programme changes address on NPA-201, their power needs to be derived from Output Pin 13 to allow
the criteria for getting into command mode to be controlled.

IMPORTANT NOTE, if UNO is not 3.3V then voltage splitters need be used else you will fry the NPA.
 */

#include "Wire.h"
int readline(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
        break;
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len - 1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

int OldI2CAddress = 0;
int NewI2CAddress = 0;
int i = 0;


byte data[10];

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        //switches sensor on

  Serial.begin(115200);         // start serial for output
  while (!Serial);              // Leonardo: wait for serial monitor

  Wire.begin();
  Serial.println("Amphenol Advanced Sensors");
  Serial.println("Arduino I2C Change Address for NPA201");

  Serial.print("Initial ");
  ScanI2CBus();
}

void loop() {
  static char buffer[80];

  if (OldI2CAddress <= 0 & i <= 0)
  {
    Serial.println("Enter Address to Change in Decimal");
    i = 1;
  }

  if (OldI2CAddress <= 0) {
    if (readline(Serial.read(), buffer, 80) > 0) {
      OldI2CAddress = atoi(buffer);
      Serial.print("You entered old: ");
      Serial.print(OldI2CAddress);
      Serial.println(" ");

    }
  }
  if (OldI2CAddress > 0 & NewI2CAddress <= 0 & i == 1) {
    Serial.println("Enter New Address in Decimal");
    i = 2;
  }

  if (OldI2CAddress > 0 & NewI2CAddress <= 0) {

    if (readline(Serial.read(), buffer, 80) > 0) {
      NewI2CAddress = atoi(buffer);
      Serial.print("You entered new: ");
      Serial.print(NewI2CAddress);
      Serial.println(" ");
    }
  }
  if (OldI2CAddress > 0 & NewI2CAddress > 0 & i == 2) {
    Serial.println("Are these values correct?");
    i = 3;
  }
  if (OldI2CAddress > 0 & NewI2CAddress > 0) {
    if (readline(Serial.read(), buffer, 80) > 0) {
      String answer = buffer;
      if (answer == "y" | answer == "Y" | answer == "yes" | answer == "Yes" | answer == "YES") {

        i = 4;
        //Sub routine to enter command mode, change address, and revert to normal mode, no reboot.
        ChangeAddress();
        Serial.println("Change Made");
        
        Serial.print("Before Reboot ");
        ScanI2CBus();

        //reboot sensor by power off-on
        digitalWrite(13, LOW); delay(100); digitalWrite(13, HIGH); delay(1);

        Serial.print("After Reboot ");
        ScanI2CBus();
      }
        else
        {
          i = 0;
          OldI2CAddress = 0 ;
          NewI2CAddress = 0;
        }
      }
    }



  }


  // --------------------Subroutines hereafter--------------------------------

  void ChangeAddress() {
    digitalWrite(13, LOW);        //switches sensor off
    delay(5);
    digitalWrite(13, HIGH);        //switches sensor on
    delay(1);

    // Enter Command Mode
    Wire.beginTransmission(OldI2CAddress); Wire.write(0xA9);
    Wire.endTransmission();   // end transmission
    delay(1);

    // Read Status EEPROM Word 02
    Wire.beginTransmission(OldI2CAddress); Wire.write(0x22);
    Wire.endTransmission();   // end transmission
    delay(1);

    // data fetch of status
    Wire.requestFrom(OldI2CAddress, 3);
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();

    // print byte and calculated values
    Serial.print(data[0], BIN); Serial.print(","); Serial.print(data[1], BIN); Serial.print(",");Serial.println(data[2], BIN);
    Serial.print("Address In Memory: 0x"); if (data[3] < 16){Serial.print("0");} Serial.println(data[3], HEX);
/*
    // compile byte values from required new address value
    data[6] = ((NewI2CAddress >> 5) | 12);
    data[7] = (NewI2CAddress << 3);
    //Serial.print(data[1],BIN); Serial.print("\t"); Serial.print(data[2],BIN); Serial.println(",");
    //Serial.print(data[6],BIN); Serial.print("\t"); Serial.print(data[7],BIN); Serial.println(",");

    //Write New Address to EEPROM
    Wire.beginTransmission(OldI2CAddress); Wire.write(0x42); Wire.write(data[6]); Wire.write(data[7]);
     Wire.endTransmission();   // end transmission
*/
    // Set to Normal Mode
    Wire.beginTransmission(OldI2CAddress); Wire.write(0xA8);
    Wire.endTransmission();   // end transmission
  }

  void ScanI2CBus()
  {
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at address ");
        Serial.print(address, DEC); Serial.println(" (decimal)");

        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print("Unknow error at address ");
        Serial.println(address, DEC);
      }
    }
    if (nDevices == 0)
      Serial.println("No I2C devices found\n");
    else
      Serial.println("done\n");

  }


  /*
Copyright (c) 2016 Amphenol Advanced Sensors
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
