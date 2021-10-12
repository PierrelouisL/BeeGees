// MaximWire - Version: Latest

#define MAXIMWIRE_EXTERNAL_PULLUP

#include <MaximWire.h>

#define PIN_BUS 9

MaximWire::Bus bus(PIN_BUS);
MaximWire::DS18B20 device;

void setup() {
  Serial.begin(9600);
}

float echantillon[3];// echantillon des 3 capteurs float,apres Fct_maximwire2()


void Fct_maximwire2() {
  unsigned short nbr_sensors = 1;
  MaximWire::Discovery discovery = bus.Discover();
  do {
    MaximWire::Address address;
    if (discovery.FindNextDevice(address)) {
      Serial.print("FOUND: ");
      Serial.print(address.ToString());
      if (address.IsValid()) {
        Serial.print(" (VALID)");
      } else {
        Serial.print(" (INVALID)");
      }
      if (address.GetModelCode() == MaximWire::DS18B20::MODEL_CODE) {
        Serial.print(" (DS18B20)");
        MaximWire::DS18B20 device(address);
        if (device.IsParasitePowered(bus)) {
          Serial.print(" (PARASITE POWER)");
        }
        float temp = device.GetTemperature<float>(bus);
        echantillon[nbr_sensors - 1] = temp;
        Serial.print(" temp");
        Serial.print(nbr_sensors);
        Serial.print("=");
        Serial.print(temp);
        Serial.println();
        device.Update(bus);
      } else {
        Serial.println();
      }
    } else {
      Serial.println("NOTHING FOUND");
    }
    nbr_sensors++;
  } while (discovery.HaveMore());
  delay(1000);


}


void loop() {

  Fct_maximwire2();

  /*//print les valeur du tableau de capteur float
    for (int i=0;i<3;i++){
    Serial.print(echantillon[i]);
    Serial.print("=temp");
    Serial.print(i);
    Serial.println();
    }*/

}
