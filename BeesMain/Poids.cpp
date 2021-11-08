#include "bees.h"

#include <HX711.h>

HX711 scale;

float Offset = 21245.74; // Offset pour 65kg

/*
float Weight_ref = 65; // Poids de référence, à enelevé
float weight;

void init_HX711(){
  scale.begin(D2, D3);  // LOADCELL_DOUT_PIN = D2, LOADCELL_SCK_PIN = D3;

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
}*/

// --- Version final quand on saura le offset de la ruche 
void init_HX711(){
  Serial.begin(9600);
  scale.begin(D2, D3);  // LOADCELL_DOUT_PIN = D2, LOADCELL_SCK_PIN = D3;

  scale.set_scale(Offset);
}
  
void get_weight(data *data_weight){
  scale.power_up();               // Wake up
  delay(3000);
  scale.wait_ready_retry(3, 500); // Wait for sensor
  data_weight->Poids = scale.get_units(10) - 2.7;
  scale.power_down();             // Sleep mode
}
