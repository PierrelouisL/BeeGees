#include "bees.h"

void data_10(data *data){
  data->Temp_couvain     = (data->Temp_couvain+20)*10;  
  data->Temp_cote[0]     = (data->Temp_cote[0]+20)*10;
  data->Temp_cote[1]     = (data->Temp_cote[1]+20)*10;
  data->Batterie         = data->Batterie;       
  data->Temp_ambiant     = (data->Temp_ambiant+20)*10;         
  data->Humi_couvain     = data->Humi_couvain*10;
  data->Humi_ambiant     = data->Humi_ambiant*10;
  data->Pression         = data->Pression;
  data->Poids            = data->Poids*10;  
  data->Luminosite       = 0; // à changer
  data->SON              = 0; // à changer
}

void Buffer_creation(data data, int *buffer_int_sigfox){
  
  int value_array[11]        = { (int)data.Temp_couvain, (int)data.Temp_cote[0], (int)data.Temp_cote[1], (int)data.Batterie,
                                 (int)data.Temp_ambiant, (int)data.Humi_couvain, (int)data.Humi_ambiant,(int)data.Pression,
                                 (int)data.Poids, (int)data.Luminosite, (int)data.SON};
  int nb_bit_value_array[11] = { 9, 9, 9, 9, 
                                 10, 10, 10, 10,
                                 11, 7,2};
  int buffer_bit_sigfox[NB_bits];
  int end_buffer = NB_bits-1;

  // Transformation des données en fonction des bits que l'on va envoyer
  for(int i=0; i<11; i++){
    for(int j=0; j<nb_bit_value_array[i]; j++){
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
 
  if(end_buffer <= 0){
    Serial.print("end_buffer = ");
    Serial.println(end_buffer);
  }
  else{
    while(end_buffer >= 0){
      buffer_bit_sigfox[end_buffer] = 0;
      end_buffer--;
    }
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
  char buffer_sigfox[50];
  // A modifier ->
  sprintf(buffer_sigfox, "AT$SF=%04x%04x%04x%04x%04x%04x\n\r", buffer_int_sigfox[0], buffer_int_sigfox[1], buffer_int_sigfox[2], buffer_int_sigfox[3], buffer_int_sigfox[4], buffer_int_sigfox[5]);

  Serial1.write(buffer_sigfox);

  // Verification
  Serial.print("Temp_couvain = ");      Serial.println(data.Temp_couvain);
  Serial.print("Temp_cote1 = ");        Serial.println(data.Temp_cote[0]); 
  Serial.print("Temp_cote2 = ");        Serial.println(data.Temp_cote[1]);
  Serial.print("Temp_ambiant = ");      Serial.println(data.Temp_ambiant);
  Serial.print("Poids = ");             Serial.println(data.Poids);
  Serial.print("Batterie = ");          Serial.println(data.Batterie);
  Serial.print("Humidite Couvain = ");  Serial.println(data.Humi_couvain);
  Serial.print("Humidite Ambiant = ");  Serial.println(data.Humi_ambiant);
  Serial.print("Pression = ");          Serial.print(data.Pression);        Serial.println("kPa");
  Serial.print("Luminosite = ");        Serial.println(data.Luminosite);
  Serial.print("SON = ");               Serial.println(data.SON);
}


// Sigfox CallBack V1
//poids:1:uint:12::1 humidity_ambiant:3:uint:10::5 humidity_couvain:4:uint:10::3 batterie:5:uint:10::1 temperature_ambiant:7:uint:10::7 temp_cote2:8:uint:10::5 temp_cote1:9:uint:10::3 temp_couvain:10:uint:10::1
// body
//{ 
//  "temperature-couvain" : {"value":"{customData#temp_couvain}"}, 
//  "temperature-cote-couvain-1" : {"value":"{customData#temp_cote1}"}, 
//  "temperature-cote-couvain-2" : {"value":"{customData#temp_cote2}"}, 
//  "temperature-ambiant" : {"value":"{customData#temperature_ambiant}"},  
//  "batterie" : {"value":"{customData#batterie}"}, 
//  "humidity-couvain" : {"value":"{customData#humidity_couvain}"},
//  "humidity" : {"value":"{customData#humidity_ambiant}"},
//  "poids" : {"value":"{customData#poids}"}
//}

// Sigfox CallBack V2 + pression, son, modif de nb de bit
// son:0:uint:2::7 luminosite:0:uint:7::5 poids:1:uint:11::6 pression:2:uint:10::3 humidity_ambiant:3:uint:10::1 humidity_couvain:5:uint:10::7 temperature_ambiant:6:uint:10::5 batterie:7:uint:9::3 temp_cote2:8:uint:9::2 temp_cote1:9:uint:9::1 temp_couvain:10:uint:9::0
// body
//{ 
//                         "son" : {"value":"{customData#son}"},
//                  "luminosite" : {"value":"{customData#luminosite}"},
//                       "poids" : {"value":"{customData#poids}"},
//                    "pression" : {"value":"{customData#pression}"},
//                    "humidity" : {"value":"{customData#humidity_ambiant}"},
//            "humidity-couvain" : {"value":"{customData#humidity_couvain}"},
//         "temperature-ambiant" : {"value":"{customData#temperature_ambiant}"},  
//                    "batterie" : {"value":"{customData#batterie}"},
//  "temperature-cote-couvain-2" : {"value":"{customData#temp_cote2}"}, 
//  "temperature-cote-couvain-1" : {"value":"{customData#temp_cote1}"},
//         "temperature-couvain" : {"value":"{customData#temp_couvain}"}
//}
