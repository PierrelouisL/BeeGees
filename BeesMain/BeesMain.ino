#include "bees.h"

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
}

void loop() {

  data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0};
  int buffer_int_sigfox[6];

  //Serial.println("Capteur lets go");
  get_weight(&Alldata);
  get_DHT22(&Alldata);
  get_DS18B20(&Alldata);
  get_batterie(&Alldata);
  get_sensor_board(&Alldata);

  delay(2000);
  data_10(&Alldata);
  Buffer_creation(Alldata, buffer_int_sigfox);
  PrintSigfox(Alldata, buffer_int_sigfox);
  //Serial.println("Fin du print");
  
  sleepcard();
  delay(600000); // 660000 pour 11 mins
  UNsleepcard();
}
