#include <SevenSeg.h>

/*
Measuring Current Using ACS712
*/
const int V5 = A1;
const int V12 = A2;
int mVperAmp1 = 155; // use 185 for 5a, 100 for 20A Module and 66 for 30A Module
int mVperAmp2 = 70; // use 185 for 5a, 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
// store last 10 readings of both voltages
float amps[2][10] = { 
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0}
};
short it = 0; // iterator for the array of readings

const int DIGITS = 4;
int digpins[DIGITS] = {12,11,10,9};
SevenSeg show(2,3,5,7,8,4,1);


// read analog input and calculate amperage based on milivolts-per-amp
double readAmps(const int aPin, const int milivoltsAmp, const int index)
{
  int RawValue = analogRead(aPin);
  double Voltage = (RawValue / 1023.0) * 5000; // Gets you mV
  double Amps = ((Voltage - ACSoffset) / milivoltsAmp);
  /*
  Serial.print("Raw Value = " ); // shows pre-scaled value 
  Serial.print(RawValue); 
  Serial.print("\t mV = "); // shows the voltage measured 
  Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
  Serial.print("\t Amps = "); // shows the voltage measured 
  Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
  */
  amps[index][it] = Amps;
  return Amps;
}

void setup(){ 
  Serial.begin(115200);
  show.setDigitPins(DIGITS, digpins);
  show.setDPPin(6);
  long mil = millis();
  while ( millis() - mil < 1000 )
    show.write("Hi!");
  delay(1000);
  mil = millis();
  while ( millis() - mil < 1000 )
    show.write("Were");
  delay(1000);
  mil = millis();
  while ( millis() - mil < 1000 )
    show.write("up");
  delay(1000);
}

float avg(const short index)
{
  float result = 0;
  for ( int j = 0; j < 10 ; j++)
    result += amps[index][j];
  if (result > 0)
    return result/10;
  else
    return 0;
}

void loop()
{
  
  readAmps(V5, mVperAmp1, 0);
  float a5 = avg(0);
  if (a5 > 0.05)
  {
    long mil = millis();  
    while ( millis() - mil < 1000 )
      show.write(a5);
  }
  
  readAmps(V12, mVperAmp2, 1);
  float a12 = avg(1);
  if (a12 > 0.05) 
  {
    long mil = millis();
    while ( millis() - mil < 1000 )
      show.write(a12);
  }
  
  if (++it > 9) it = 0;
}


