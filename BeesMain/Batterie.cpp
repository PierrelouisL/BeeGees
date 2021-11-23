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
void UNsleepcard(){
  digitalWrite(PIN_ENABLE_SENSORS_3V3, HIGH); // turn on sensors
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);
}
