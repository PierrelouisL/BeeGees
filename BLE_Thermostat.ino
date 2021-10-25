#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>

const int ledPin = LED_BUILTIN;

static const char* MANUFACTURER_NAME = "Arduino";
static const char* MODEL_NUMBER = "Nano 33 BLE Sense";

static const char* LED_SERVICE_UUID = "277eaf48-6698-4da9-8329-335d05343490";
static const char* LED_CHARACTERISTIC_UUID = "277eaf49-6698-4da9-8329-335d05343490";

static const char* DEVICE_INFORMATION_UUID = "0000180a-0000-1000-8000-00805f9b34fb";  // 180A (16-bit equivalent)
static const char* MANUFACTURER_NAME_UUID = "00002a29-0000-1000-8000-00805f9b34fb";   // 2A29 (16-bit equivalent)
static const char* MODEL_NUMBER_UUID = "00002a24-0000-1000-8000-00805f9b34fb";        // 2A24 (16-bit equivalent)

static const char* ENVIRONMENTAL_SENSING_UUID = "0000181a-0000-1000-8000-00805f9b34fb";   // 181A (16-bit equivalent)
static const char* TEMPERATURE_UUID = "00002a6e-0000-1000-8000-00805f9b34fb";             // 2A6E (16-bit equivalent)
static const char* HUMIDITY_UUID = "00002a6f-0000-1000-8000-00805f9b34fb";                // 2A6F (16-bit equivalent)

// Creating LED Service
BLEService ledService(LED_SERVICE_UUID);
BLEByteCharacteristic ledStatusCharacteristic(LED_CHARACTERISTIC_UUID, BLERead | BLEWrite);

// Creating Device-Info Service
BLEService deviceInformationService(DEVICE_INFORMATION_UUID);
BLEStringCharacteristic manufacturerNameCharacteristic(MANUFACTURER_NAME_UUID, BLERead, 10); 
BLEStringCharacteristic modelNumberCharacteristic(MODEL_NUMBER_UUID, BLERead, 20);

// Creating Environmental-Sensing Service
BLEService environmentalSensingService(ENVIRONMENTAL_SENSING_UUID);
BLEShortCharacteristic temperatureCharacteristic(TEMPERATURE_UUID, BLERead | BLENotify);
//BLEUnsignedShortCharacteristic humidityCharacteristic(HUMIDITY_UUID, BLERead | BLENotify);
BLECharacteristic humidityCharacteristic(HUMIDITY_UUID, BLERead | BLEWrite, 16);

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
  ledStatusCharacteristic.setEventHandler(BLEWritten, onLedCharacteristicWritten);

  // Setting the Peripheral device name
  BLE.setLocalName("Smart Thermostat");

  // Set the UUID for the service Advertised by the peripheral (in our case, environmental sensing)
  BLE.setAdvertisedService(environmentalSensingService);

  // Adding characteristics to Environmental-Sensing Service
  environmentalSensingService.addCharacteristic(temperatureCharacteristic);
  environmentalSensingService.addCharacteristic(humidityCharacteristic);
  // Adding the Environmental-Sensing Service to peripheral
  BLE.addService(environmentalSensingService);

  // Adding characteristics to LED Service
  ledService.addCharacteristic(ledStatusCharacteristic);
  // Adding the LED Service to peripheral
  BLE.addService(ledService);

  // Adding characteristics to Device-Info Service
  deviceInformationService.addCharacteristic(manufacturerNameCharacteristic);
  deviceInformationService.addCharacteristic(modelNumberCharacteristic);
  // Adding the Device-Info Service to peripheral
  BLE.addService(deviceInformationService);

  // Initialize values for Read-Only Characterisitics (i.e Device-Info Service)
  manufacturerNameCharacteristic.setValue(MANUFACTURER_NAME);
  modelNumberCharacteristic.setValue(MODEL_NUMBER);

  // Start 'Advertisement'
  BLE.advertise();
  Serial.println("BLE device waiting for central connections..");
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
    temperatureCharacteristic.setValue((int)temperature);
    Serial.println(temperature);

    // BLE defines Humidity UUID 2A6F Type uint16
    // Unit is in percent with a resolution of 0.01 percent
    //uint16_t humidity = ;
    sprintf(SsidLocal,"T0 = %.2f°C", temperature);
    humidityCharacteristic.setValue((unsigned char *)SsidLocal,16) ;
    //humidityCharacteristic.writeValue(humidity);
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
void onLedCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("Central wrote new LED Characteristic event");

  if (ledStatusCharacteristic.value()) {
    // Turn LED ON
    Serial.println("Turn LED ON");
    turnLedOn();
  } else {
    // Turn LED OFF
    Serial.println("Turn LED OFF");
    turnLedOff();
  }
}

// LED events
void initializeLed() {
  pinMode(ledPin, OUTPUT);
  turnLedOff();
}

void turnLedOn() {
  digitalWrite(ledPin, HIGH);
}

void turnLedOff() {
  digitalWrite(ledPin, LOW);
}
