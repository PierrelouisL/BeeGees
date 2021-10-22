#include "bees.h"

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  
  init_tempboard();
  init_HX711();
}


void loop() {
  delay(1000);
  testBluetooth();
  testGPS();
}
