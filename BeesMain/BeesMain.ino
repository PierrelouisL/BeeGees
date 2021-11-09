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

void sleepcard(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW); // turn off sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);
  //NRF_POWER->SYSTEMOFF = 1;
}
void UNsleepcard(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, HIGH); // turn on sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);
  //NRF_POWER->SYSTEMOFF = 0;
}

void loop() {

  data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0};
  int buffer_int_sigfox[6];

  Serial.println("Capteur lets go");
  get_weight(&Alldata);
  get_DHT22(&Alldata);
  get_DS18B20(&Alldata);
  get_temp_humi_board(&Alldata);
  get_batterie(&Alldata);

  delay(2000);
  data_10(&Alldata);
  Buffer_creation(Alldata, buffer_int_sigfox);
  for(int i=0; i<6; i++){
    Serial.print(buffer_int_sigfox[i]);
    Serial.print(" - ");
  }
  Serial.println("");
  PrintSigfox(Alldata, buffer_int_sigfox);
  Serial.println("Fin du print");

  sleepcard();
  delay(20000); // 660000 pour 11 mins
  UNsleepcard();
}
