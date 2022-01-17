#include "bees.h"
#include "mbed.h"

data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};
int Main_state = INIT; 

void setup()
{
  LedON();
  digitalWrite(D2, HIGH);
  delay(2000);
  Serial.begin(9600);
  Serial1.begin(9600);

  // Init all the sensor
  init_sensor_board();
  init_sensor();
  init_HX711();
  init_batterie();
  init_PDM();
  
  while(Serial1.read() != -1){
    Serial1.write("AT\r\n");
  }
  BLEInit();
  delay(2000);
  get_DS18B20(&Alldata);  // Avoid "nan" value
  
  LedOFF();
}

/**
 * Here is the main part of our program, it handles bluetooth Low Energy during the first 5 minutes then disable it 
 * to save energy. It is a fixed state machine which have 2 states: 
 * - INIT: Uses BLE to get sensor value and send them with bluetooth
 * - SEND_VAL: Send values
 */

void loop() {

  //data Alldata = { 0, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, ALL_SENSORS_ON};
  int buffer_int_sigfox[6];
  
  switch(Main_state){
    case INIT:
      // We first enter here here we send data with BLE! 
      // Every sensor is sent to user
      Alldata._delay = 60000; // we stay 5mins in ble mode 
      get_DHT22(&Alldata);
      get_DS18B20(&Alldata);
      get_batterie(&Alldata);
      get_Abeilles(&Alldata);
      get_luminosite(&Alldata);
      if(millis() > Alldata._delay){
        Serial.println("Changement d'état");
        BLE_end();
        Main_state = SEND_VAL;
        Alldata._delay = 595000;
      }
      BLE_Poll();
      break;
    case SEND_VAL:
      switch(Alldata.pwr){
        case ALL_SENSORS_ON:
          Alldata._delay = 60000; // 10 mins de délai
          get_weight(&Alldata);
          get_DHT22(&Alldata);
          get_DS18B20(&Alldata);
          get_batterie(&Alldata);
          get_sensor_board(&Alldata);
          //get_Abeilles(&Alldata);
          get_luminosite(&Alldata);
          break;
        case NO_BOARD_SENSORS:
          Alldata._delay = 120000; // 20 mins délai
          get_weight(&Alldata);
          get_DHT22(&Alldata);
          get_DS18B20(&Alldata);  
          get_batterie(&Alldata);
          break;
        case NO_ANALOG:
          Alldata._delay = 180000; // 30 mins de délai
          get_weight(&Alldata);
          get_DHT22(&Alldata);
          get_DS18B20(&Alldata);
          break;
        case ESSENTIALS:
          Alldata._delay = 3600000; // 1h de délai
          get_weight(&Alldata);
          get_DS18B20(&Alldata);
          break;  
      }
    
      PrintSerial(Alldata);                         // Print sur le pc
      data_10(&Alldata);                              // Adapter les unités des données
      Buffer_creation(Alldata, buffer_int_sigfox);    // Création du buffer pour l'envoie Sigfox
      PrintSigfox(buffer_int_sigfox);                 // Print sur Sigfox  
      
      sleepcard();
      delay(Alldata._delay - 5000);
      UNsleepcard();
      if(ALL_SENSORS_ON){                          // Si mode avec FFT alors on la réactive
        //unsleepFFT();
      }
      delay(5000);
  }
  
}
