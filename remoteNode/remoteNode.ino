#include <Time.h>
#include <Wire.h>
#define ON true
#define OFF false
#define PWMON 111
#define PWMOFF 102
#define START 9

int motorPin = 9;
char command = 0;
byte SlaveSend;

char level = 0;
int setPos = 0;
int curPos = 0;
int preSpeed = 0;
int curSpeed = 0;
int realSpeed = 0;
int largeErrCount = 0;

char val;
int s = 0;
int count = 0;
byte x;


// PWM output @ 25 kHz, only on pins 9 and 10.
// Output value should be between 0 and 320, inclusive.
void analogWrite25k(int pin, int value){
  switch (pin) {
    case 9:
      OCR1A = value;
      break;
    case 10:
      OCR1B = value;
      break;
    default:
      // no other pin will work
      break;
    }
}

void setup(){
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(2, OUTPUT);
  /*pinMode(motorPin, OUTPUT);*/
  // Configure Timer 1 for PWM @ 25 kHz.
    TCCR1A = 0;           // undo the configuration done by...
    TCCR1B = 0;           // ...the Arduino core library
    TCNT1  = 0;           // reset timer
    TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
    ICR1   = 320;         // TOP = 320
    x = 1;
    // Set the PWM pins as output.
    pinMode(motorPin, OUTPUT);
    randomSeed(analogRead(0));
}

void receiveEvent(int howMany){
  while (0 < Wire.available()){ // loop through all but the last
    command = Wire.read(); // receive byte as a character
    if (command >= 0 && command <= 9){
      setPos = 100 * (int) command;  
    }
    if (command < 0){
      command = -command;  
    }
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent(){
  SlaveSend = (byte)level; 
  Wire.write(SlaveSend);   // sends one byte converted POT value to master
}

void printSpeed(int num) {
  if (num < 48) {
     Serial.print((char)48);
     Serial.print((char)48);
     Serial.print((char)48);
  } else {
    Serial.print((char)(num/100+48));
    Serial.print((char)(num/10%10+48));
    Serial.print((char)(num%10+48));
  }
}

void statusReport(){
  Serial.write(27);
  Serial.write("[2J");
  Serial.println();
  
  Serial.println("////////////////////////////////////////");
  Serial.println("////////      Welcome!            //////");
  Serial.print("////////   control speed:   ");
  printSpeed(curSpeed);
  Serial.println("   //////");
  Serial.print("////////   real speed:      ");
  printSpeed(realSpeed);
  Serial.println("   //////");
  Serial.print("////////   set position:     ");
  Serial.print(setPos/100);
  Serial.print(".");
  Serial.print(setPos/10%10);
  Serial.println("  //////");
  Serial.print("////////   current position: ");
  Serial.print(curPos/100);
  Serial.print(".");
  Serial.print(curPos/10%10);
  Serial.print(curPos%10);
  Serial.println(" //////");
  Serial.println("////////                          //////");
  Serial.print("////////   Warning level:    ");
  Serial.print(level);
  Serial.println("    //////");
  if (level == 48) {
    Serial.println("////////      Warning!            //////");  
  }
  Serial.println("////////////////////////////////////////");
  Serial.write(27);
  Serial.print("[H");
}

void setPWM() {
  // analogRead values go from 0 to 1023
  // analogWrite values from 0 to 320 
  if (command == 'o') {
    preSpeed = 160;
    curSpeed = 160;
    realSpeed = 160;
  } else if (command == 'f') {
    curSpeed = 0;
    preSpeed = 0;  
  } else if (command == 'i') {
    if (realSpeed*1.005 <= 320) {
      curSpeed = preSpeed*1.005 + 0.5;   
    } else {
      curSpeed = 320;  
    }
    preSpeed = curSpeed;
  } else if (command == 'd') {
    if (realSpeed*0.995 >= 0) {
      curSpeed = preSpeed*0.995 + 0.5;
    } else {
      curSpeed = 0;
    }  
    preSpeed = curSpeed;
  } else {
    curSpeed = preSpeed;
    preSpeed = curSpeed;
  }
  analogWrite25k(motorPin, curSpeed);
}

int measure() {
 int randNumber = 0; 
 if (largeErrCount > 10) {
  largeErrCount = 0;
 }
 if (largeErrCount >= 8 && largeErrCount <= 10) {
  randNumber = random(-9, 9); 
 } else {
  randNumber = random(-3, 3); 
 }
 largeErrCount+=1;
 return randNumber; 
}

void changePos() {
  if (curPos/10 == setPos/10) {
    curSpeed = 0;
    preSpeed = 0; 
    realSpeed = 0; 
  } else if (curPos < setPos) {
    if (curSpeed > 0){
      curPos = curPos + realSpeed/16;
    }
  } else if (curPos > setPos) {
    if (curSpeed > 0){
      curPos = curPos - realSpeed/16;
    }
  }
}

void getWarn() {
  if (curSpeed == 0){
    level = 50;  
  } else if ( realSpeed >= curSpeed * 1.05 || realSpeed <= curSpeed * 0.95) {
    level = 48; 
  } else if ( realSpeed >= curSpeed * 1.02 || realSpeed <= curSpeed * 0.98) {
    level = 49;
  } else if ( realSpeed >= curSpeed * 1.01 || realSpeed <= curSpeed * 0.99) {
    level = 50;  
  } else {
    level = 50; 
  }
}

void loop()
{
  // use the command to set the PWM output
  // get the curSpeed, preSpeed and realSpeed from it
  setPWM(); 
  
  // get the real speed from monitoring the voltage
  realSpeed = curSpeed + measure();

  // change the position 
  changePos(); 

  // get the warning level 
  getWarn(); 

  // Display the status 
  statusReport();
  delay(1000);
}
