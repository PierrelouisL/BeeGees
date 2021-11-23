#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fftr.h"
#include <PDM.h>

#define M_PI 3.14159265358979324
#define Size_TAB 2048 
#define Fmax 16000

typedef struct deuxFreqz{
	int Freqz1;
	int Freqz2;
}deuxFreqz;

void get_sample();
void check_sample_ready();
void FftReal();
deuxFreqz MaxFreq();
