#include "bees.h"

#include <Arduino_HTS221.h>

extern float echantillon[3];
extern float temperature_carte;   // Temperature ambiant
extern float Weight;              // Poids
extern float h_dht22 = 0;         // Humidité

int Temp_couvain = (int)echantillon[2]*10;
int Temp_cote1 = (int)echantillon[1]*10;
int Temp_cote2 = (int)echantillon[0]*10;
int Temp_ambiant = (int)temperature_carte*10;
int Poids = (int)0*10;
int Batterie = (int)0*10;
int Humidity = (int)0*10;

void PrintSigfox(){ 
  delay(100);
  char buffer_sigfox[96];
  // A modifier ->
  sprintf(buffer1, "AT$SF=%02x%02x%02x%02x%02x%02x%02x\n\r", Humidity, Batterie, Poids, Temp_ambiant, Temp_cote2, Temp_cote1, Temp_couvain);
  
  Serial1.write(buffer_sigfox);
}
