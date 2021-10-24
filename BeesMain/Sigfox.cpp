#include "bees.h"

extern float echantillon[3];      // DS18B20 Sensor
extern float board_temperature;   // Board Sensor
extern float weight;              // HX711 Sensor
extern float h_dht22;             // Humidity DHT22 Sensor 

int Temp_couvain  = (int)echantillon[2]*10;
int Temp_cote1    = (int)echantillon[1]*10;
int Temp_cote2    = (int)echantillon[0]*10;
int Temp_ambiant  = (int)board_temperature*10;
int Poids         = (int)Weight*10;
int Batterie      = (int)0*10;
int Humidity      = (int)h_dht22*10;

int buffer_int_sigfox[6];

int* Buffer_creation(){
  int value_array[7] = { Temp_couvain, Temp_cote1, Temp_cote2, Temp_ambiant, Batterie, Humidity, Poids};
  bool buffer_bit_sigfox[96];
  int end_buffer = 95;
  
  for(int i=0; i<6; i++){
    
    for(int j=0; j<9; j++{
      if(value_array[i] > 1023){
        buffer_sigfox[end_buffer] = 1;
      }
      else{
        if((value_array[i] % 2) == 0){
          buffer_sigfox[end_buffer] = 0;
        }
        else{
          buffer_sigfox[end_buffer] = 1;
      }
      value_array[i]/2;
      }
      end_buffer--;
    }
  } 
  for(int j=0; j<12; j++{
    if((value_array[6] % 2) == 0){
      buffer_sigfox[end_buffer] = 0;
    }
      else{
    buffer_sigfox[end_buffer] = 1;
    }
    value_array[i]/2;
    end_buffer--;
  }

  while(end_buffer >= 0){
    buffer_sigfox[end_buffer] = 0;
    end_buffer--;
  }
  
  end_buffer = 96;
  int temp;
  
  for(int i=5; i>=0; i--){
    temp = 0;
    for(int j=0; j<16; j++){
      temp = temp + buffer_sigfox[end_buffer]*pow(2, j);
      end_buffer--;
    }
    buffer_int_sigfox[i] = temp;
  }
}
void PrintSigfox(){ 
  delay(100);
  char buffer_sigfox[96];
  // A modifier ->
  sprintf(buffer_sigfox, "AT$SF=%04x%04%04x%04x%04x%04x\n\r", buffer_int_sigfox[0], buffer_int_sigfox[1], buffer_int_sigfox[2], buffer_int_sigfox[3], buffer_int_sigfox[4], buffer_int_sigfox[5]);
  
  Serial1.write(buffer_sigfox);
}
