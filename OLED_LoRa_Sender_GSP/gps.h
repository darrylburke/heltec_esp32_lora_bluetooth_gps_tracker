int vals = 5;
float lats[] = {44.028348,44.029121,44.029422,44.028932,44.033198};
float lons[] = {-79.445234,-79.441541,-79.44437,-79.448178,-79.447129};
float currentlat = 44.028631;
float currentlon = -79.443978;
float homelat = currentlat;
float homelon = currentlon;
int gpscounter=0;

struct latlon {
   float lat;
   float lon;
};


void initGPS() {
  //do nothing
  randomSeed(analogRead(0));
}


 struct latlon getcurrentPosition() {

  float  Distance  = (50+random(100)) / 1000;
  Distance = .050;
  float  Bearing = random(360);
 // Bearing =0;
  
  struct latlon tmpposition;


float CurLon = currentlon;
float CurLat = currentlat;
float r_CurLon;
float r_CurLat;

float r_Bearing;

int Eradius = 6371; // mean radius of the earth  

  r_CurLon = radians(CurLon);
  r_CurLat = radians(CurLat);
  r_Bearing = radians(Bearing);
  float DestLat = asin(sin(r_CurLat)*cos(Distance/Eradius)+cos(r_CurLat)*sin(Distance/Eradius)*cos(r_Bearing)); 
  float DestLon = r_CurLon + atan2(sin(r_Bearing)*sin(Distance/Eradius)*cos(r_CurLat),cos(Distance/Eradius)-sin(r_CurLat)*sin(DestLat)); 
  DestLon = (DestLon+3*PI)/(2*PI);
  int i = DestLon;
  DestLon = (DestLon - i) * (2*PI) - PI;  // normalise to -180..+180º

  currentlon = degrees(DestLon);
  currentlat = degrees(DestLat);
  tmpposition.lat=currentlat;
  tmpposition.lon=currentlon;
//
//  tmpposition.lat = lats[gpscounter];
//  tmpposition.lon = lons[gpscounter];
//  gpscounter++;
// if ( gpscounter >= vals) {
//   gpscounter=0;
// }

  return tmpposition;
}




