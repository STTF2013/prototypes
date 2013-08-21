// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <Time.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,6,5,4,3,2);

// initialize the library for the water temperature sensor
OneWire  ds(A0);  // on pin 10 (a 4.7K resistor is necessary

// initialize a number of special characters to print on the LCD
 
// degree centigrade
byte degree[8] = {
  B01000,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
 
// diagonal right (for temperature symbol)
byte diagonalArrow[8] = {
  B01111,
  B00011,
  B00101,
  B01001,
  B10000,
  B00000,
  B00000,
  B00000
};
 
// water symbol (wt vertical)
byte wtSymbol[8] = {
  B10001,
  B11011,
  B01110,
  B01010,
  B00000,
  B11111,
  B00100,
  B00100
};
 
// sailor symbol (sl vertical)
byte slSymbol[8] = {
  B11000,
  B10000,
  B01000,
  B11000,
  B00000,
  B01000,
  B01000,
  B01100
};
 
// wind symbol (wn vertical)
byte wnSymbol[8] = {
  B10001,
  B11011,
  B01110,
  B01010,
  B00000,
  B11010,
  B11110,
  B10110
};

// an array to store the labels for the week days

String weekDays[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

// a 2 dimensional array to store the values for each day
//values: {water temp, total number of sailors, morning participation, noon participation, evening participation, wind value}
int weekValues[7][6] = { {22,10,200,50,100,7}, {18,6,200,0,50,4}, {10,15,100,255,50,5}, {17,8,20,100,50,7}, {12,0,150,50,0,1}, {28,0,10,0,0,2}, {30,10,100,150,50,3} };

// constants to define some fixed values
int _BRIGHTSTEP = 5.1;   //  ( nrsailors * 100 / 50) * 255 / 100

// constants for LCD dimension
int _LCDROWS = 2;
int _LCDCOLS = 16;

// constants to define the position of the values in the 2 dimensional array
int _WTRTEMP = 0;
int _SAILORS = 1;
int _MORNING = 2;
int _NOON = 3;
int _EVENING = 4;
int _WIND = 5;

// constants to define the position (pin) of the LEDs
int _LEDMORNING = 10;
int _LEDNOON = 11;
int _LEDEVENING = 9;

// constants to define the positoin (pin) of the three buttons: left, right, log in
int _BTNLEFT = 8;
int _BTNRIGHT = 12;
int _BTNLOG = 13;

// constants to define the number for the special characters
int _DEGREE = 1;
int _DIAGONALARR = 2;
int _WTSYMBOL = 3;
int _SLSYMBOL = 4;
int _WNSYMBOL = 5;

// variable to define intial day to start (5 = Saturday)
int numDay = 5;

// variable to define the on/off state of the "wind sensor"
int onoff = 0;

void setup() {
  
  delay(3000);
  
  // creating the special characters
  lcd.createChar(_DEGREE, degree);
  lcd.createChar(_DIAGONALARR, diagonalArrow);
  lcd.createChar(_WTSYMBOL, wtSymbol);
  lcd.createChar(_SLSYMBOL, slSymbol);
  lcd.createChar(_WNSYMBOL, wnSymbol);
  
  // pinModes for the three buttons
  pinMode(_BTNLEFT,INPUT);
  pinMode(_BTNRIGHT,INPUT);
  pinMode(_BTNLOG,INPUT);
  
  // pinModes for the three LEDs
  pinMode(_LEDMORNING, OUTPUT);
  pinMode(_LEDNOON, OUTPUT);
  pinMode(_LEDEVENING, OUTPUT);
  
  // set up the debugging environment
  Serial.begin(9600);
  
  while(!Serial);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(_LCDCOLS, _LCDROWS);
  
   // Turn on the display:
  lcd.display();
  
  // position the cursor so 'SailorScope' is centered
  lcd.setCursor(3,0);

  // Print a message to the LCD.
 lcd.print("SaiLoRSCoPe");
 
 // position the cursor in the second line to print the values (that's in the loop function)
 lcd.setCursor(0,1);
 
}

// function to turn on the LEDs depending on the selected day of the week 
void displayParticipation(int day){
  analogWrite(_LEDMORNING, weekValues[day][_MORNING]);
  analogWrite(_LEDNOON, weekValues[day][_NOON]);
  analogWrite(_LEDEVENING, weekValues[day][_EVENING]);
 
}

// function to print the values day, water temp, number of sailors, wind in the LCD
void printDisplay(int day, int temp, int sailors, int wind){
   lcd.setCursor(0,1);
   lcd.print(weekDays[day]);
   lcd.print(" ");
   
   lcd.write(_WTSYMBOL);
   lcd.print((String)temp);
   lcd.write(_DEGREE);
   lcd.print(" ");
   
   lcd.write(_SLSYMBOL);
   lcd.print((String)sailors);
   lcd.print(" ");
   
   lcd.write(_WNSYMBOL);
   lcd.print( (String)wind);
   lcd.write(_DIAGONALARR);
  
}

// function to get the water temp value from the sensor (copied from an example)
int getWaterTemp(){
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  int celsius;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
  }
  
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return 93;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (int)raw / 16.0;

  return celsius;
}

// function for the log in button: it increases the value (brightness) of the LED
// for the prototype a random number between 0 and 23 represents the current hour (when someone logs in)
void logIn(int numDay){
  
  int hour = random(23);
  int timeOfDay = 0;
  
  if ((hour >= 0) && (hour <12))  
    timeOfDay = _MORNING;
  else if ((hour >= 12) && (hour < 18))
    timeOfDay = _NOON;
  else if ((hour >= 18) && (hour <= 23))
    timeOfDay = _EVENING;
    
  timeOfDay = _EVENING;
 
  if (  weekValues[numDay][timeOfDay] < 255)
    weekValues[numDay][timeOfDay] += _BRIGHTSTEP;
    
  weekValues[numDay][1] += 1;
}


void loop() {
  
  // variables to program the wind sensor (but it was not finished)
  int firstMinute = 0;
  int lastMinute = 0;
  int firstSecond = 0;
  int lastSecond = 0;
  int diffTime = 0;
  
 // to check the values of the reed sensor (for the wind velocity)   
 Serial.println(digitalRead(15));
 
 // ** the wind part [not finshed] ** /
 if (digitalRead(15) == 1 && onoff == 0) {
    onoff = 1;
    firstSecond = second();
    firstMinute = minute();
 } else if (digitalRead(15) == 0 && onoff == 1) {
   onoff = 0;
   lastSecond = second();
   lastMinute = minute();
   
   if (firstMinute == lastMinute) {
      diffTime = lastSecond - firstSecond;
   }  
   
 }
 
 // A condition statement to detect if one of the three button was clicked
  if (digitalRead(_BTNRIGHT) == HIGH) {  // pressing right button: moves one day forward
    if (numDay == 6)
      numDay = 0;
    else
      numDay++;  
  }   else if (digitalRead(_BTNLEFT) == HIGH) { // pressing left button: moves one day backward
    if (numDay == 0 )
      numDay = 6;
    else
      numDay--;
  } else if (digitalRead(_BTNLOG) == HIGH){ // pressing log in button: calls the function logIn with the selected day as input
    logIn(numDay);
    delay(50);
  }  

// For the prototype: A condition statement to check if the day is Saturday (day of the demo)
// then water temp comes from the sensor; otherwise all values are retrieved from the 2 dimensional array with fixed (history) values
   
  if (numDay == 5)  { //Saturday
    int temp = getWaterTemp(); 
    printDisplay(numDay, temp, weekValues[numDay][_SAILORS], weekValues[numDay][_WIND]);
  } else {
    printDisplay(numDay, weekValues[numDay][_WTRTEMP], weekValues[numDay][_SAILORS],  weekValues[numDay][_WIND]);
  }
  
  // Display the LED's
  
  displayParticipation(numDay);
  delay(200);
  
}


