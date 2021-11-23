#include "bees.h"

#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>

void init_sensor_board(){
  BARO.begin(); 
  HTS.begin();
  pinMode(A5, INPUT);
}

void get_sensor_board(data *data_board){

  float R1 = 10000;
  float Vin = analogRead(A0) * (3.3/1024.0);
  if(!HTS.begin()){
    HTS.begin();
  }
  if(!BARO.begin()){
    BARO.begin();
  }
  data_board->Pression = BARO.readPressure();
  data_board->Temp_ambiant = HTS.readTemperature();
  data_board->Humi_ambiant = HTS.readHumidity();
}

void get_luminosite(data *data_Luminosite){
  float R1 = 10000.0;
  float Vin = analogRead(A5); // Un entier pour contenir une valeur variant de 0 à 1023
  float R0 = (R1*3.3/Vin) - R1;
  data_Luminosite->Luminosite = 500/(R0/1000);
}
