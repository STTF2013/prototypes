/*
  Button
 
 Turns on 6 light emitting diodew(LED) connected to pin 3, 5, 6, 9, 10, 11, when pressing a pushbutton attached to pin 2, 4, 7, 8, 12, 13 */
 
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 4;     // the number of the pushbutton pin
const int buttonPin3 = 7;     // the number of the pushbutton pin
const int buttonPin4 = 8;     // the number of the pushbutton pin
const int buttonPin5 = 12;    // the number of the pushbutton pin
const int buttonPin6 = 13;    // the number of the pushbutton pin
const int led1 =  3;      // the number of the LED pin
const int led2 =  5;      // the number of the LED pin
const int led3 =  6;      // the number of the LED pin
const int led4 =  9;      // the number of the LED pin
const int led5 =  10;     // the number of the LED pin
const int led6 =  11;     // the number of the LED pin

// variables will change:
double brightness1 = 0;    // how bright the LED is
double brightness2 = 0;    // how bright the LED is
double brightness3 = 0;    // how bright the LED is
double brightness4 = 0;    // how bright the LED is
double brightness5 = 0;    // how bright the LED is
double brightness6 = 0;    // how bright the LED is
double fadeAmount = 1;     // how many points to fade the LED by

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(led1, OUTPUT);      
  pinMode(led2, OUTPUT);      
  pinMode(led3, OUTPUT);      
  pinMode(led4, OUTPUT);      
  pinMode(led5, OUTPUT);      
  pinMode(led6, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);     
  pinMode(buttonPin2, INPUT);     
  pinMode(buttonPin3, INPUT);     
  pinMode(buttonPin4, INPUT);     
  pinMode(buttonPin5, INPUT);     
  pinMode(buttonPin6, INPUT);     
}

void loop(){
  brightness1 = dimLed(led1, buttonPin1, brightness1);
  brightness2 = dimLed(led2, buttonPin2, brightness2);
  brightness3 = dimLed(led3, buttonPin3, brightness3);
  brightness4 = dimLed(led4, buttonPin4, brightness4);
  brightness5 = dimLed(led5, buttonPin5, brightness5);
  brightness6 = dimLed(led6, buttonPin6, brightness6);
  
// wait for 30 milliseconds to see the dimming effect    
  delay(30);
}

 double dimLed(int ledPin, int buttonPin, double currentBrightness) {
  int buttonState = digitalRead(buttonPin);
 
  if (buttonState == HIGH) {
    //turn led on
    currentBrightness = 255;
    analogWrite(ledPin, int(currentBrightness));                              
  } else {
    // dim LED off:
    // change the brightness for next time through the loop:
    
      currentBrightness = max (currentBrightness - fadeAmount,0);
      
    analogWrite(ledPin, (int)currentBrightness); 
  }
  
   Serial.println(currentBrightness);
  return currentBrightness;
}
