/*void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }

  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte);
  }
}
*/

#include <Arduino_HTS221.h>

void setup() {
  Serial1.begin(9600);
  //while (!Serial);
  delay(100);
  HTS.begin();
}

void loop() {
  // read all the sensor values
  float temperature = HTS.readTemperature();
  float humidity    = HTS.readHumidity();

  // print on sigfox
  char buffer1[50];
  sprintf(buffer1, "AT$SF=%02x\n\r",(int)temperature);
  Serial1.write(buffer1);

  // wait 1 second to print again
  delay(5000);
}
  // print on sigfox
  //Serial.write("AT$SF=%02x\r\n", (int)temperature);
