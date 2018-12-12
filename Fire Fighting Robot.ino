//#include<SoftwareSerial.h>
#include <Servo.h>

//int d1 = 2;
//int d3 = 4;
Servo myservo;  
int pos = 0;
int flag = 0;
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
const int firesensorAhead = A3;
const int firesensorLeft = A4;
const int firesensorRight = A5;
const int rightDelay = 460 ;
const int leftDelay = 440;
const int obs1Echo = 3;//ahead
const int obs2Echo = 2;//leftAhead
const int obs3Echo = 4;//rightAhead
const int obs4Echo = 5; //left
const int obs5Echo = 6; //right
const int obs1Trigger = A0; //ahead
const int obs2Trigger = A1; //left
const int obs3Trigger = A2; //right
const int obs4Trigger= 7; //leftAhead
const int obs5Trigger= 8; //rightAhead
//const int led = 10;
const int motora1 = 10;  //2
const int motora2= 8;   //7
const int motorb2 = 11; //15
const int motorb1 = 12; //10
int nextTurn=1;
//const int firesensor = A0;
const int trigger = 2;
const int buzzer = 13; 
const int cam_motor1 = 11; /* use same in case of valve */
const int cam_motor2 = 12; 
//int enable = 9;  /* enable for cam motor driver */

void setup(){
  //BT.begin(38400);
//  pinMode(led,OUTPUT);
  /*pinMode(d1,INPUT);
  pinMode(d2,INPUT);
  pinMode(d3,INPUT);*/
  myservo.attach(9); 
  pinMode(obs1Echo,INPUT);
  pinMode(obs2Echo,INPUT);
  pinMode(obs3Echo,INPUT);
  pinMode(obs4Echo,INPUT);
  pinMode(obs5Echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(motora1,OUTPUT);
  pinMode(motora2,OUTPUT);
  pinMode(motorb1,OUTPUT);
  pinMode(motorb2,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(firesensorAhead,INPUT);
  pinMode(firesensorLeft,INPUT);
  pinMode(firesensorRight,INPUT);
  pinMode(obs1Trigger,OUTPUT);
  pinMode(obs2Trigger,OUTPUT);
  pinMode(obs3Trigger,OUTPUT);
  pinMode(obs4Trigger,OUTPUT);
  pinMode(obs5Trigger,OUTPUT);
 /* pinMode(cam_motor1,OUTPUT);
  pinMode(cam_motor2,OUTPUT);*/
  Serial.begin(9600);
  myservo.detach();
  
}
  void brake()
  {    
    //Serial.println("BRAKE");
    digitalWrite(motora1,LOW);
        digitalWrite(motora2,LOW);
        digitalWrite(motorb1,LOW);
        digitalWrite(motorb2,LOW);
        delay(800);
  }
  void rotate()
  {
      Serial.println("Rotate");
      digitalWrite(motora1,HIGH);
      digitalWrite(motora2,LOW);
      digitalWrite(motorb2,HIGH);
      digitalWrite(motorb1,LOW);
     
  }
    void left()
  {
            Serial.println("Left");
digitalWrite(motora2,LOW);
      digitalWrite(motora1,HIGH);
      digitalWrite(motorb1,HIGH);
      digitalWrite(motorb2,LOW);

  }
  void right()
  {
            Serial.println("Right");

           digitalWrite(motora1,LOW);
      digitalWrite(motora2,HIGH);
      digitalWrite(motorb1,LOW);
      digitalWrite(motorb2,HIGH);
  }
  void forward(){
       bool fire1 = fireAhead();
      bool fire2 = fireLeft();
      bool fire3 = fireRight();
    
     analogWrite(obs1Trigger, 0);
      delayMicroseconds(2);
      analogWrite(obs1Trigger, 255) ;
      delayMicroseconds(10);
      analogWrite(obs1Trigger, 0) ;
      long duration, cmMiddle;
      duration = pulseIn(obs1Echo,HIGH);
      cmMiddle= microsecondsToCentimeters(duration);
              Serial.println("obstacle Middle *");
              Serial.println(cmMiddle);
      if(cmMiddle <= 19)
      {
        brake(); 
        Serial.println("Brake in forward()");
        delay(2000);
        Serial.println("DELAY OVER");
        return ;
      }
      else{
            Serial.println("Forward");
      digitalWrite(motorb1,HIGH);
      digitalWrite(motorb2,LOW);
      digitalWrite(motora1,LOW);
      digitalWrite(motora2,HIGH);
      }
      
        
  }
  void backward()
  {
            Serial.println("Backward");
    digitalWrite(motora2,LOW);
      digitalWrite(motora1,HIGH);
      digitalWrite(motorb2,HIGH);
      digitalWrite(motorb1,LOW);
      
  }
   void start()
  {     digitalWrite(cam_motor1,HIGH);
        digitalWrite(cam_motor2,LOW);
  }
  void spray_stop()
  {
     digitalWrite(cam_motor1,LOW);
     digitalWrite(cam_motor2,HIGH);
  }
  void normal()
  {
    digitalWrite(cam_motor1,LOW);
    digitalWrite(cam_motor2,LOW);
   }

  bool fireAhead(){
        int sensorReading;
      sensorReading = analogRead(firesensorAhead);
      int range = map(sensorReading, sensorMin, sensorMax, 0, 10);
      Serial.println(sensorReading);
      Serial.println(range);
      if(sensorReading <= 45)
      {
        Serial.println("Fire Ahead!!!");
        brake();  
        while(range <= 8)
        {
          brake();
          Serial.println("In the loop");
          sensorReading = analogRead(firesensorAhead);
          range = map(sensorReading, sensorMin, sensorMax, 0, 10);
          myservo.attach(9);
          if(flag==0){
              for (pos = 90; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
        }
        for (pos = 100; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
          //count++;
        }
              flag = 1;
          }      
          Serial.println(range);
          digitalWrite(buzzer,HIGH);
        }
//        stop spraying
      //delay(10000);
      flag = 0;
      myservo.detach();
 //       myservo.write(100);
        digitalWrite(buzzer,LOW);
        return true;
        
      }
      else{
        return false;
      }
    
  }

   bool fireLeft(){
      int sensorReading;
      sensorReading = analogRead(firesensorLeft);
      int range = map(sensorReading, sensorMin, sensorMax, 0, 10);
      Serial.println(sensorReading);
      Serial.println(range);
      if(range <= 6)
      {
        Serial.println("Fire Left!!!");
        brake();
        left();
        delay(leftDelay);
        brake();
        backward();
        delay(450);
        brake();
        while(true){
            int sensorReading;
            sensorReading = analogRead(firesensorAhead);
            int range = map(sensorReading, sensorMin, sensorMax, 0, 10);
            Serial.println(sensorReading);
            Serial.println(range);   
            //left();
            if(range == 0)
            {
              brake();
             while(range <= 8)
                  {
                    sensorReading = analogRead(firesensorAhead);
          range = map(sensorReading, sensorMin, sensorMax, 0, 10);
          //          start spraying
                            myservo.attach(9);
          if(flag==0){
              for (pos = 90; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
        }
        for (pos = 100; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
          //count++;
        }
              flag = 1;
          }
                    digitalWrite(buzzer,HIGH);
                  }
          //        stop spraying
                  flag=0;
                  myservo.detach();
                  digitalWrite(buzzer,LOW);
                  return true;
                
            }
          
        }
        
      }
      else
          return false;
    
  }

    void startSpray(){
        for (pos = 90; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
        }
        for (pos = 100; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
          //count++;
        }
    }

      

   bool fireRight(){
    int sensorReading;
      sensorReading = analogRead(firesensorRight);
      int range = map(sensorReading, sensorMin, sensorMax, 0, 10);
      Serial.println(sensorReading);
      Serial.println(range);
      if(range <= 6)
      {
        Serial.println("Fire Right!!!");
        brake();
        //right();
        //backward();
        //delay(100);
         right();
        delay(rightDelay);
        brake();
        backward();
        delay(450);
       while(true){
            int sensorReading1;
            sensorReading1 = analogRead(firesensorAhead);
            int range1 = map(sensorReading1, sensorMin, sensorMax, 0, 10);
            //Serial.println(sensorReading1);
            Serial.println("Fire ahead");
            Serial.println(range1);   
            //right();
            if(range1 == 0)
            {
              brake(); 
               
              while(range1<=8)
              {
                forward(); 
                sensorReading1 = analogRead(firesensorAhead);
          range1 = map(sensorReading1, sensorMin, sensorMax, 0, 10);
                if(range1 <= 6)
                {
                  brake();
                  break;
                }
              }
                    while(range1 <= 8)
              {
                sensorReading1 = analogRead(firesensorAhead);
          range1 = map(sensorReading1, sensorMin, sensorMax, 0, 10);
     //          start spraying
                          myservo.attach(9);
          if(flag==0){
              for (pos = 90; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
        }
        for (pos = 100; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(10);                       // waits 15ms for the servo to reach the position
          //count++;
        }
              flag = 1;
          }     
                digitalWrite(buzzer,HIGH);             
              }
      //        stop spraying
        
            myservo.detach();
            if(range1 > 7){
              brake();
            }
              
              digitalWrite(buzzer,LOW);
              return true;
             
            }
          
        }
//        
      }
      else
          return false;  
  }
//  
  void loop(){
    //myservo.detach();
    
      if(obstacleAhead())
      {
        brake();
        Serial.println("Brake in obstacle Ahead");
        if(obstacleRight() && !obstacleLeft()){
          Serial.println("#1");
          //left();
          left();
            delay(leftDelay);
            forward();
            delay(400);
            brake();
            Serial.println("brake in case #1");
            if(obstacleRight())
            {
              left();
              delay(leftDelay);
              nextTurn = 1;
            }
            else{
              right();
              delay(rightDelay);
            }
          //nextTurn=1;
          //delay(7000);
        }
        else if(obstacleLeft() && !obstacleRight()){
          Serial.println("#2");
          right();
           delay(rightDelay);
          //nextTurn=0;
          forward();
            delay(400);
            brake();
            Serial.println("brake in case #2");
            if(obstacleLeft())
            {
              Serial.println("#2-1");
              right();
              delay(rightDelay);
              nextTurn=0;
            }
            else{
              Serial.println("#2-2");
              left();
              delay(leftDelay);
            }
          //delay(2400);
        }
        else if(obstacleLeft() && obstacleRight()){
          Serial.println("#3");
          backward();
          delay(1000);
          if(obstacleRight())
          {
          left();
          delay(leftDelay);
          forward();
          delay(500);
          right();
          delay(rightDelay);
          }
          else if(obstacleLeft())
          {
          right();
          delay(rightDelay);
          forward();
          delay(500);
          left();
          delay(leftDelay);
          }
        }
        else{
          if(nextTurn==1){
            Serial.println("#4");
            right();
            delay(rightDelay);
            forward();
            delay(500);
            brake();
            Serial.println("brake in case #4");
            if(obstacleLeft())
            {
              Serial.println("#4-1");
              right();
              int del = rightDelay;
              delay(del);
              nextTurn=0;
            }
            else{
              Serial.println("#4-2");
              left();
              delay(leftDelay);
              forward();
              delay(850);
              brake();
              left();
              delay(leftDelay);
              forward();
              delay(500);
              brake();
              right();
              delay(rightDelay);
            }
            
            //delay(7000);
          }
          else if(nextTurn==0){
            Serial.println("#5");
            left();
            delay(leftDelay);
            forward();
            delay(500);
            brake();
            Serial.println("brake in case #5");
            if(obstacleRight())
            {
              Serial.println("#5-1");
              left();
              delay(leftDelay);
              nextTurn = 1;
            }
            else{
              Serial.println("#5-2");
              right();
              delay(rightDelay);
              forward();
              delay(850);
              brake();
              right();
              delay(rightDelay);
              forward();
              delay(500);
              brake();
              left();
              delay(leftDelay);
            }
        
        //    delay(7000);
          }
        }
      }
      else{
//        bool right1= obstacleLeftAhead();
//        delay(2000);
         //else if(!obstacleLeftAhead() && !obstacleRightAhead())
          //{
              forward();
              Serial.println("#4-1");
         //}
//         else if(obstacleLeftAhead() && !obstacleRightAhead() && !obstacleAhead())
//         {
//          right();
//          delay(rightDelay);
//          forward();
//          delay(200);
//          brake();
//          if(!obstacleLeft())
//          {
//            left();
//            delay(leftDelay);
//          }
//         }
//         else if(!obstacleLeftAhead() && obstacleRightAhead() && !obstacleAhead())
//         {
//          left();
//          delay(leftDelay);
//          forward();
//          delay(200);
//          brake();
//          if(!obstacleRight())
//          {
//            right();
//            delay(rightDelay);
//          }
//         }
        
        
     // delay(2000);
      // brake();  
      }
      //delay(3000);
     
}
      long microsecondsToInches(long microseconds)
      {
      return microseconds / 74 / 2;
      }
      
      long microsecondsToCentimeters(long microseconds)
      {
      return microseconds / 29 / 2;
      }



    bool obstacleAhead(){
         bool fire1 = fireAhead();
      bool fire2 = fireLeft();
      bool fire3 = fireRight();
      analogWrite(obs1Trigger, 0);
      delayMicroseconds(2);
      analogWrite(obs1Trigger, 255) ;
      delayMicroseconds(10);
      analogWrite(obs1Trigger, 0) ;
      long duration, cmMiddle;
      duration = pulseIn(obs1Echo,HIGH);
      cmMiddle= microsecondsToCentimeters(duration);
              Serial.println("obstacle Middle");
              Serial.println(cmMiddle);

   
      if(cmMiddle <= 19)
      {
        return true;
      }
      return false;
     }

   bool obstacleLeftAhead(){
//       digitalWrite(obs4Trigger, LOW);
//      delayMicroseconds(2);
//      digitalWrite(obs4Trigger, HIGH);
//      delayMicroseconds(10);
//      digitalWrite(obs4Trigger, LOW);
//
//       long duration, cmLeft;
//      duration = pulseIn(obs2Echo,HIGH);
//      cmLeft = microsecondsToCentimeters(duration);
//              Serial.println("obstacle Left Ahead");
//              Serial.println(cmLeft);
//      if(cmLeft <= 30)
//      {
//        return true;
//      }
//        return false;

     }
     

     
     bool obstacleRightAhead(){
//       digitalWrite(obs5Trigger, LOW);
//      delayMicroseconds(2);
//      digitalWrite(obs5Trigger, HIGH);
//      delayMicroseconds(10);
//      digitalWrite(obs5Trigger, LOW);
//
//       long duration, cmRight;
//      duration = pulseIn(obs3Echo,HIGH);
//      cmRight = microsecondsToCentimeters(duration);
//              Serial.println("obstacle Right Ahead");
//              Serial.println(cmRight);
//      if(cmRight <= 30)
//      {
//        return true;
//      }
//        return false;

     }

    bool obstacleLeft(){
      bool fire1 = fireAhead();
      bool fire2 = fireLeft();
      bool fire3 = fireRight();
      analogWrite(obs2Trigger, 0);
      delayMicroseconds(2);
      analogWrite(obs2Trigger, 255);
      delayMicroseconds(10);
      analogWrite(obs2Trigger, 0);
      long duration, cmLeft;
      duration = pulseIn(obs4Echo,HIGH);
      cmLeft = microsecondsToCentimeters(duration);
              Serial.println("obstacle Left");
              Serial.println(cmLeft);
      if(cmLeft <= 30)
      {
        return true;
      }
        return false;
     }


      bool obstacleRight(){
           bool fire1 = fireAhead();
      bool fire2 = fireLeft();
      bool fire3 = fireRight();
      analogWrite(obs3Trigger, 0);
      delayMicroseconds(2);
      analogWrite(obs3Trigger, 255);
      delayMicroseconds(10);
      analogWrite(obs3Trigger, 0);
      long duration, cmRight;
      duration = pulseIn(obs5Echo,HIGH);
      cmRight = microsecondsToCentimeters(duration);
              Serial.println("obstacle Right");
              Serial.println(cmRight);
 
      if(cmRight<= 30)
      {
        return true;
      }
         return false;
     }

