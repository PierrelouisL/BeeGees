#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fftr.h"
#include "FFTKiss.h"

#include <PDM.h>

/*#define Size_TAB 1024 
#define Fmax 16000*/

// Fe max = 16000Hz max du capteur
// Fe = 32000Hz
// Il faut une puissace de 2 : 2^15 = 32768 

// On prend 1024 car on alloue plusieur tableau dans la fonction
// sinon on n'a plus de place

// De plus en realiter(d'apres des test) le capteur ne capte pas bien
// (pas du tt) les frequences a ces extremes, notammment en haute frequences
// a cause du manque d'amplitude

// On choisie Fmax=15625 Hz donc Fe = 31250 Hz donc Te = 32us
// On choisie comme ca car Te doit etre un entier

// On a une precision de 15625/1024 = 15Hz environ
// peut etre moins et peut aller jusqua 4%

//  Recapitulatif:
//  N=1024
//  Fe=31250 Hz Te=32us Fmax=15625 Hz

/*
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
*/
/* void FftReal(){
  for(int ji;ji<Size_TAB;ji++){
    in[ji]=sampleBuffer[ji];
  }
  
  kiss_fftr_cfg cfg;

  if ((cfg = kiss_fftr_alloc(Size_TAB, 0/*is_inverse_fft*//*, NULL, NULL)) != NULL){*/
//    kiss_fftr(cfg, in, out);
//    free(cfg);
//  }
//  else{
//    Serial.println("not enough memory?\n");
//  }
//} */
/*
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
        /*deux_freqz.Freqz1 = fm;
        freqAmpli1 = ampliTemp;
    }    
	if(ampliTemp > freqAmpli2 && ampliTemp < freqAmpli1){// on regarde les amplitudes et on gardes la 2m plus haute
        deux_freqz.Freqz2 = fm;
		freqAmpli2 = ampliTemp;
    }
  }               
  deux_freqz.Freqz1=int(deux_freqz.Freqz1*(Fmax/Size_TAB))*1.04;  
  deux_freqz.Freqz2=int(deux_freqz.Freqz2*(Fmax/Size_TAB))*1.04;
  
  return deux_freqz;
}
*/