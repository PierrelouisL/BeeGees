#include "HX711.h"

HX711 scale;

/*const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;*/

float Offset;
float poids = 65; // Poids de référence

void setup(){
  Serial.begin(9600);

  scale.begin(D2, D3);

  scale.set_scale();
  scale.tare();
  Serial.println("Veuillez poser le poids");
  delay(10000);
  Serial.println("Je calibre bientôt");
  delay(10000);
  Serial.println("Je calibre");
  Offset = scale.get_units(20);
  Serial.print("Test = ");
  Serial.println(Offset);
  scale.set_scale(Offset/poids);
}

void loop(){
  Serial.print("Average:\t");
  Serial.println(scale.get_units(10), 2);

  scale.power_down();              // put the ADC in sleep mode
  delay(3000);
  scale.power_up();
}
