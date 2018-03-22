#define BAND    868100000  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST false
#define TXPOWER 14
#define SPREADING_FACTOR 12
#define BANDWIDTH 125000
#define CODING_RATE 5
#define PREAMBLE_LENGTH 8
#define SYNC_WORD 0x34
void configForLoRaWAN()
{
  LoRa.setTxPower(TXPOWER);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.setPreambleLength(PREAMBLE_LENGTH);
  LoRa.setSyncWord(SYNC_WORD);
  LoRa.crc();
}

tring loraCfg = "Cfg:";
void  displayLoRaConfig(int x, int y){
  loraCfg = 
            "fr " + String(BAND/1000000, DEC)
          + " sf" + String(SPREADING_FACTOR, DEC)
          + " bw" + String(BANDWIDTH/1000, DEC)
          + " cr" + String(CODING_RATE, DEC) + "/4";
  display.drawString(x, y, loraCfg);
  loraCfg =        
          + " pr" + String(PREAMBLE_LENGTH, DEC)
          + " pw" + String(TXPOWER, DEC)
          + " sw" + String(SYNC_WORD, HEX)
          ;
  display.drawString(x, y+10, loraCfg);
}

 if (!LoRa.begin(BAND,PABOOST)) {
    display.drawString(0, 0, "Starting LoRa failed!");
    display.display();
    while (1);
  }
  displayLoRaConfig(0,20);
  display.drawString(0, 0, "LoRa Initial success!");

...

  display.display();
...

