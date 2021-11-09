#include "bees.h"

int sensorPin = A1;    // select the input pin for the potentiometer
int ledPin = 13;       // select the pin for the LED

void init_batterie() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_PWR, LOW);
}

void get_batterie(data *data_batterie) {
  float voltage= analogRead(sensorPin) * (3.3 / 1023.0);
  data_batterie->Batterie = (int) voltage*100;
  /*if(voltage > 2.92){         // Battery over 90%
    data_batterie->Batterie = 90;
  }else if(voltage > 2.8){    //Battery between 60%-90%
    data_batterie->Batterie = 60;
  }else if(voltage > 2.73){   //Battery between 30%-60%
    data_batterie->Batterie = 30;
  }else if(voltage > 2.66){   //Battery between 10%-30%
    data_batterie->Batterie = 10;
  }else{                      //Battery under 10%
    data_batterie->Batterie = 1;
  }*/
}
