#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

// Se declara la variable para el valor del sensor
int sensorValue;

// Se declara la variable para la matriz de leds
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// Se declara la variable para el estado del aire actual y anterior
int airState=0, airStateOld=0;

// Se declaran las variables del Peltier
int peltier = 5; //The N-Channel MOSFET is on digital pin 3
int pltState=0 , pltStateOLD=0; // Estados Peltier
int powerON = 99; //Power level fro 0 to 99%
int powerOFF = 0;
int peltier_level_ON = map(powerON, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET
int peltier_level_OFF = map(powerOFF, 0, 99, 0, 255);
// Se declaran las variables del botton
int button = 7;
int btnState=0, btnStateOld=0;

void setup()
{
  Serial.begin(9600);                // sets the serial port to 9600
  Serial.println("serial activado");   //debug
  Serial.println("matrix configure");   //debug
  matrix.begin(0x70);                // pass in the address 2 the matrix
  pinMode(peltier, OUTPUT);
  pinMode(button, INPUT);
}

// Cara sonriente
static uint8_t __attribute__ ((progmem)) smile_bmp[]={0x3C, 0x42, 0x95, 0xA1, 0xA1, 0x95, 0x42, 0x3C};
// Cara triste
static uint8_t __attribute__ ((progmem)) frown_bmp[]={0x3C, 0x42, 0xA5, 0x91, 0x91, 0xA5, 0x42, 0x3C};
// Cara neutra
static uint8_t __attribute__ ((progmem)) neutral_bmp[]={0x3C, 0x42, 0x95, 0x91, 0x91, 0x95, 0x42, 0x3C};

void loop()
{
  // Leemos el sensor y determinamos el estado del aire
  sensorValue = analogRead(0);       // read analog input pin 0
  Serial.println(sensorValue, DEC);  // prints the value read
  if (sensorValue < 300) { // Aire limpio
    airState=1;
  }
  else if (sensorValue>=300 && sensorValue<500 ) { // Aire Normal
  airState=2;
  }
  else { // Aire viciado
    airState=3;
  }

  // Si el estado actual del aire es diferente del estado anterior, actualizamos display
  if (airState!=airStateOld) {
    if (airState==1) { // Aire limpio
        matrix.clear();
        matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
    }
    else if (airState==2) { // Aire Normal
      matrix.clear();
      matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();
    }
    else if (airState==3) {  // Aire Viciado
      matrix.clear();
      matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();
    }
  }
  // leemos estado del boton
  btnState = digitalRead(button);
  // si se aprieta el boton
if (btnStateOld==LOW && btnState == HIGH) {
  // si la peltier esta apagada
  if(pltState==LOW){
   // Conectamos la PELTIER
   peltier_level_ON = map(powerON, 0, 99, 0, 255);
   // debug info
   Serial.print(button);
   Serial.print("ON: ");
   Serial.print("Power=");
   Serial.print(powerON);
   Serial.print(" PLevel=");
   Serial.println(peltier_level_ON);
   analogWrite(peltier, peltier_level_ON); //Write this new value out to the port
   //digitalWrite(2,LOW);
   // Cambiamos el estado
   pltState=HIGH;
   }
   else {
   // Si la peltier esta abierta
   // Desconectamos la PELTIER
   peltier_level_OFF = map(powerOFF, 0, 99, 0, 255);
   // debug info
   Serial.print("OFF: ");
   Serial.print("Power=");
   Serial.print(powerOFF);
   Serial.print(" PLevel=");
   Serial.println(peltier_level_OFF);
   analogWrite(peltier, peltier_level_OFF); //Write this new value out to the port
   //digitalWrite(2,LOW);
   // Cambiamos el estado
   pltState=LOW;
   }
  
}
  // El estado actual pasa a ser el antiguo
  btnStateOld=btnState;
  // esperamos un momentito
  delay(100);  // wait 100ms for next reading
}
