// Example by Tom Igoe

import processing.serial.*;

Serial myPort;  // The serial port
    
    int protocol;
    int cntr;
    int lf = 10; 
    int[] yOffset = {0, 150, 300};
    int[] xOffset = {0,500};
    float[] y1old = {0,0,0,0,0,0};
    float[] y2old = {0,0,0,0,0,0};
    int x=0;
    color c1=#FF0000;
    color c2=#00FF00;

    
void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[5], 57600);
  size(1000,450);
  noSmooth();
  background(0);
}

void draw() {
  
  byte[] bytes = new byte[15];
  while (myPort.available() > 0) {
    int c1=myPort.read();
    int c2=myPort.read();
    println(c1,c2);
    if ((c1==0xa5) && (c2==0x5a)) {
      myPort.readBytes(bytes);
      x++;
      stroke(0);
      line(x,0,x,450);
      line(x+500,0,x+500,450);
      if (x>500) {x=0;}
      for(int c = 0; c < 6; c++) {
      int highByte = bytes[2+(c*2)] & 0xFF;
      int lowByte = bytes[2+(c*2)+1] & 0xFF;
      if(highByte < 0) {highByte = highByte + 256;}
      if(lowByte < 0) {lowByte = lowByte + 256;}
        
        int y = lowByte + (256*highByte);
        float yM = map(y, 0, 1023, 0, 150);
        float y1 = yOffset[c%3]+(yM);
        int xo = xOffset[c%2]+x; 
        //println(xo + " " + yo + " " + a + " " + val1 + " " + val2);
        if (xo==0) {
          stroke(c1);
          point(xo,y1);
          y1old[c]=y1;
        } else {
         stroke(c1);
         line(xo,y1,xo-1,y1old[c]);
         stroke(c2);
         y1old[c]=y1;
        }
    }
    }
  }
}


  