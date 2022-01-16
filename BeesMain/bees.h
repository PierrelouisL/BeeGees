#ifndef __BEES__
#define __BEES__

// ----------------------------------------------------
//  Variables globales et librairies 

#include <Arduino_HTS221.h>
#include "mbed.h"


// Temperature.cpp global value
  // DHT Sensor
  #define DHTPIN 7      // Pin DHT22
  #define DHTTYPE DHT22 // DHT 22 (AM2302)
  // DS18B20 Sensor
  #define PIN_BUS 4     // Pin DS18B20 (sonde)
  #define MAXIMWIRE_EXTERNAL_PULLUP

// Sigfox.cpp global value
#define NB_bits 96
// ----------------------------------------------------

typedef enum Power_saving_state{
  ALL_SENSORS_ON,
  NO_ANALOG, // No Battery + No Light check
  NO_BOARD_SENSORS, // No analog + no board sensors except Temp
  ESSENTIALS // Only 2 DS18B20 sensors and Weight
}Power_saving_state;

typedef enum States{
  INIT,
  SEND_VAL
}States;

typedef struct data{
  float Temp_couvain;  
  float Temp_cote[2];
  float Batterie;    
  float Temp_ambiant;  
  float Humi_couvain;
  float Humi_ambiant;
  float Pression;
  float Poids;
  float Luminosite;
  float EtatAbeilles;  
  int FreqzAbeilles[2];  
  Power_saving_state pwr;
  int _delay;
}data;

/* ----------------------------------------------------
 *  Fonctions liées à Sigfox.cpp
 */
void data_10(data *data);
void Buffer_creation(data data, int *buffer_int_sigfox);
void PrintSigfox(int buffer_int_sigfox[]);
void PrintSerial(data data);


/* ----------------------------------------------------
 *  Fonctions liées à Poids.cpp
 */
void init_HX711();
void get_weight(data *data_weight);

/* ----------------------------------------------------
 *  Fonctions liées à Bluetooth.cpp
 */
void BLEInit();
void BLE_Poll();
void BLE_end();
void setBLEValues();
/* ----------------------------------------------------
 *  Fonctions liées à Temperature.cpp
 */
void init_sensor();
void get_DHT22(data *data_humiditeCouvain);
void get_DS18B20(data *data_tempCote);

/* ----------------------------------------------------
 *  Fonctions liées à GPS.cpp
 */
void testGPS(void);

/* ----------------------------------------------------
 *  Fonctions liées à Batterie.cpp
 */
void init_batterie();
void get_batterie(data *data_batterie);
void sleepcard();
void Ronflex_Lvl_Sleep();
void UNsleepcard();
void LedON();
void LedOFF();

/* ----------------------------------------------------
 *  Fonctions liées à CapteurCarte.cpp
 */
void init_sensor_board();
void get_sensor_board(data *data_pression);
void get_luminosite(data *data_Luminosite);

/* ----------------------------------------------------
 *  Fonctions liées à FFT.cpp
 */
void init_PDM();
void get_Abeilles(data *data_Abeilles);
void unsleepFFT();
#endif

/*classe à implémenter quand pas flemme
 * typedef int sint10 ;

class Hive{
  public:
    Hive(void);
    sint10 getT1(void){return T1;}
    sint10 getT2(void){return T2;}
    sint10 getT3(void){return T3;}
    sint10 getT0(void){return T0;}

  private:
    float T1; // Température milieu
    float T2; // Température coté 
    float T3; // Température coté 
    float T0; // Température à l'intérieur milieu de la ruche
    float H0; // Humidité à l'intérieur de la ruche
    int batterie; // Niveau de la batterie entre 0 et 100 (en %)
    float poids; // poids de la ruche
    int latitude; // coords gps
    int longitude; // coords gps
    int delai_scrutation; // délai de scrutation entre les capteurs par le TPL5110
    // Ajouter variables bluetooth quand il sera fait...
}*/
