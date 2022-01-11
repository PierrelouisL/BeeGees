#include "bees.h"
#include "mbed.h"

data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};

void setup()
{
  LedON();
  digitalWrite(D2, HIGH);
  delay(2000);
  //Serial.begin(9600);
  Serial1.begin(9600);

  // Init all the sensor
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
  init_PDM();
  
  while(Serial1.read() != -1){
    Serial1.write("AT\r\n");
  }

  delay(2000);
  get_DS18B20(&Alldata);  // Avoid "nan" value
  
  LedOFF();
}

void loop() {

  //data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};
  int buffer_int_sigfox[6];

  switch(Alldata.pwr){
    case ALL_SENSORS_ON:
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      get_batterie(&Alldata);
      get_sensor_board(&Alldata);
      //get_Abeilles(&Alldata);
      //get_luminosite(&Alldata);
      break;
    case NO_ANALOG:
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      get_sensor_board(&Alldata);   
      get_Abeilles(&Alldata);
      break;
    case NO_BOARD_SENSORS:
      get_weight(&Alldata);
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      break;
    case ESSENTIALS:
      get_weight(&Alldata);
      get_DS18B20(&Alldata);
      break;  
  }
  
  data_10(&Alldata);                              // Adapter les unités des données
  Buffer_creation(Alldata, buffer_int_sigfox);    // Création du buffer pour l'envoie Sigfox
  PrintSigfox(buffer_int_sigfox);                 // Print sur Sigfox  
  //PrintSerial(Alldata);                           // Print sur le pc
  
  sleepcard();
  //Ronflex_Lvl_Sleep();
  delay(595000); // 600000 pour 10 mins
  UNsleepcard();
  unsleepFFT();
  delay(5000);
}
