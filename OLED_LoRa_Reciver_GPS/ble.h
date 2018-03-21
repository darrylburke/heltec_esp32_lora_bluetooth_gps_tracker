#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;
#define BLEName "myESP32db"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID           "6e400001-b5a3-f393-e0a9-e50e24dcca9E" 
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9E"
#define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9E"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setupBLE() {
 
 // displaymsg("Setup BLE:",BLEName);
  // Create the BLE Device
  BLEDevice::init(BLEName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
//  displaymsg("Waiting a client connection to notify...","");
}

void sendBLEbytes(byte *latlonbytes,int len) {
     
      

//
//       int latloc = msg.indexOf("Lat");
//      latloc+=5;
//      int latlocend = msg.indexOf(",",latloc);
//      
//      int lonloc = msg.indexOf("Lon");
//      lonloc+=5;
//      int lonlocend = msg.indexOf("}",lonloc);
//      
//      String _lat = msg.substring(latloc,latlocend);
//      String _lon = msg.substring(latloc,latlocend);
//      
//     // displaymsg("LAT",_lat);
//     //delay(300);
//     // displaymsg("LON",_lon);
//      
//      int msg_len = msg.length();
//      byte msgbytes[msg_len];
//      msg.getBytes(msgbytes,msg_len);


     // displaymsg("*** NOTIFY:",String(value));
      pCharacteristic->setValue(latlonbytes,len);
     // pCharacteristic->setValue(&value, 1);
      pCharacteristic->notify();
      //pCharacteristic->indicate();
      value++;
  

}

void sendBLEString(String msg) {
     
      
    if (deviceConnected) {

       int latloc = msg.indexOf("Lat");
      latloc+=5;
      int latlocend = msg.indexOf(",",latloc);
      
      int lonloc = msg.indexOf("Lon");
      lonloc+=5;
      int lonlocend = msg.indexOf("}",lonloc);
      
      String _lat = msg.substring(latloc,latlocend);
      String _lon = msg.substring(latloc,latlocend);
      
     // displaymsg("LAT",_lat);
     //delay(300);
     // displaymsg("LON",_lon);
      
      int msg_len = msg.length();
      byte msgbytes[msg_len];
      msg.getBytes(msgbytes,msg_len);


     // displaymsg("*** NOTIFY:",String(value));
      pCharacteristic->setValue(msgbytes,msg_len);
     // pCharacteristic->setValue(&value, 1);
      pCharacteristic->notify();
      //pCharacteristic->indicate();
      value++;
  }

}
void sendBLE() {

  if (deviceConnected) {
    displaymsg("*** NOTIFY:",String(value));
    pCharacteristic->setValue(&value, 1);
    pCharacteristic->notify();
    //pCharacteristic->indicate();
    value++;
  }

}
