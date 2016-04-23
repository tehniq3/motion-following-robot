/*************************************************************************************************
**************************************************************************************************
Motion Follow
Created by Calvin Kielas-Jensen

Using an Arduino UNO, check Instructables.com for the circuit diagram.

This script allows two ultrasonic range sensors to follow movement while mounted on the top of a
servo. The distance threshold can be changed but should not be set too far as the sensors will
begin to fail.

Anyone is welcome to use and modify this code as long as I am given credit. Thank you for
respecting the open source movement! 
**************************************************************************************************
Nicu FLORICA aka niq_ro made some changes in sketch...
see http://www.tehnic.go.ro &
http://arduinotehniq.com/

*************************************************************************************************/

#include <Servo.h> 

Servo myservo;

const int Lin = 10, Rin = 12, Lout = 11, Rout = 13, serv = 9; //setting sensor pins and servo pin

// establish variables for duration 
// and the distance result in inches
long Rduration, Lduration, Rinches, Linches, Rcm, Lcm;

int threshold = 25; //Sensor threshold in inches
byte hist = 1;
byte angmin = 20;
byte angmax = 160;

//int angle = 80; //Initial angle
int angle = 90; //Initial angle

boolean debug = false; //Serial communication for debuging. Set to true for serial communication.
//boolean debug = true; //Serial communication for debuging. Set to true for serial communication.

void setup() {
  // initialize serial communication:
  if (debug)
  {
    Serial.begin(9600);
  }
  myservo.attach(serv); //attach servo to pin 9
   myservo.write(angle);
   delay(2000);
   
}

void loop()
{
  
  //Most of the sensor code has been taken from David Mellis's PING sensor code
  //I modified it for a 4 pin sensor as oppsed to the 3 pin sensor
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(Rout, OUTPUT);
  digitalWrite(Rout, LOW);
  delayMicroseconds(2);
  digitalWrite(Rout, HIGH);
  delayMicroseconds(5);
  digitalWrite(Rout, LOW);

  Rduration = pulseIn(Rin, HIGH);
  
  pinMode(Lout, OUTPUT);
  digitalWrite(Lout, LOW);
  delayMicroseconds(2);
  digitalWrite(Lout, HIGH);
  delayMicroseconds(5);
  digitalWrite(Lout, LOW);

  Lduration = pulseIn(Lin, HIGH);

  // convert the time into a distance
  Rinches = microsecondsToInches(Rduration);
//  delay(50);
  Linches = microsecondsToInches(Lduration);
//  delay(50);
//  Rcm = microsecondsTocm(Rduration);
Rcm = Rinches *2.54;
Lcm = Linches *2.54;
//  delay(50);
//  Lcm = microsecondsTocm(Lduration);
  
    
  if (debug)
  {
    Serial.print("Left: ");
    Serial.print(Linches);
    Serial.print("in (");
    Serial.print(Lcm);
    Serial.println("cm)");
    
    Serial.print("Right: ");
    Serial.print(Rinches);
    Serial.print("in (");
    Serial.print(Rcm);
    Serial.println("cm)");

  }
  follow();
  
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

void follow() 
{
  if (Linches <= threshold || Rinches <= threshold)
  {
    if (Linches + hist < Rinches) 
    {
      angle = angle - 2;
    }
    if (Rinches + hist < Linches)
    {
      angle = angle + 2;
    }
  }
  if (angle >= angmax)
  {
    angle = angmax;
  }
  if (angle <= angmin)
  {
    angle = angmin;
  }
  myservo.write(angle);
  
  if (debug)
  {
    Serial.println("Angle: ");
    Serial.print(angle);
    Serial.println(" degree");
  } 
//  delay(500);
//delay(2);
}
