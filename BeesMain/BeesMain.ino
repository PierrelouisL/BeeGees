#include "bees.h"
#include "mbed.h"

data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};

void setup()
{
  delay(3000);
  //Serial.begin(9600);
  Serial1.begin(9600);
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
  init_PDM();
  delay(3000);
  Serial1.write("AT$SF=0x12345678\n\r");
}

void loop() {

  //data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};
  int buffer_int_sigfox[6];
  
  switch(Alldata.pwr){
    case ALL_SENSORS_ON:
      //Serial.println("Capteur lets go all");
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      get_batterie(&Alldata);
      get_sensor_board(&Alldata);
      get_Abeilles(&Alldata);
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
  
  data_10(&Alldata);                              // Adapter les unités des données
  Buffer_creation(Alldata, buffer_int_sigfox);    // Création du buffer pour l'envoie Sigfox

  PrintSigfox(buffer_int_sigfox);                 // Print sur Sigfox  
  //PrintSerial(Alldata);                           // Print sur le pc
  //Serial.println("Fin du print");

  //sleepFFT();
  sleepcard();
  //Ronflex_Lvl_Sleep();
  delay(20000); // 600000 pour 10 mins
  UNsleepcard();
  unsleepFFT();
}
