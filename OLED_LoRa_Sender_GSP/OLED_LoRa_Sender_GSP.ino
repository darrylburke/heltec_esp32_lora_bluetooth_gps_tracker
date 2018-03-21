/*
  This is a simple example show the LoRa sended data in OLED.

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
#include "images.h"
#include "gps.h"

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

#define LORAID 1

unsigned int counter = 0;

SSD1306 display(0x3c, 4, 15);
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo()
{
  display.clear();
  display.drawXbm(0,5,logo_width,logo_height,logo_bits);
  display.display();
}

void setup()
{
  pinMode(16,OUTPUT);
  pinMode(25,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  display.clear();
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI00);
  
  if (!LoRa.begin(BAND,PABOOST))
  {
    display.drawString(0, 0, "Starting LoRa failed!");
    display.display();
    while (1);
  }
  display.drawString(0, 0, "LoRa Initial success!");
  display.display();
  delay(1000);
  initGPS();
}

String bytestoString(byte *array,int len) {
   String output ="";
   for (int x=0;x<len;x++) {
   output.concat( array[x]);
   output.concat(":");
   }
   return output;
  
}
void loop()
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);


  
  // send packet
 latlon position = getcurrentPosition();
 String outputLat = "Lat: ";
 outputLat.concat(String(position.lat,6));

 String outputLon = "Lon: ";
 outputLon.concat(String(position.lon,6));
 
 String deviceID = "Device ID:";
 deviceID.concat(String(LORAID));
  
  display.drawString(0, 0, "Sending packet:[");
  display.drawString(80, 0, String(counter));
  display.drawString(99, 0, "]");
  
  display.drawString(0,10,outputLat);
 


byte *_lat = (byte *)&position.lat;
byte *_lon = (byte *)&position.lon;
byte msg[9];
msg[0] = LORAID;
for (int x=0;x<4;x++){
  msg[1+x]=_lat[x];
  msg[1+x+4]=_lon[x];
}

  display.drawString(0,20,outputLon);
 // display.drawString(0,30,bytestoString(msg,9));
  display.drawString(0,30,deviceID);
  display.display();
  LoRa.beginPacket();
  //LoRa.print(outputLat);
  LoRa.write(msg,9);
  //LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  delay(5000);
}
