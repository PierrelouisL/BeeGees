#include "bees.h"

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  init_HX711();
  init_temp_humi_board();
  delay(2000);
  init_batterie();
}

void loop() {

  data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0};
  int buffer_int_sigfox[6];

  Serial.println("Capteur lets go");
  get_weight(&Alldata);
  delay(1000);
  get_DHT22(&Alldata);
  delay(1000);
  get_DS18B20(&Alldata);
  delay(1000);
  get_temp_humi_board(&Alldata);
  delay(1000);
  get_batterie(&Alldata);
  delay(2000);
  
  data_10(&Alldata);
  Buffer_creation(Alldata, buffer_int_sigfox);
  for(int i=0; i<6; i++){
    Serial.print(buffer_int_sigfox[i]);
    Serial.print(" - ");
  }
  PrintSigfox(Alldata, buffer_int_sigfox);
  Serial.println("Fin du print");
  delay(10000); // 660000 pour 11 mins
}
