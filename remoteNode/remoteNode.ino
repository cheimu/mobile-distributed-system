

#define START 9
//#define DUTYCYCLE 128 
#include <Time.h>
#include <Wire.h>
#define ON true
#define OFF false
#define PWMON 128
#define PWMOFF 0

bool swlock(int duty, bool sw);

int motorPin = 9;
char val;
bool sw = OFF;
char c = 0;
int s = 0;
int count = 0;
void setup()
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(2, OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  
}


void loop()
{

  delay(100);
   int duty;
  if (c < 0){
    duty = (int)-c;  
  } else {
    duty = (int)c;
  }
  // if (checkAddress(addr)
  sw = swlock(duty, sw);
  if (duty) {
    analogWrite(motorPin, (int)duty);// analogRead values go from 0 to 1023, analogWrite values from 0 to 255; 
    
  } else {
    analogWrite(motorPin, (int)0);
    delay(1000);  
  }

  
  
}

void receiveEvent(int howMany) {
    
    
      while (0 < Wire.available()) { // loop through all but the last
        c = Wire.read(); // receive byte as a character
        
        digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
      }
   
 
}

bool swlock(int duty, bool sw) {
  if (duty != PWMON && sw == OFF) {
    return false; 
  } else if (duty == PWMOFF && sw == ON) {
    return false;  
  
  } else if (duty != PWMOFF && sw == ON) {
    return false;  
  } else if (duty == PWMON && sw == OFF) {
    return true;  
  } else if (duty == PWMOFF && sw == ON) {
    return false;  
  } else {
    return false;  
  }
}
