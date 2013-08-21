// Bottom Line
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrices[] = {
  Adafruit_8x8matrix(),
  Adafruit_8x8matrix(),
  Adafruit_8x8matrix(),
  Adafruit_8x8matrix(),
};

const int width = 4;
const int height = 2;
const int pixels = 8;

int currentPositionX = 0;
int currentPositionY = 0;
int firstRun = 1;

int table[pixels*width][pixels*height];

int16_t pos_x =0;
int16_t pos_y =0;
uint16_t col  =15;

void setup() {
  matrices[0].begin(0x73); // pass in the address
  matrices[1].begin(0x71); // pass in the address
  matrices[2].begin(0x72); // pass in the address
  matrices[3].begin(0x70); // pass in the address
  Serial.begin(9600);
  pinMode(12,INPUT);
  currentPositionX = readX();
  currentPositionY = readY();
}

int readX() {
  return pixels*width-1 - map(analogRead(A0),0,1023,0,pixels*width-1);
}

int readY() {
  return map(analogRead(A1),0,1023,0,pixels*height-1);
}

void loop() {
  
  // clear matrix value - to be corrected
  if(digitalRead(12) == HIGH) {
    for(int i = 0 ; i < width*pixels ; i++) {
         for(int j = 0 ; j < height*pixels ; j++) {
           table[i][j] = 0;
      }
    }
  }
  
  int newX = readX();
  int newY = readY();
  
  if(newX == currentPositionX && newY == currentPositionY) {
      //BLINK
      if(table[currentPositionX][currentPositionY] == 1)
        table[currentPositionX][currentPositionY] = 0;
      else
        table[currentPositionX][currentPositionY] = 1;
   } else{
      //MOVEMENT
     table[currentPositionX][currentPositionY] = 1;
     table[newX][newY] = 1;
   }
  
  //table[newX][newY] = 1;
  
  currentPositionX = newX;
  currentPositionY = newY;

  drawTable();

  // print out the value read in serial:
 // Serial.println(sensorValue);
  delay(20);        // delay in between reads for stability
}

void drawTable() {
  int x = 0;
  int y = 0;
  int currentMatrix = 0;
  for(int j = 0 ; j < width ; j++) {
    drawSingleMatrixLED(currentMatrix,x,y);
    x+=pixels;
    currentMatrix++;
  }  
}

void drawSingleMatrixLED(int currentIndex, int startingX, int startingY) {
  
  Adafruit_8x8matrix matrix = matrices[currentIndex];
  
  for(int i = 0 ; i < pixels ; i++) {
      for(int j = 0 ; j < pixels ; j++) {
        if(table[startingX+j][startingY+i] == 1)
          matrix.drawPixel(j, i,col);
    }
  }
  matrix.writeDisplay();
}
