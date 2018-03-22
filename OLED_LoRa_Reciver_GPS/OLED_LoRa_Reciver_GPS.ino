/*
  This is a simple example show the LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306.h"
#include "definitions.h" 
#include "images.h"
#include "gps_utils.h"

// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true

//SSD1306 display(0x3c, 4, 15);
String rssi = "RSSI --";
String packSize = "--";
String packet ;


void displaymsg(String msg,String msg2){
 display.clear();
   display.drawString(0, 0, msg);
  display.drawString(0, 10, msg2);
  display.display();
  delay(200);
}

#ifdef BLE
#include "ble.h"
#endif

void logo(){
  display.clear();
  display.drawXbm(0,5,logo_width,logo_height,logo_bits);
  display.display();
}

String bytestoString(byte *array,int len) {
   String output ="";
   for (int x=0;x<len;x++) {
   output.concat( array[x]);
   output.concat(":");
   }
   return output;
  
}
void showOnLED(byte *latlonbytes,int len) {

 // ssss

 //float* loraLocation = stringToLatLon(latlonbytes,len);
}


void drawCompass(float distance, float bearing){

 delay(1000);
 display.clear();
 String output = "Dist: ";
 output.concat(String(distance,1));
 output.concat("M Bearing: ");
 output.concat(String(bearing,0));
 
 display.drawString(0, 0, output);
 display.display(); 
 //display.drawLine(0,0,DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2);

 int cx=DISPLAY_WIDTH/2;
 int cy=DISPLAY_HEIGHT/2;
//for (int x=270;x<360;x++){
//  triangleAngle(cx,cy,50,50,x);
//  delay(50);
//}
 int w=50;
 int h=50;
 if (bearing < 45 || bearing > 315) w=30;
 if (bearing > 45 && bearing < 135) h=30;
 if (bearing > 135 && bearing < 225) w=30;
 if (bearing > 225 && bearing < 315) h=30;
 triangleAngle( cx,cy,w,h,bearing);

//  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=2) {
//    display.drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, i);
//    display.display();
//    delay(10);
//  }
//  

 //display.drawVerticalLine(180,100,150);
  

 //display.display();
 
}
void sendbuzzer() {

      for (int x=0;x<10;x++) {
       digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
       delay (100);
       digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
       delay(50);
      }
      

  LoRa.beginPacket();
  //LoRa.print(outputLat);
  byte msg[] = {1,1,5};
  LoRa.write(msg,3);
  //LoRa.print(counter);
  LoRa.endPacket();
  delay(1000);
  
}
void cbk(int packetSize) {
  //packet =""
  packetcounter++;
  packSize = String(packetSize,DEC);
  byte packets[packetSize];
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  for (int i = 0; i < packetSize; i++) { packets[i] = (byte) LoRa.read(); }
  //delay(500);                       // wait for a second

  curtime = millis();
  
  if (lasttime == 0) {
    lasttime = curtime;
  }
  
  
//
//  float lat;
//  float lon;
//  float deviceid;
//
//deviceid=packets[0];
//ulat.b[0] = packets[1];
//ulat.b[1] = packets[2];
//ulat.b[2] = packets[3];
//ulat.b[3] = packets[4];
//ulon.b[0] = packets[5];
//ulon.b[1] = packets[6];
//ulon.b[2] = packets[7];
//ulon.b[3] = packets[8];
//
//lat = ulat.fval;
//lon = ulon.fval;
//float latlon[] = {deviceid,lat,lon}; 

bytesToLatLon(currentLatLon,packets,packetSize);
if (prevLatLon[0] == 0){
  prevLatLon[0] = currentLatLon[0];
  prevLatLon[1] = currentLatLon[1];
  prevLatLon[2] = currentLatLon[2];
}


getMyLocation(myLatLon);

String outputLat = "Lat: ";
 outputLat.concat(String(currentLatLon[1],6));

 String outputLon = "Lon: ";
 outputLon.concat(String(currentLatLon[2],6));
 String deviceID = "Device: "; 
 deviceID.concat(String(currentLatLon[0],1));

 
  float ddistance = calc_dist(prevLatLon[1],prevLatLon[2],currentLatLon[1],currentLatLon[2]);
  String dDD = "Spd: ";
  float dspeed = ddistance / ((curtime - lasttime) / 1000);
  dDD.concat(String(dspeed));
  float dbearing = calc_bearing(prevLatLon[1],prevLatLon[2],currentLatLon[1],currentLatLon[2]);
  if (dbearing < 0){
    dbearing +=360;
  }
  dDD.concat(" Brn: ");
  dDD.concat(String(dbearing));



  float mdistance = calc_dist(myLatLon[1],myLatLon[2],currentLatLon[1],currentLatLon[2]);
  String mDD = "Dst: ";
  mDD.concat(String(mdistance));
  float mbearing = calc_bearing(myLatLon[1],myLatLon[2],currentLatLon[1],currentLatLon[2]);
  if (mbearing < 0){
    mbearing +=360;
  }
  mDD.concat(" Dir: ");
  mDD.concat(String(mbearing));
 
  
  
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  rssi.concat( "  ");
  rssi.concat(packSize);
  rssi.concat(" bytes");
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
 // display.drawString(0 , 15 , "Received "+ packSize + " bytes");
 // display.drawStringMaxWidth(0 , 26 , 128,bytestoString(packets,packetSize));
 
  display.drawString(0, 0, rssi);  
  display.drawString(0,10,outputLat);
  display.drawString(0,20,outputLon);
  display.drawString(0,30,deviceID);
  display.drawString(0,40,mDD);
  display.drawString(0,50,dDD);
 
  display.display();
#ifdef COMPASS
 drawCompass(mdistance,mbearing);
#endif

  if (deviceConnected){
    sendBLEbytes(packets,packetSize);
  }
  prevLatLon[0] = currentLatLon[0];
  prevLatLon[1] = currentLatLon[1];
  prevLatLon[2] = currentLatLon[2];
  lasttime = curtime;
 digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
 if (packetcounter > 2) {
  packetcounter=0;
  sendbuzzer();
 }
}


void setup() {
    pinMode(25,OUTPUT);
   
  setupBLE();
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、
  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  display.clear();
   
   
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI00);
  

  
  if (!LoRa.begin(BAND,PABOOST)) {
    display.clear();
    display.drawString(0, 0, "Starting LoRa failed!");
    display.display();
    while (1);
  }
  display.clear();
  display.drawString(0, 0, "LoRa Initial success!");
  display.drawString(0, 10, "Wait for incomm data...");
  display.display();
  delay(1000);
 // LoRa.onReceive(cbk);
  LoRa.receive();

  sendbuzzer();
   
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  // sendBLE();
  
  //delay(2000);
}
