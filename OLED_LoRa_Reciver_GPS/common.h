
union lat_tag {
   byte b[4];
   float fval;
} ulat;

union lon_tag {
   byte b[4];
   float fval;
} ulon;

float currentLatLon[] = {0,0,0,0};
float prevLatLon[] ={0,0,0,0};
float myLatLon[] ={0,0,0,0};
unsigned long lasttime = 0;
unsigned long curtime = 0;
SSD1306 display(0x3c, 4, 15);
