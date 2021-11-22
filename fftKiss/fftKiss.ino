//#include "mbed.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fftr.h"

#include <PDM.h>

#ifndef M_PI
#define M_PI 3.14159265358979324
#endif

// definition des constantes


#define N 2048 //tableau de taille N
#define Te_us 32
#define Fmax 16000

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

// buffer to read samples into, each sample is 16-bits
short sampleBuffer[N];

// number of samples read
volatile int samplesRead;

void onPDMdata();
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);
  Serial.print("helllooooo\n");

  while (!Serial);
  Serial.print("helllooooo22\n");

  // configure the data receive callback
  PDM.onReceive(onPDMdata);

  // optionally set the gain, defaults to 20
  // PDM.setGain(30);
Serial.print("helllooooo333\n");
  // initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate
  if (!PDM.begin(1, Fmax)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
  Serial.print("helllooooo444\n");
}

void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}




unsigned int temp=0;

unsigned int sendValue=0;

//Ticker flipper;

// tableau de la fft kissfft
kiss_fft_scalar in[N];
kiss_fft_cpx out[N / 2 + 1];



// fonction adapter du poste de Alexey Frunze
// https://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
// Jan 26 '13 at 14:29
//  pour executer la kissfft
void TestFftReal()
{
  for(int ji;ji<N;ji++)
  {
    in[ji]=sampleBuffer[ji];
  }
  
  kiss_fftr_cfg cfg;

  if ((cfg = kiss_fftr_alloc(N, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
  {
    kiss_fftr(cfg, in, out);
    free(cfg); 
  }
  else
  {
    printf("not enough memory?\n");
    exit(-1);
  }
}
// Fin de l'algorithme FFT


// Fonction pour avoir la freq max
unsigned int freqMax;
unsigned int freqMax2;

double freqMaxAMP1;
double freqMaxAMP2;

void MaxFreq()
{
    unsigned int freq1 = 0;
    float freqAmpli1 = 0;
    
    float ampliTemp = 0;
    
    unsigned int freq2 = 0;
    float freqAmpli2 = 0;
        
    for(int fm = 1; fm < N/2; fm++)
    {
        ampliTemp = sqrt(out[fm].r*out[fm].r + out[fm].i*out[fm].i);
        if(ampliTemp > freqAmpli1)// on regarde les amplitudes et on gardes la plus haute
        {
            freq1 = fm;
            freqAmpli1 = ampliTemp;
        }
        
        if(ampliTemp > freqAmpli2 && ampliTemp < freqAmpli1)// on regarde les amplitudes et on gardes la 2m plus haute
        {
            freq2 = fm;
            freqAmpli2 = ampliTemp;
        }
        
    }               
    
//  //Si on ne veut pas prendre en dessous d'une certaine frequence
//  printf(" %.f : frq\r\n",sqrt(data[2*freq1]*data[2*freq1] + data[2*freq1+1]*data[2*freq1+1]));
//  printf(" %.f : frq\r\n",sqrt(data[2*freq2]*data[2*freq2] + data[2*freq2+1]*data[2*freq2+1]));
//          
//  if(sqrt(data[2*freq1]*data[2*freq1] + data[2*freq1+1]*data[2*freq1+1])<115) // on ne prend pas les sons sous 41.2 dB
//      freq1=0;
//  if(sqrt(data[2*freq2]*data[2*freq2] + data[2*freq2+1]*data[2*freq2+1])<115)
//      freq2=0;
    
    freqMaxAMP1=sqrt(out[freq1].r*out[freq1].r + out[freq1].i*out[freq1].i );
    freqMaxAMP2=sqrt(out[freq2].r*out[freq2].r + out[freq2].i*out[freq2].i );
        
    freqMax=int(freq1*(Fmax/N))*1.12;      
    freqMax2=int(freq2*(Fmax/N))*1.12;
    //printf(" %.i : frq\r\n",int(max_freq*(Fmax*2/N)));
}

int etat_des_abeille;

void etat_abeilles(){
  if( (freqMax> 340)&&(freqMax< 450)&&(freqMax2> 340)&&(freqMax2< 450) ){
    etat_des_abeille=3;
  }
  else if( (freqMax> 340)&&(freqMax2> 3000) ){
    etat_des_abeille=2;
  }
  else if( (freqMax> 125)&&(freqMax< 300)&&(freqMax2> 125)&&(freqMax2< 300) ){
    etat_des_abeille=1;
  }
  else
    etat_des_abeille=0;
  
}
 
void loop() {  
  delay(2000);
    Serial.print("hellloooottttttto\n");
    Serial.print("loop debut\n");
    //flipper.attach_us(getSgnal,Te_us);// debut de remplissage du tableau
    
   
        if (samplesRead) // si tableau plein
        {
            samplesRead=0;
                        
            TestFftReal();           
                  
            MaxFreq();// trouver les freq max
                        
            // affichache freq 1
            Serial.print("freq:");
            Serial.println(int(freqMax*1));// on adapte avec une erreur de 1.5%
            Serial.print("db:");
            Serial.println(20*log10(freqMaxAMP1));
 
            // affichache freq 2
            Serial.print("freq: ");
            Serial.println(int(freqMax2*1));// on adapte avec une erreur de 1.5%
            Serial.print("db:");
            Serial.println(20*log10(freqMaxAMP2));

            etat_abeilles();
            Serial.println(etat_des_abeille);
            // l'erreur de 12% inferieur est sans doute du a la clock
            // de la carte qui n'ai pas presise , aucune idee de pourqoui
            
            // on a une frequence precise a 4%(normalement toujours en dessous)
            
        }   
    
}

//--------------------------------------------
