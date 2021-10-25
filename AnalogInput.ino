/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/

int sensorPin = A1;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  float voltage= analogRead(sensorPin) * (3.3 / 1023.0);
  Serial.println(voltage);
  delay(1000);
  if(voltage > 2.92){
    // Battery over 90%
    Serial.println("Battery over 90%");
  }else if(voltage > 2.8){
    //Battery between 60%-90%
    Serial.println("Battery between 60%-90%");
  }else if(voltage > 2.73){
    //Battery between 30%-60%
    Serial.println("Battery between 30%-60%");
  }else if(voltage > 2.66){
    //Battery between 10%-30%
    Serial.println("Battery between 10%-30%");
  }else{
    //Battery under 10%
    Serial.println("Battery under 10%");
  }
}
