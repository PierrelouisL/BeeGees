#include "bees.h"

#include <MaximWire.h>
#include <DHT.h>
#include <DHT_U.h>

MaximWire::Bus bus(PIN_BUS);
MaximWire::DS18B20 device;
  
DHT dht(DHTPIN, DHTTYPE);

void init_sensor(){
    dht.begin(); 
}

void get_DS18B20(data *data_tempCote){
  unsigned short nbr_sensors = 1;
  float temp;
  
  MaximWire::Discovery discovery = bus.Discover();
  do{
    if (nbr_sensors > 2){
      break;
    }
    MaximWire::Address address;
    if (discovery.FindNextDevice(address)){
        if (address.GetModelCode() == MaximWire::DS18B20::MODEL_CODE){
          MaximWire::DS18B20 device(address);
          temp = device.GetTemperature<float>(bus);
          data_tempCote->Temp_cote[nbr_sensors - 1] = temp;
          device.Update(bus);
        }
    nbr_sensors++;
    } 
  } while (discovery.HaveMore());
}

void get_DHT22(data *data_DHT){
  data_DHT->Temp_couvain = dht.readTemperature();  
  data_DHT->Humi_couvain = dht.readHumidity(); 
}
