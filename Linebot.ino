/****************************************
Linebot
by Bithead942

Arduino Pins:
3 - PWM control for motor outputs 1 and 2
5 - Left LED
6 - Center LED
7 - Right LED
9 - Piezoelectric Buzzer
11 - PWM control for motor outputs 3 and 4
12 - Direction control for motor outputs 1 and 2
13 - Direction control for motor outputs 3 and 4
A0 - Left Sensor
A1 - Center Sensor
A2 - Right Sensor

Requires:
Arduino Uno
1 Sparkfun Motor Shield
1 Piezoelectric buzzer
3 LEDs
3 Analog line sensors (IR emitter/receiver pair)

****************************************/

#include "pitches.h"
const int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
const int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
const int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
const int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13
const int pBuzzer = 9;
const int pLeftSensor = A0;    
const int pCenterSensor = A1;  
const int pRightSensor = A2;
const int pLeftLED = 5;
const int pCenterLED = 6;
const int pRightLED = 7;
const int iLineVal = 750;

int val = 0;     //value for fade
int aFull = 200;  //255 max
int bFull = 200;  //255 max
int iLeftValue = 0;  // variable to store the value coming from the sensor
int iCenterValue = 0;
int iRightValue = 0;
int iLastDirection = 1;

//Star Wars Theme
int melody[] = { NOTE_C4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_D4};
int noteDurations[] = {2,4,8,8,8,2,4,8,8,8,2,4,8,8,8,2};
int iNumNotes = 16;

void setup()
{
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  pinMode(pBuzzer, OUTPUT);
  
  pinMode(pLeftLED, OUTPUT);  
  pinMode(pCenterLED, OUTPUT); 
  pinMode(pRightLED, OUTPUT); 
  
  digitalWrite(pLeftLED, LOW);
  digitalWrite(pCenterLED, LOW);
  digitalWrite(pRightLED, LOW);
  
  Beep(2);
}

void loop()
{ 
    // read the value from the sensor:
  iLeftValue = analogRead(pLeftSensor);  
  iCenterValue = analogRead(pCenterSensor);
  iRightValue = analogRead(pRightSensor);
  
  if (iLeftValue  < iLineVal and iCenterValue >= iLineVal and iRightValue < iLineVal)
     {
       GoForward();
     }
  if (iLeftValue  >= iLineVal and iCenterValue >= iLineVal and iRightValue < iLineVal)
     {
       SlightLeft();
     }
  if (iLeftValue  < iLineVal and iCenterValue >= iLineVal and iRightValue >= iLineVal)
     {
       SlightRight();
     }
  if (iLeftValue  >= iLineVal and iCenterValue < iLineVal and iRightValue < iLineVal)
     {
       MidLeft();
     }
  if (iLeftValue  < iLineVal and iCenterValue < iLineVal and iRightValue >= iLineVal)
     {
       MidRight();
     }
  if (iLeftValue < iLineVal and iCenterValue < iLineVal and iRightValue < iLineVal)
     {
       if (iLastDirection == 0)
          {
          HardLeft();
          }
       if (iLastDirection == 2)
          {
          HardRight();
          }
       if (iLastDirection == 1)
          {
          GoForward();
          }
     }
  if (iLeftValue  >= iLineVal and iCenterValue >= iLineVal and iRightValue >= iLineVal)
     {
          Stopped();
     }
     
  delay(150);
  digitalWrite(pLeftLED, LOW);
  digitalWrite(pCenterLED, LOW);
  digitalWrite(pRightLED, LOW);
}

void Beep(int iTimes)
{
  int i;
  for (i=0; i< iTimes; i++)
     {
     analogWrite(pBuzzer, 100);
     delay(100);
     analogWrite(pBuzzer, 0);
     delay(100);
     }
}

void VictorySound()
{
  for (int thisNote = 0; thisNote < iNumNotes; thisNote++)
    {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 900/noteDurations[thisNote];
    tone(pBuzzer, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pBuzzer);
    }
}

void GoForward() //full speed forward
{ 
  digitalWrite(pCenterLED, HIGH);
  iLastDirection = 1;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull);    
  analogWrite(pwm_b, bFull);
}

void Stopped() //stop
{ 
  analogWrite(pwm_a, 0);    
  analogWrite(pwm_b, 0); 
  VictorySound();
  while(1) { }
}

void SlightLeft()
{
  digitalWrite(pLeftLED, HIGH);
  digitalWrite(pCenterLED, HIGH);
  iLastDirection = 0;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull);    
  analogWrite(pwm_b, bFull - 30);
}

void SlightRight()
{
  digitalWrite(pCenterLED, HIGH);
  digitalWrite(pRightLED, HIGH);
  iLastDirection = 2;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull - 30);    
  analogWrite(pwm_b, bFull);
}

void MidLeft()
{
  digitalWrite(pLeftLED, HIGH);
  iLastDirection = 0;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull);    
  analogWrite(pwm_b, bFull - 110);
}

void MidRight()
{
  digitalWrite(pRightLED, HIGH);
  iLastDirection = 2;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull - 110);    
  analogWrite(pwm_b, bFull);
}

void HardLeft()
{
  iLastDirection = 0;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull - 3);    
  analogWrite(pwm_b, bFull - 135);
}

void HardRight()
{
  iLastDirection = 2;
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, aFull - 135);    
  analogWrite(pwm_b, bFull - 3);
}
