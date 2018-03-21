

void triangle(float* X,float* Y){
// display.clear();
 
 display.drawLine(X[0],Y[0],X[1],Y[1]);
 display.drawLine(X[1],Y[1],X[2],Y[2]);
 display.drawLine(X[0],Y[0],X[2],Y[2]);
 display.drawCircle(X[0], Y[0], 5);
 display.display();
}

void triangleAngle( float x, float y,float w, float h,float angleInDegrees) {
  
  float xArray[3] = {0,0,0};
  float yArray[3] = {0,0,0};
 
  float wr = w*0.5; 
  float hr = h*.5;
  angleInDegrees -=90;
  for (int i = 0; i < 3; i++) {
    xArray[i] = x + wr*cos(radians(angleInDegrees));
    yArray[i] = y + hr*sin(radians(angleInDegrees));
    angleInDegrees = angleInDegrees + (360 / 3);
  }
 
  triangle(xArray, yArray );
} // func
//


