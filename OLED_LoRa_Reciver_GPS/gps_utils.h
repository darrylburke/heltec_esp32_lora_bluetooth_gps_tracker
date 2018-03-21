

double R = 6371.00;
double toDegrees = 57.295779;

void bytesToLatLon(float* results,byte *latlonbytes,int len){
  float lat;
  float lon;
  float deviceid;

  deviceid = latlonbytes[0];
  
  ulat.b[0] = latlonbytes[1];
  ulat.b[1] = latlonbytes[2];
  ulat.b[2] = latlonbytes[3];
  ulat.b[3] = latlonbytes[4];
  
  ulon.b[0] = latlonbytes[5];
  ulon.b[1] = latlonbytes[6];
  ulon.b[2] = latlonbytes[7];
  ulon.b[3] = latlonbytes[8];
  
  lat = ulat.fval;
  lon = ulon.fval;

  results[0]=deviceid;
  results[1]=lat;
  results[2]=lon;
}
void getMyLocation(float* results){
  
  results[0]=0;
  results[1]=HOMELAT;
  results[2]=HOMELON;
  
}
float calc_bearing(float lat1, float lon1, float lat2, float lon2) {

  float Rlon1 = lon1*(PI/180);
  float Rlat1 = lat1*(PI/180);
  float Rlon2 = lon2*(PI/180);
  float Rlat2 = lat2*(PI/180);

  //This portion calculates the differences for the Radian latitudes and longitudes and saves them to variables
  float dlon = Rlon2 - Rlon1;
  float dlat = Rlat2 - Rlat1;
  
  //This portion is the Haversine Formula for distance between two points. Returned value is in KM
  float a = (sq(sin(dlat/2))) + cos(Rlat1) * cos(Rlat2) * (sq(sin(dlon/2)));
  float e = 2 * atan2(sqrt(a), sqrt(1-a)) ;
  float d = R * e;

  //This portion is the Haversine Formula for required bearing between current location and destination. Returned value is in Degrees
  double x = cos(Rlat2)*sin(Rlon2-Rlon1); //calculate x
  double y = cos(Rlat1)*sin(Rlat2)-sin(Rlat1)*cos(Rlat2)*cos(Rlon2-Rlon1); //calculate y

  float brRad = atan2(x, y); 
  float reqBear = toDegrees*brRad;
  return (reqBear);
}


float calc_dist(float lat1, float lon1, float lat2, float lon2)
{
float dist_calc=0;
float dist_calc2=0;
float diflat=0;
float diflon=0;

//I've to spplit all the calculation in several steps. If i try to do it in a single line the arduino will explode.
diflat=radians(lat2-lat1);
lat1=radians(lat1);
lat2=radians(lat2);
diflon=radians((lon2)-(lon1));

dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
dist_calc2= cos(lat1);
dist_calc2*=cos(lat2);
dist_calc2*=sin(diflon/2.0);
dist_calc2*=sin(diflon/2.0);
dist_calc +=dist_calc2;

dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

dist_calc*=6371000.0; //Converting to meters
//Serial.println(dist_calc);
return dist_calc;
}

