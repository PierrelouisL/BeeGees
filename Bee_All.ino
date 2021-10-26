
#define MAXIMWIRE_EXTERNAL_PULLUP
#include <MaximWire.h>
#include <Arduino_HTS221.h>
#include <HX711.h>


#define PIN_BUS 9
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 8 // broche ou l'on a branche le capteur
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);//déclaration du capteur

HX711 scale;

float Offset;
float poids = 65; // Poids de référence


void setup()
{
 Serial.begin(9600);
 Serial1.begin(9600);
 Serial.println("DHTxx test!");
 dht.begin();
 HTS.begin();
 


  scale.begin(D2, D3);
  scale.set_scale(21245.74);
  //scale.tare();
//  Serial.println("Veuillez poser le poids");
//  delay(10000);
//  Serial.println("Je calibre bientôt");
//  delay(10000);
//  Serial.println("Je calibre");
//  Offset = scale.get_units(20);
//  Serial.print("Offset = ");
//  Serial.println(Offset);
//  scale.set_scale(Offset/poids);
//  Serial.print("Offset dans le scale = ");
//  Serial.println(Offset/poids);
}

float h_dht22;
float t_dht22;
 
void GetDHT22data(){ 
  delay(2000);
 // La lecture du capteur prend 250ms
 // Les valeurs lues peuvet etre vieilles de jusqu'a 2 secondes (le capteur est lent)
  h_dht22 = dht.readHumidity();//on lit l'hygrometrie
  t_dht22 = dht.readTemperature();//on lit la temperature en celsius (par defaut)
 
 //On verifie si la lecture a echoue, si oui on quitte la boucle pour recommencer.
 if (isnan(h_dht22) || isnan(t_dht22) )
 {
   Serial.println("Failed to read from DHT sensor!");
   return;
 }
 
 //Affichages :
 Serial.print("Humidite: ");
 Serial.print(h_dht22);
 Serial.print(" %\t");
 Serial.print("Temperature: ");
 Serial.print(t_dht22);
 Serial.println(" °C ");
 
}



MaximWire::Bus bus(PIN_BUS);
MaximWire::DS18B20 device;


float echantillon[3];// echantillon des 3 capteurs float,apres Fct_maximwire2()

void Fct_maximwire2() {
  unsigned short nbr_sensors = 1;
  MaximWire::Discovery discovery = bus.Discover();
  do {
    if (nbr_sensors > 3)break;
    MaximWire::Address address;
    if (discovery.FindNextDevice(address)) {
        Serial.print("FiO==adUND: ");
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
    nbr_sensors++;
    } 
   
  } while (discovery.HaveMore());
  delay(1000);
}

float temperature_carte;
float humidity_carte;

void temp_carte(){
  temperature_carte = HTS.readTemperature()-4;
  humidity_carte    = HTS.readHumidity();
  Serial.print("temperature_carte:");
  Serial.print(temperature_carte);
  Serial.println();
  
  Serial.print("humidity_carte:");
  Serial.print(humidity_carte);
  Serial.println();

  //if(temperature_carte ==0 || humidity_carte == 0){HTS.begin();}
  if (!HTS.begin()) {  // Initialize HTS22 sensor if not
    HTS.begin();
  }
  
}

float poid_rep;

void poid_get(){
  Serial.print("Average:\t");
  poid_rep = scale.get_units(10) -2.79;
  Serial.println(poid_rep, 2);
  
  scale.power_down();              // put the ADC in sleep mode
  delay(3000);
  scale.power_up();
}

void PrintSigfox(){
  char buffer1[96];
  sprintf(buffer1, "AT$SF=%02x%02x%02x%02x%02x%02x%02x\n\r", (int)h_dht22, (int)t_dht22, (int)poid_rep, (int)echantillon[0], (int)echantillon[1], (int)temperature_carte, (int)humidity_carte);
  
  //Serial1.write(buffer1);
  Serial.print("sigufox:\t\n");
  sprintf(buffer1, "=%02x%02x%02x%02x%02x%02x%02x\n\r", (int)h_dht22, (int)t_dht22, (int)poid_rep, (int)echantillon[0], (int)echantillon[1], (int)temperature_carte, (int)humidity_carte);
  Serial.print(buffer1);
  // wait 1 second to print again
  delay(1000);
}

//humidity_out::uint:8 temp_couvain::uint:8 poids::uint:8  temp_cote2::uint:8 temp_cote1::uint:8 temperature_ambiant::uint:8 humidity::uint:8 
void loop() {
  
  delay(2000);
  Fct_maximwire2();
  GetDHT22data();
  temp_carte();
  poid_get();
  PrintSigfox();
  delay(10000);


}
