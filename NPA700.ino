#include "Wire.h"

//For Barometric NPA-700N-17.5A in hPa, Multiplier = 1000, Offset = 200 
// For NPA-700N-015A in hPa, Multiplier = 1034.212, Offset = 0  

#define I2CAddress 0x28
#define Units "hPa"
#define Multiplier 1000
#define Offset 200


int k = 1;

void setup(){
  
  Serial.begin(115200);  // start serial for output
  Wire.begin();
  Serial.println("Amphenol Advanced Sensors");
  Serial.println("Arduino NPA700 Monitor");
}

void getdata(byte *a, byte *b, byte *c, byte *d)
{
  Wire.beginTransmission(I2CAddress);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(I2CAddress, 4);
  *a = Wire.read();
  *b = Wire.read();
  *c = Wire.read();
  *d = Wire.read();
}
 void showthedata()
{
  byte aa,bb,cc,dd;
  float temperature=0;  float pressure=0;
  float Data_value;
  getdata(&aa,&bb,&cc,&dd);

  Data_value = ((aa & 0x3F )<< 8) | bb ;
//Serial.print(aa);Serial.print(", ");Serial.print(bb);Serial.print(", ");Serial.print(cc);Serial.print(", ");Serial.print(dd);Serial.print(", ");

pressure = (float)(( Data_value - 1638 ) / ( 14745 - 1638 )) * Multiplier + Offset;
pressure = round(pressure);
Serial.print(int (pressure));Serial.print("  ");Serial.print(Units);Serial.print("  ");

  if ( k < 5 )
    {
    k++;
    } else {
    Serial.println(" ");
    k = 1;
  }
}
void loop(){

  showthedata();
  
  delay(3000);
}
