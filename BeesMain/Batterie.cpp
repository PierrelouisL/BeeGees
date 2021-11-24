#include "bees.h"

int Pin_Batterie = A2;    // Pin batterie
int ledPin = 13;          // select the pin for the LED

void init_batterie() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_PWR, LOW);
}

void get_batterie(data *data_batterie) {
  float voltage= analogRead(Pin_Batterie) * (3.3 / 1023.0) * 100;
  if(voltage < 257.65){
    voltage = 258;
  }
  data_batterie->Batterie = (int)voltage;
}

void sleepcard(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW);  // turn off sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);
}

void Ronflex_Lvl_Sleep(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW);  // turn off sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);

  //------- DISABLE TWIM (I2C) -------//
  // A tester peut etre ça fck up les capteurs
  NRF_TWIM0->TASKS_STOP = 1; 
  NRF_TWIM0->ENABLE = 0;
  NRF_TWIM1->TASKS_STOP = 1;
  NRF_TWIM1->ENABLE = 0;


  //------- DISABLE UART --------//
  NRF_UART0->TASKS_STOPTX = 1;
  NRF_UART0->TASKS_STOPRX = 1;
  NRF_UART0->ENABLE = 0;

  //------- DISABLE CRYPTOCELL ---------//
  NRF_CRYPTOCELL->ENABLE = 0;

  //------- DISABLE RADIO (BLE) p308 ------//
  NRF_RADIO->POWER = 0;

  //------ DISABLE SPI ---------//
  NRF_SPI0->ENABLE = 0;
  NRF_SPI1->ENABLE = 0;
  NRF_SPI2->ENABLE = 0;

  //----- DISABLE Analog to Digital -----//
  NRF_SAADC->ENABLE = 0;

}

void UNsleepcard(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, HIGH); // turn on sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);
  /*NRF_TWIM0->TASKS_STOP = 0;
  NRF_TWIM0->ENABLE = 1;
  NRF_TWIM1->TASKS_STOP = 0;
  NRF_TWIM1->ENABLE = 1;*/
}
