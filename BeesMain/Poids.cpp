#include "bees.h"

#include <HX711.h>

HX711 scale;

float Offset = 1931294.00; // Offset 2711787.00 pour 84.5, Offset 1936859.00 pour 66kg
float Weight_ref = 65.00; // Poids de référence


/*void init_HX711(){
  scale.begin(D6, D5);  // LOADCELL_DOUT_PIN = D2, LOADCELL_SCK_PIN = D3;

  scale.set_scale();
  scale.tare();

  // 20 secondes pour mettre un poids après le tare et l'offset mit à 0
  Serial.println("Veuillez poser le poids");
  delay(9000);
  Serial.println("Je calibre bientôt");
  delay(9000);
  Serial.println("Je calibre");
  Offset = scale.get_units(20);
  Serial.print("Offset = ");
  Serial.println(Offset);
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
