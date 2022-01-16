#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>


static const char* ENVIRONMENTAL_SENSING_UUID = "0000181a-0000-1000-8000-00805f9b34fb";   // 181A (16-bit equivalent)
static const char* OPTI_CHARACTERISTIC_UUID = "00002a6e-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* TMP_COUVAIN_CHARACTERISTIC_UUID = "00002a6f-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)
static const char* TMP_1_CHARACTERISTIC_UUID = "00002a70-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* TMP_2_CHARACTERISTIC_UUID = "00002a71-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)
static const char* BAT_CHARACTERISTIC_UUID = "00002a72-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* HUM_COUVAIN_CHARACTERISTIC_UUID = "00002a73-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)
static const char* HUM_AMBIANT_CHARACTERISTIC_UUID = "00002a74-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* PRESSION_CHARACTERISTIC_UUID = "00002a75-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)
static const char* POIDS_CHARACTERISTIC_UUID = "00002a76-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* Freq_CHARACTERISTIC_UUID = "00002a77-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)

// Sensing values
BLEService environmentalSensingService(ENVIRONMENTAL_SENSING_UUID);
BLEByteCharacteristic optiStatusCharacteristic(OPTI_CHARACTERISTIC_UUID, BLERead | BLEWrite);
BLECharacteristic tmpCouvCharacteristic(TMP_COUVAIN_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic tmp1Characteristic(TMP_1_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic tmp2Characteristic(TMP_2_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic batCharacteristic(BAT_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic humCouvCharacteristic(HUM_COUVAIN_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic humAmbCharacteristic(HUM_AMBIANT_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic pressionCharacteristic(PRESSION_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic poidsCharacteristic(POIDS_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);
BLECharacteristic freqCharacteristic(Freq_CHARACTERISTIC_UUID, BLERead | BLEWrite, 20);


typedef struct data{
  float Temp_couvain;  
  float Temp_cote[2];
  float Batterie;    
  float Temp_ambiant;  
  float Humi_couvain;
  float Humi_ambiant;
  float Pression;
  float Poids;
  int FreqzAbeilles;  
}data;

data Alldata = { 19, {20, 21}, 50, 45, 46, 1000, 45, 345};

void setup() {
  // Initialize Serial with Baud-Rate
  Serial.begin(9600);

  // Without Serial when using USB power bank HTS sensor seems to needs some time for setup
  delay( 10 );
  
  // Initialize LED
  initializeLed();

  // Initialize temperature sensor (in Arduino)
  if (!HTS.begin()) {
    Serial.println("Failed to start temperature Sensor");
    while(1);
  }

  // Initialize BLE Stack (in Arduino)
  if (!BLE.begin()) {
    Serial.println("Failed to start BLE");
    while(1);
  }

  // Assigning Event Handlers (eventHandler: Sets the callback function to be called in case of match with the chosen event)
  BLE.setEventHandler(BLEConnected, eventHandlerBleConnected);
  BLE.setEventHandler(BLEDisconnected, eventHandlerBleDisconnected);
  optiStatusCharacteristic.setEventHandler(BLEWritten, onOptiCharacteristicWritten);

  
  // Setting the Peripheral device name
  BLE.setLocalName("BHE");

  BLE.setAdvertisedService(environmentalSensingService);
  environmentalSensingService.addCharacteristic(optiStatusCharacteristic);
  environmentalSensingService.addCharacteristic(tmpCouvCharacteristic);
  environmentalSensingService.addCharacteristic(tmp1Characteristic);
  environmentalSensingService.addCharacteristic(tmp2Characteristic);
  environmentalSensingService.addCharacteristic(batCharacteristic);
  environmentalSensingService.addCharacteristic(humCouvCharacteristic);
  environmentalSensingService.addCharacteristic(humAmbCharacteristic);
  environmentalSensingService.addCharacteristic(pressionCharacteristic);
  environmentalSensingService.addCharacteristic(poidsCharacteristic);
  environmentalSensingService.addCharacteristic(freqCharacteristic);
  BLE.addService(environmentalSensingService);
  setBLEValues();
  
  // Start 'Advertisement'
  BLE.advertise();
  Serial.println("BLE device waiting for central connections..");
}


void setBLEValues(){
  char SsidLocal[32];
  // Envoi température couvain
  //sprintf(SsidLocal,"TMP Couv = %.2f°C", data.Temp_couvain);
  //tmpCouvCharacteristic.setValue((unsigned char *)SsidLocal,16);

  // Envoi température DS18B20 n1
  sprintf(SsidLocal,"TMP n1 = %.2f°C", Alldata.Temp_cote[1]);
  tmp1Characteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi température DS18B20 n2
  sprintf(SsidLocal,"TMP n2 = %.2f°C", Alldata.Temp_cote[2]);
  tmp2Characteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi niv bat
  sprintf(SsidLocal,"Niv bat = %.2f %", Alldata.Batterie);
  batCharacteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi humidité couvain
  sprintf(SsidLocal,"Hum Couv = %.2f %", Alldata.Humi_couvain);
  humCouvCharacteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi humidité ambiante
  sprintf(SsidLocal,"Hum amb = %.2f %", Alldata.Humi_ambiant);
  humAmbCharacteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi pression
  sprintf(SsidLocal,"Pression = %.2fPa", Alldata.Pression);
  pressionCharacteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi poids
  sprintf(SsidLocal,"poids = %.2fkg", Alldata.Poids);
  poidsCharacteristic.setValue((unsigned char *)SsidLocal,20);

  // Envoi freq
  sprintf(SsidLocal,"Freq = %.2f Hz", (float)Alldata.FreqzAbeilles);
  freqCharacteristic.setValue((unsigned char *)SsidLocal,20);
}

void loop() {
  static long previousMillis = 0;
  long interval = 1000;
  long currentMillis = millis();
  char SsidLocal[32];
  
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    // BLE defines Temperature UUID 2A6E Type sint16
    // Unit is in degrees Celsius with a resolution of 0.01 degrees Celsius
    float temperature = HTS.readTemperature();
    Serial.println(temperature);

    sprintf(SsidLocal,"T0 = %.2f°C", temperature);
    tmpCouvCharacteristic.setValue((unsigned char*)SsidLocal, 20);
    Serial.println(SsidLocal);
  }

  // Poll BLE Events
  BLE.poll();
}


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

// Update LED status (ON/OFF) upon receiving appropriate writeCharacteristic event from central
void onOptiCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("Central wrote new LED Characteristic event");
  Serial.print("Value = ");
  Serial.print(optiStatusCharacteristic.value());
  Serial.print("\n");
}
