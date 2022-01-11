#include "bees.h"

#include <HX711.h>

HX711 scale;

float Offset = 1931294.00; // Offset pour 66kg


float Weight_ref = 65; // Poids de référence, à enlever
/*float weight;

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
  scale.begin(D6, D5);      // LOADCELL_DOUT_PIN = D7, LOADCELL_SCK_PIN = D6;
  scale.set_scale(Offset/Weight_ref);
}
  
void get_weight(data *data_weight){
  scale.power_up();               
  delay(3000);
  if(scale.wait_ready_retry(3, 500)){ 
    data_weight->Poids = scale.get_units(5);
    data_weight->Poids = data_weight->Poids * 10;
  } 
  if(data_weight->Poids < 0){
    data_weight->Poids = 0;
  }
  scale.power_down();             
}
