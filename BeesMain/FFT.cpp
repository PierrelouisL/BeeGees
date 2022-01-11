#include "bees.h"

#include <FFTKiss.h>
#include <PDM.h>

void init_PDM(){
  check_sample_ready();
  PDM.begin(1, Fmax);
}

void get_Abeilles(data *data_Abeilles){

  static float ancien_poids;

  deuxFreqz freqzs = MaxFreq();
  
  data_Abeilles->FreqzAbeilles[0] = freqzs.Freqz1;
  data_Abeilles->FreqzAbeilles[1] = freqzs.Freqz2;
  
  if( ( (freqzs.Freqz1 > 340)&&(freqzs.Freqz1 < 450) &&(freqzs.Freqz2 > 340)&&(freqzs.Freqz2 < 450) && (data_Abeilles->Poids - ancien_poids > 1) ) ){
    data_Abeilles->EtatAbeilles = 3;
  }
  else if( (freqzs.Freqz1 > 2800)&&(freqzs.Freqz2 > 3000) ){
    data_Abeilles->EtatAbeilles = 2;
  }
  else if( (freqzs.Freqz1 > 125)&&(freqzs.Freqz1 < 300) && (freqzs.Freqz2 > 125)&&(freqzs.Freqz2 < 300)){
    data_Abeilles->EtatAbeilles = 1;
  }
  else{
    data_Abeilles->EtatAbeilles = 0;
  }
  
  ancien_poids = data_Abeilles->Poids;
  
  PDM.end();
}

void unsleepFFT(){
  if(!PDM.begin(1, Fmax)){
    Serial.println("Fail to start FFT");
  }
}
