#include "bees.h"

void data_10(data *data){
  data->Temp_couvain     = data->Temp_couvain*10;  
  data->Temp_cote[0]     = data->Temp_cote[0]*10;
  data->Temp_cote[1]     = data->Temp_cote[1]*10;       
  data->Temp_ambiant     = data->Temp_ambiant*10;  
  data->Poids            = data->Poids*10;         
  data->Batterie         = data->Batterie*10;
  data->Humi_couvain     = data->Humi_couvain*10;
  data->Humi_ambiant     = data->Humi_ambiant*10;
}

void Buffer_creation(data data, int *buffer_int_sigfox){
  
  int value_array[NB_value] = {  (int)data.Temp_couvain, (int)data.Temp_cote[0], (int)data.Temp_cote[1], 
                          (int)data.Temp_ambiant, (int)data.Batterie, 
                          (int)data.Humi_couvain, (int)data.Humi_ambiant,(int)data.Poids };
  int buffer_bit_sigfox[NB_bits];
  int end_buffer = NB_bits-1;

  // Transformation des températures, batterie et humidités sur 10 bits
  for(int i=0; i<NB_value-1; i++){
    for(int j=0; j<10; j++){
      if(value_array[i] > 1023){
        buffer_bit_sigfox[end_buffer] = 1;
      }
      else{
        if((value_array[i] % 2) == 0){
          buffer_bit_sigfox[end_buffer] = 0;
        }
        else{
          buffer_bit_sigfox[end_buffer] = 1;
        }
        value_array[i] = value_array[i]/2;
      }
      end_buffer--;
    }
  }
  // Transformation du poids sur 12 bits
  for(int j=0; j<12; j++){
    if((value_array[NB_value-1] % 2) == 0){
      buffer_bit_sigfox[end_buffer] = 0;
    }
      else{
    buffer_bit_sigfox[end_buffer] = 1;
    }
    value_array[NB_value-1] = value_array[NB_value-1]/2;
    end_buffer--;
  }

  while(end_buffer >= 0){
    buffer_bit_sigfox[end_buffer] = 0;
    end_buffer--;
  }
  
  end_buffer = NB_bits-1;
  int temp;

  // Préparation de l'envoie sur Sigfox
  for(int i=5; i>=0; i--){
    temp = 0;
    for(int j=0; j<16; j++){
      temp = temp + buffer_bit_sigfox[end_buffer]*pow(2, j);
      end_buffer--;
    }
    buffer_int_sigfox[i] = temp;
  }
}

void PrintSigfox(data data, int buffer_int_sigfox[]){        
  delay(100);
  char buffer_sigfox[50];
  // A modifier ->
  sprintf(buffer_sigfox, "AT$SF=%04x%04x%04x%04x%04x%04x\n\r", buffer_int_sigfox[0], buffer_int_sigfox[1], buffer_int_sigfox[2], buffer_int_sigfox[3], buffer_int_sigfox[4], buffer_int_sigfox[5]);

  Serial1.write(buffer_sigfox);

  // Verification
  Serial.print("Temp_couvain = ");
  Serial.println(data.Temp_couvain);
  Serial.print("Temp_cote1 = ");
  Serial.println(data.Temp_cote[0]);
  Serial.print("Temp_cote2 = ");
  Serial.println(data.Temp_cote[1]);
  Serial.print("Temp_ambiant = ");
  Serial.println(data.Temp_ambiant);
  Serial.print("Poids = ");
  Serial.println(data.Poids);
  Serial.print("Batterie = ");
  Serial.println(data.Batterie);
  Serial.print("Humidity Couvain = ");
  Serial.println(data.Humi_couvain);
  Serial.print("Humidity Ambiant = ");
  Serial.println(data.Humi_ambiant);
}


// Sigfox CallBack
//poids:1:uint:12::0 humidity:2:uint:10::3 batterie:3:uint:10::1 temperature_ambiant:5:uint:10::7 temp_cote2:6:uint:10::5 temp_cote1:7:uint:10::3 temp_couvain:8:uint:10::1// body
//{ 
//  "temperature-couvain" : {"value":"{customData#temp_couvain}"}, 
//  "temperature-cote-couvain-1" : {"value":"{customData#temp_cote1}"}, 
//  "temperature-cote-couvain-2" : {"value":"{customData#temp_cote2}"}, 
//  "temperature-ambiant" : {"value":"{customData#temperature_ambiant}"},  
//  "batterie" : {"value":"{customData#batterie}"}, 
//  "humidity" : {"value":"{customData#humidity}"},
//  "poids" : {"value":"{customData#poids}"}
//}

// Thomas
//{ 
//  "temperature-couvain" : {"value":"{customData#temp_couvain}"}, 
//  "temperature-cote-couvain-1" : {"value":"{customData#temp_cote1}"}, 
//  "temperature-cote-couvain-2" : {"value":"{customData#temp_cote2}"}, 
//  "temperature-ambiant" : {"value":"{customData#temperature_ambiant}"},  
//  "humidity" : {"value":"{customData#humidity}"},
//  "humidity-couvain" :{"value":"{customData#humidity_c}"},
//  "poids" : {"value":"{customData#poids}"}
//}
