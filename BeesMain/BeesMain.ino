#include "bees.h"

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
  init_PDM();
  //delay(10000);
}

void loop() {
  
  data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0};
  int buffer_int_sigfox[6];
  
  Serial.println("Capteur lets go");
  get_weight(&Alldata);
  get_DHT22(&Alldata);
  get_DS18B20(&Alldata);
  get_batterie(&Alldata);
  get_sensor_board(&Alldata);
  get_Abeilles(&Alldata);
  get_luminosite(&Alldata);
  delay(2000);
  data_10(&Alldata);                              // Adapter les unités des données
  Buffer_creation(Alldata, buffer_int_sigfox);    // Création du buffer pour l'envoie Sigfox
  PrintSigfox(buffer_int_sigfox);                 // Print sur Sigfox
  PrintSerial(Alldata);                           // Print sur le pc
  Serial.println("Fin du print");

  sleepFFT();
  sleepcard();
  delay(20000); // 660000 pour 11 mins
  UNsleepcard();
  unsleepFFT();
}
