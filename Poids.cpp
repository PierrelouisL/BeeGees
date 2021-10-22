#include "bees.h"
#include "HX711.h"

HX711 scale;

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

float Offset;
float Weight_ref = 65; // Poids de référence, à enelevé
float Weight;

void init_HX711(){
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale();
  scale.tare();

  // 20 secondes pour mettre un poids après le tare et l'offset mit à 0
  Serial.println("Veuillez poser le poids");
  delay(10000);
  Serial.println("Je calibre bientôt");
  delay(10000);
  Serial.println("Je calibre");
  Offset = scale.get_units(20);
  scale.set_scale(Offset/Weight_ref);
}

/* --- Version final quand on saura le offset de la ruche 
void Init_Poids(){
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(Offset);
  scale.tare();
}*/
void HX711_up(){
  scale.power_up();               // Wake up
}
void get_weight(){
  scale.wait_ready_retry(3, 500); // Wait for sensor
  Weight = scale.get_units(10);
  scale.power_down();             // Sleep mode
}
