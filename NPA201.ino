/*
Note: This sketch requires is set to use an Adafuit SSD1306 0.96" screen or
compatible and requires the libraries to drive that which are not included here.
 
 * */
 */#include "Wire.h"

//enable display
#define ENABLE_LCD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define I2CAddress 0x27
  long aa, bb, cc, dd, ee;
  float temperature = 0.00;
  float pressure = 0.00;

#ifdef ENABLE_LCD
void displayReading(int press, float temp) {
  //display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("   Amphenol Sensors  "); 
  display.setCursor(0, 55);
  display.print("  Barometric Pressure");

  display.setTextSize(2);
  display.setCursor(23, 15);
  display.print(press);
  display.setCursor(23, 36);
  display.setTextSize(2);
  display.print(temp);

  display.setTextSize(1);
  display.setCursor(88, 15);  
  display.print("hPa");
  display.setCursor(88, 36);
  display.print (char(247)); display.print ("C");

  display.display();
}
#endif //ENABLE_LCD

void setup() {
    pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        //switches sensor on
  Serial.begin(115200);  // start serial for output
  Wire.begin();
  Serial.println("Amphenol Advanced Sensors");
  Serial.println("Arduino NPA-201 Monitor");

  #ifdef ENABLE_LCD
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("Amphenol");
  display.setCursor(5, 25);
  display.print("Advanced");
display.setCursor(5, 45);
  display.print("Sensors");
  display.display();
  delay(1000);
  display.display();
#endif //ENABLE_LCD
}


void NPA201ReadData()
{


  // Initiate Comms to device, initiate measure and read 5 bytes of data
  Wire.beginTransmission(I2CAddress);
  Wire.write(0xAC);  Wire.write(0);
  Wire.endTransmission();
  delay(21);
  Wire.requestFrom(I2CAddress, 5);
  aa = Wire.read();
  bb = Wire.read();
  cc = Wire.read();
  dd = Wire.read();
  ee = Wire.read();

  //Pressure Value = BridgeData/65535 * (1260-260) + 260 (hPa)
  //Temperature_Value = TempData/65535 * (85+40) - 40   (°C)
  // additional calculations to make values reasonable based on accuracy

  pressure =  (float) ((bb << 8) | cc) / 65535 * 1000 + 260;
  pressure = round(pressure);

  temperature =  (float) ((dd << 8) | ee) / 65535 * 125 - 40;
  temperature = round(temperature * 10);
  temperature = temperature / 10;
 
#ifdef ENABLE_LCD
      displayReading(pressure, temperature);
#endif //ENABLE_LCD  
}

void loop() {
  NPA201ReadData();
  
  Serial.print(int (pressure)); Serial.print("  "); Serial.print("hPa"); Serial.print("  ");
  Serial.print(temperature); Serial.print("  "); Serial.write(176); Serial.println("C");

  delay(3000);
}

  /*
Copyright (c) 2017 Amphenol Advanced Sensors
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
