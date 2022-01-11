/* Version 1.18  */
#include "bees.h"
#include "mbed.h"

data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};

void setup()
{
  digitalWrite(D2, HIGH);
  LedON();
  delay(3000);
  //Serial.begin(9600);
  Serial1.begin(9600);
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
  init_PDM();
  delay(3000);
  get_DS18B20(&Alldata);
  LedOFF();
}

void loop() {

  //data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};
  int buffer_int_sigfox[6];
  
  switch(Alldata.pwr){
    case ALL_SENSORS_ON:
      //Serial.println("lets go batterie");
      get_weight(&Alldata);
      //Serial.println("lets go DHT22");
      get_DHT22(&Alldata);
      //Serial.println("lets go DS18B20");
      get_DS18B20(&Alldata);
      //Serial.println("lets go bat");
      get_batterie(&Alldata);
      //Serial.println("lets go inboard");
      get_sensor_board(&Alldata);
      //Serial.println("lets go freq");
      get_Abeilles(&Alldata);
      //Serial.println("lets go lum");
      get_luminosite(&Alldata);
      break;
    case NO_ANALOG:
      //Serial.println("Capteur lets go NO_ANALOG");
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      get_sensor_board(&Alldata);   
      get_Abeilles(&Alldata);
      break;
    case NO_BOARD_SENSORS:
      //Serial.println("Capteur lets go NO_BOARD_SENSORS");
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      break;
    case ESSENTIALS:
      //Serial.println("Capteur lets go ESSENTIALS");
      get_weight(&Alldata);
      get_DS18B20(&Alldata);
      break;  
  }

  //PrintSerial(Alldata);                           // Print sur le pc
  data_10(&Alldata);                              // Adapter les unités des données
  Buffer_creation(Alldata, buffer_int_sigfox);    // Création du buffer pour l'envoie Sigfox
  PrintSigfox(buffer_int_sigfox);                 // Print sur Sigfox  

  sleepcard();
  //Ronflex_Lvl_Sleep();
  digitalWrite(D2, LOW);
  delay(600000); // 600000 pour 10 mins
  digitalWrite(D2, HIGH);
  UNsleepcard();
  unsleepFFT();
}
