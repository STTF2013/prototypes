//This is the main window with all code

#include <LiquidCrystal.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonYes = 6;     // the number of the pushbutton pin
const int buttonNo = 7;     // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;
//String question = "Hello everyone in Portugal!  How are you?";
String questions[] = {"Acha que ajuda o ambiente quando utiliza transportes publicos?","Apoia as greves?","Fala com outras pessoas quando espera pelos transportes?","Acha que apanhar o barco faz parte da cultura de Lisboa?"};
float yesAnswers[] = {0,0,0,0};
float noAnswers[] = {0,0,0,0};
int maxAnswers = 4;

int currentQuestion = 0;

void setup() {
  
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonYes, INPUT);     
  // initialize the pushbutton pin as an input:
  pinMode(buttonNo, INPUT);     
   pinMode(ledPin, OUTPUT);  
   lcd.begin(16, 2); 
   myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
    myservo.write(55); 
  lcd.clear();  
}

void loop(){
  
  
   // set the cursor to (0,0):

  //    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
//    delay(random(15,300));                       // waits 15ms for the servo to reach the position 

  int buttonPress = wiggle();

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonPress == 0) {     
    // turn LED on:    
    //digitalWrite(ledPin, HIGH); 
 
     yesAnswers[currentQuestion]++;
    // myservo.write(servoAngle);
     lcd.clear();
     lcd.print("Respondeste SIM");
     lcd.setCursor(0, 1);
     
     float percentage =  yesAnswers[currentQuestion] / ( yesAnswers[currentQuestion]+ noAnswers[currentQuestion]);
     
         int p = percentage*100;
    String str = "";
    str.concat(p);
   str.concat("% concordam!");
    lcd.print(str);
    
     float angle = 110;
     angle = angle*percentage;
     myservo.write(angle);
      delay(3000);
      currentQuestion ++;
    currentQuestion%=maxAnswers;
      lcd.clear();
  } else if(buttonPress == 1){
   //  digitalWrite(ledPin, HIGH);  
     noAnswers[currentQuestion]++;
    //myservo.write(servoAngle);
    lcd.clear();
    lcd.print("Respondeste NAO");
    lcd.setCursor(0, 1);
    float percentage =  noAnswers[currentQuestion] / ( yesAnswers[currentQuestion]+ noAnswers[currentQuestion]);
     
//    lcd.print(percentage*100);
    int p = percentage*100;
    String str = "";
    str.concat(p);
   str.concat("% concordam!");
    lcd.print(str);
    float angle = 110;
     angle = angle*percentage;
    myservo.write(angle);
    delay(3000);
    currentQuestion ++;
    currentQuestion%=maxAnswers;
    lcd.clear();
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
  delay(300);
}

int wiggle() {
  
  int iterations = 0;
      lcd.setCursor(0, 0);
    lcd.print(questions[currentQuestion]);
  
  while(true) {
    iterations++;
    buttonState1 = digitalRead(buttonYes);
    buttonState2 = digitalRead(buttonNo);
   
   if(buttonState1 == HIGH || buttonState2 == HIGH)
     break;
    // read the state of the pushbutton value:
    
    myservo.write(55);
//    delay(random(10,50));
    myservo.write(55-random(5,30));
    
    buttonState1 = digitalRead(buttonYes);
    buttonState2 = digitalRead(buttonNo);
   
   if(buttonState1 == HIGH || buttonState2 == HIGH)
     break;
    
    delay(random(10,50));
    myservo.write(55+random(5,30)); 
    
    if(iterations % 10 == 0) {
      lcd.scrollDisplayLeft();
      iterations = 0;
    }
  }
  
  if(buttonState1 == HIGH)
    return 0;
  else
    return 1;
}



