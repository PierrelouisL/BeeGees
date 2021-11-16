#include "bees.h"

#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>

void init_sensor_board(){
  BARO.begin(); 
  HTS.begin();
}

void get_sensor_board(data *data_board){
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
