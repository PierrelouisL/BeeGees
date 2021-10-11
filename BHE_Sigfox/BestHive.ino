#include <Arduino_HTS221.h>

float Temp_couvain = 0;
float Temp_cote1 = 0;
float Temp_cote2 = 0;
float Temp_ambiant = 0;
float Poids = 0;
float Batterie = 0;
float Humidity = 0;

void PrintSigfox(){
  char buffer1[50];
  sprintf(buffer1, "AT$SF=%02x%02x%02x%02x%02x%02x%02x\n\r", (int)Humidity, (int)Batterie, (int)Poids, (int)Temp_ambiant, (int)Temp_cote2, (int)Temp_cote1, (int)Temp_couvain);
  
  Serial1.write(buffer1);
  
  // wait 1 second to print again
  delay(5000);
}

void setup() {
  Serial1.begin(9600);
  //while (!Serial);
  delay(100);
  //HTS.begin();
}

void loop(){
  
  // read all the sensor values
  temperature = HTS.readTemperature();
  humidity    = HTS.readHumidity();

  // print on sigfox
  PrintSigfox();
}
