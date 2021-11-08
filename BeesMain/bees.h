#ifndef __BEES__
#define __BEES__

// ----------------------------------------------------
//  Variables globales et librairies 

#include <Arduino_HTS221.h>

// temperature.cpp global value
  // DHT Sensor
  #define DHTPIN 8 // broche ou l'on a branche le capteur
  #define DHTTYPE DHT22 // DHT 22 (AM2302)
  // DS18B20 Sensor
  #define PIN_BUS 9
  #define MAXIMWIRE_EXTERNAL_PULLUP

// Sigfox.cpp global value
#define NB_value 8
#define NB_bits 96
// ----------------------------------------------------

typedef struct data{
  float Temp_couvain;  
  float Temp_cote[2];    
  float Temp_ambiant;  
  float Poids;         
  float Batterie;
  float Humi_couvain;
  float Humi_ambiant;
}data;

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

/* ----------------------------------------------------
 *  Fonctions liées à Sigfox.cpp
 */

void data_10(data *data);
void Buffer_creation(data data, int *buffer_int_sigfox);
void PrintSigfox(data data, int buffer_int_sigfox[]);


/* ----------------------------------------------------
 *  Fonctions liées à Poids.cpp
 */
void init_HX711();
void get_weight(data *data_weight);

/* ----------------------------------------------------
 *  Fonctions liées à Bluetooth.cpp
 */
void testBluetooth(void);

/* ----------------------------------------------------
 *  Fonctions liées à temperature.cpp
 */

void init_temp_humi_board();
void get_DHT22(data *data_humiditeCouvain);
void get_DS18B20(data *data_tempCote);
void get_temp_humi_board(data *data_tempAmbian);

/* ----------------------------------------------------
 *  Fonctions liées à GPS.cpp
 */
void testGPS(void);

/* ----------------------------------------------------
 *  Fonctions liées à Batterie.cpp
 */
void init_batterie();
void get_batterie(data *data_batterie);

#endif
