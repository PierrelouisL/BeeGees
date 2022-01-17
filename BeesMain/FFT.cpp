#include "bees.h"

/*#include <FFTKiss.h>
#include <PDM.h>*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fftr.h"
#include "FFTKiss.h"

#include <PDM.h>

#define Size_TAB 512 
#define Fmax 16000

short sampleBuffer[Size_TAB];    // buffer to read samples into, each sample is 16-bits
volatile int samplesRead; // number of samples read

// tableau de la fft kissfft
kiss_fft_scalar in[Size_TAB];
kiss_fft_cpx out[Size_TAB / 2 + 1];

void get_sample(){
  int bytesAvailable = PDM.available();   // query the number of bytes available
  PDM.read(sampleBuffer, bytesAvailable); // read into the sample buffer
  samplesRead = bytesAvailable / 2;       // 16-bit, 2 bytes per sample
}

void check_sample_ready(){
  PDM.onReceive(get_sample); // configure the data receive callback
}

void FftReal(){
  for(int ji;ji<Size_TAB;ji++){
    in[ji]=sampleBuffer[ji];
  }
  
  kiss_fftr_cfg cfg;

  if ((cfg = kiss_fftr_alloc(Size_TAB, 0/*is_inverse_fft*/, NULL, NULL)) != NULL){
    kiss_fftr(cfg, in, out);
    free(cfg);
  }
  else{
    Serial.println("not enough memory?\n");
    free(cfg);
  }
}

deuxFreqz MaxFreq(){
  
  deuxFreqz deux_freqz;
  deux_freqz.Freqz1 = 0;
  deux_freqz.Freqz2 = 0;  
  float freqAmpli1 = 0;
  float freqAmpli2 = 0;
  float ampliTemp = 0;

  //check_sample_ready();            
  FftReal();  
    
  for(int fm = 1; fm < Size_TAB/2; fm++){
    ampliTemp = sqrt(out[fm].r*out[fm].r + out[fm].i*out[fm].i);
    if(ampliTemp > freqAmpli1){ /* on regarde les amplitudes et on gardes la plus haute */
        deux_freqz.Freqz1 = fm;
        freqAmpli1 = ampliTemp;
    }    
  if(ampliTemp > freqAmpli2 && ampliTemp < freqAmpli1){// on regarde les amplitudes et on gardes la 2m plus haute
        deux_freqz.Freqz2 = fm;
    freqAmpli2 = ampliTemp;
    }
  }               
  deux_freqz.Freqz1=int(deux_freqz.Freqz1*(Fmax/Size_TAB))*1.035;  
  deux_freqz.Freqz2=int(deux_freqz.Freqz2*(Fmax/Size_TAB))*1.035;
  
  return deux_freqz;
}

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
  else if( (freqzs.Freqz1 > 2900)&&(freqzs.Freqz2 > 2900) ){
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
