#include <Servo.h> 

//Traffic/Pedestrian Lights
int green13 = 9, red2 = 8, yellow2= 7, green2 = 6, red13 = 5, yellow13 = 4, pedRed = 2, pedGreen = 10;
int time = 10000;
//LDR
int LDR = 0;
int LDRValue = 0;
int light_sensitivity = 90;
int lamp = 12;
//IR
int black = A3, white = 11, receiverValue = 0; 
//Servo Motor
Servo servo;
int pos = 0;
boolean shouldOpen, shouldClose;
//Timer
int timer = 0, oldTimer = 0;
int timeLeft = 0; //created for button
int stage = 0; //created for button
int timer2 = 0;
//Button
int button = 3;
boolean hasShortened = true;
boolean closeTime  =true;

void setup ()
{
  pinMode (red13, OUTPUT);
  pinMode (red2, OUTPUT);
  pinMode (yellow13, OUTPUT);
  pinMode (yellow2, OUTPUT);
  pinMode (green2, OUTPUT);
  pinMode (green13, OUTPUT);
  pinMode (pedRed, OUTPUT);
  pinMode (pedGreen, OUTPUT);
  digitalWrite (red2, HIGH);
  //LDR
  Serial.begin (9600);
  pinMode (lamp, OUTPUT);
  //IR
  Serial.begin (9600);
  pinMode (white, OUTPUT);
  pinMode (white, HIGH);
  //Servo
  servo.attach (13);
  //button
  pinMode(button, INPUT);
}

void loop ()
{
  //Lights
  if (timer == oldTimer + timeLeft || timer == 0)
  {
    oldTimer = timer;
    if (timer != 0)
      stage ++;
    if (timer == 0)
    {
      digitalWrite (pedRed, HIGH);
      digitalWrite (yellow13, LOW);
      digitalWrite (red13, HIGH);
      timeLeft = 2000;
    }
    else if (stage == 1)
    {
      digitalWrite (red2, LOW);
      digitalWrite (green2, HIGH);
      timeLeft = 10000;
      if (hasShortened)
        hasShortened = false;
    }
    else if (stage == 2)
    {
      digitalWrite (green2, LOW);
      digitalWrite (yellow2, HIGH);
      timeLeft = 3000;
    }
    else if (stage == 3)
    {
      digitalWrite (yellow2, LOW);
      digitalWrite (red2, HIGH);
      timeLeft = 2000;
    }
    else if (stage == 4)
    {
      digitalWrite (red13, LOW);
      digitalWrite (green13, HIGH);
      digitalWrite (pedRed, LOW);
      //pedestrian light
      digitalWrite (pedGreen, HIGH);
      timeLeft = 5000;
    }
    else if (stage == 5 || stage == 7 || stage == 9 || stage == 11)
    {
      if (stage == 5)
        digitalWrite (pedGreen, LOW);
      digitalWrite (pedRed, HIGH);
      timeLeft = 500;
    }
    else if (stage == 6 || stage == 8 || stage == 10 || stage == 12)
    {
      digitalWrite (pedRed, LOW);
      if (stage == 12)
      {
        digitalWrite (pedRed, HIGH);
        digitalWrite (green13, LOW);
        digitalWrite (yellow13, HIGH);
        timeLeft = 3000;
      }
      else
        timeLeft = 500;
    }
    else 
    {
      if (stage == 13)
      {
        timer = -10;
        stage = -1;
      }
    }
  }
  //LDR
  LDRValue = analogRead (LDR);
  //Serial.println (LDRValue);
  //delay (50);
  if (LDRValue < light_sensitivity)
    digitalWrite (lamp, HIGH);
  else
    digitalWrite (lamp, LOW);

  //Infrared Sensor
  receiverValue = analogRead (black);
 //Serial.println (receiverValue);
  //delay (50);
   Serial.println (receiverValue);
  if (receiverValue >50 && !shouldOpen && !shouldClose)
  {
    shouldOpen = true;
    //Serial.println (receiverValue);
    //Serial.println (receiverValue); 
  }
  if (receiverValue > 50 && shouldOpen)
    closeTime = false; 
  // Serial.println (receiverValue);
  if (shouldOpen || shouldClose)
  {
    if (shouldOpen)
    {
      timer2 += 10;
      //Serial.println (timer2);
      if (!closeTime)
      {
        timer2 = 0;
        closeTime = true;
      }
      if (pos < 179)
        pos ++;
      else if (pos == 179 && timer2 == 2000)
      {
        shouldOpen = false;
        shouldClose = true;                                        
        timer2 = 0;
      }
    }
    else
    {
      if (shouldClose)
      {
        pos --;
        if (pos == 0)
          shouldClose = false;
      }
    }
    if (pos!=179)
      servo.write (pos);
    delay (9);
  }

  //button
  // Serial.println (digitalRead (button));
  if (digitalRead (button) == HIGH && !hasShortened)
  {
    //Serial.println (timeLeft);
   // Serial.println ((int) (timeLeft*0.6));
    timeLeft = (int) (timeLeft * 0.6);
    hasShortened = true;
  }

  delay (1); //for timer
  timer ++;
}


