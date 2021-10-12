
#define MAXIMWIRE_EXTERNAL_PULLUP
#include <MaximWire.h>



#define PIN_BUS 9
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 8 // broche ou l'on a branche le capteur
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);//déclaration du capteur
 
void setup()
{
 Serial.begin(9600);
 Serial.println("DHTxx test!");
 dht.begin();
}

void GetDHT22data(){
  delay(2000);
 
 // La lecture du capteur prend 250ms
 // Les valeurs lues peuvet etre vieilles de jusqu'a 2 secondes (le capteur est lent)
 float h = dht.readHumidity();//on lit l'hygrometrie
 float t = dht.readTemperature();//on lit la temperature en celsius (par defaut)
 
 //On verifie si la lecture a echoue, si oui on quitte la boucle pour recommencer.
 if (isnan(h) || isnan(t) )
 {
   Serial.println("Failed to read from DHT sensor!");
   return;
 }
 
 //Affichages :
 Serial.print("Humidite: ");
 Serial.print(h);
 Serial.print(" %\t");
 Serial.print("Temperature: ");
 Serial.print(t);
 Serial.println(" °C ");
 
}



MaximWire::Bus bus(PIN_BUS);
MaximWire::DS18B20 device;


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
  GetDHT22data();

  /*//print les valeur du tableau de capteur float
    for (int i=0;i<3;i++){
    Serial.print(echantillon[i]);
    Serial.print("=temp");
    Serial.print(i);
    Serial.println();
    }*/

}