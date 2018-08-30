/* The sketch requires the U8g2 library be loaded for the OLED display. Display can either 
 * be mounted on the Advanced Sensors Shield or on terminals 8 through 13.
 * 
 * Please adjust values below (to line) to suit your sensor
 */


//Connection V+ve - Pressure - Ground, Pressure to Analogue Input (default 0)
#define units "inches H2O"                 //define display units
float sensorRange = 4   ;                  //Define value dependent upon sensor
#define decPlaces 2                        //define how many decimal places to pring / display
/*
   0-5" water sensorRange value = 5
   0-40" water sensorRange value = 5
   0-45" water sensorRange value = 5
   0-100mB  sensorRange value = 100 etc.
*/
#define inputPin 0                         //Analogue Input Reference
#define BaudRate 19200                     //USB Serial baud rate
unsigned long samplerate = 1000;           //time interval for sample aquisition in mS
#define Vin 3.3                              //input voltage, 5 for NPA500, 3.3 for NPA300

//_______________________values above may be altered_____________________________________

#include <U8g2lib.h>  //library for display

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10);
//U8G2_R2 inverts display, U8G2_R0 is standard orientation


#define Version "Ratiometric NPA v1.0"

unsigned long sampletime;       // variable for timer


void setup() {
  Serial.begin(BaudRate);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);                 //power to screen

  u8g2.begin();                           //start OLED Driver

  Serial.println(F(Version));
  Serial.println(F("Amphenol Advanced Sensors NPA500"));
  float opVolt = Vin;
  if (opVolt = 5) {
    Serial.print(F("NPA500 "));
  } else {
    Serial.print(F("NPA300 "));
  }
  Serial.print(F("Code set to read 0-"));
  Serial.print(sensorRange);
  Serial.print(F(" "));
  Serial.print(units);
  Serial.println();


  u8g2_prepare();
  u8g2.drawStr( 5, 30, Version);
  u8g2.sendBuffer();
  delay(1500);

  u8g2_prepare();
  for (int i = 128; i > -120; i--) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB14_tr);
    u8g2.drawStr( i, 20, "Amphenol");
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr( i, 40, "Advanced Sensors");
    u8g2.sendBuffer();
    delay(10);
  }
  delay(100);

}

void loop() {
  if (millis() >= (samplerate + sampletime))
  {
    sampletime = millis();  //resets timer
    float Pressure = ReadPressure(inputPin);
    displayReading(Pressure);
    Serial.println(Pressure, decPlaces);
  }
}

//___________________________sub routines_________________________________________
float ReadPressure(byte inPin) {
  float variable = analogRead(inPin);         //read input
  float opVolt = Vin;
  variable = variable * 5 / opVolt ;          //correction for sensor operating voltage
  variable = variable * 100 / 1023 ;          //input as percentage
  variable = ( variable * 1.25 ) - 12.5;      //ratiometric percentage
  variable = variable * sensorRange / 100;  //convert to units
  return variable;
}


//____________________________display routines_____________________________________
void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
}

void displayReading(float pressure) {
  u8g2_prepare();

  u8g2.drawStr(20, 0, "Amphenol Sensors");
  u8g2.drawStr(50, 50, units);

  u8g2.setFont(u8g2_font_helvB14_tr);
  u8g2.setCursor(15, 20);
  u8g2.print(pressure, decPlaces);


  u8g2.sendBuffer();
}

/*
Copyright (c) 2018 Amphenol Advanced Sensors
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
