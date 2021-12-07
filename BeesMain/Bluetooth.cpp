#include "bees.h"
/*#include <ArduinoBLE.h>

static const char* OPTI_SERVICE_UUID = "277eaf48-6698-4da9-8329-335d05343490";
static const char* OPTI_CHARACTERISTIC_UUID = "277eaf49-6698-4da9-8329-335d05343490";


// Creating OPTI Service
BLEService optiService("181A");
BLEByteCharacteristic optiStatusCharacteristic(OPTI_CHARACTERISTIC_UUID, BLERead | BLEWrite);

// Event-Handler when BLE Peripheral (within Arduino) is connected to Central
void eventHandlerBleConnected(BLEDevice central) {
  Serial.print("Connected to central with address: ");
  Serial.println(central.address());
}

// Event-Handler when BLE Peripheral (within Arduino) is dis-connected from Central
void eventHandlerBleDisconnected(BLEDevice central) {
  Serial.print("Disconnected from central with address: ");
  Serial.println(central.address());
}

void onoptiCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println(characteristic.value());
}

void BLEInit(){
  BLE.begin();

  // Setting the Peripheral device name
  BLE.setLocalName("Best Hive Ever");
  BLE.setAdvertisedService(optiService);
  optiService.addCharacteristic(optiStatusCharacteristic);

  BLE.addService(optiService);
  BLE.setConnectable(true);
  BLE.advertise();
  

  // Assigning Event Handlers (eventHandler: Sets the callback function to be calopti in case of match with the chosen event)
  BLE.setEventHandler(BLEConnected, eventHandlerBleConnected);
  BLE.setEventHandler(BLEDisconnected, eventHandlerBleDisconnected);
  optiStatusCharacteristic.setEventHandler(BLEWritten, onoptiCharacteristicWritten);
}


void testBluetooth(){
  Serial.println("Hello B!");
  BLEDevice central = BLE.central();
  if(central){
    Serial.println("connected");
  }
}*/
