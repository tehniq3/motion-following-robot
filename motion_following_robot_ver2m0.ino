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
current version is 2m0 - 24.04.2016, Craiova, Romania
see http://www.tehnic.go.ro &
http://arduinotehniq.com/
*************************************************************************************************/

#include <Servo.h> 

Servo myservo;

const int Lin = 10, Rin = 12, Lout = 11, Rout = 13, serv = 9; //setting sensor pins and servo pin
// in = ECHO (iin to receive echo pulse) out = TRIG (pin to send trigger pulse)                                    

// establish variables for duration 
// and the distance result in inches & cm
long Rduration, Lduration, Rinches, Linches, Rcm, Lcm;

int threshold = 75;  //Sensor threshold in cm
byte hist = 5;       // histeresys for distance, in cm
byte angmin = 20;    // minimum angle for servo
byte angmax = 160;   // maximum angle for servo
int distmax = 100;   // maximum distance in cm
byte anglestep = 2;  // angle step for rotate

int midle = 90;  // center position
int angle = 90;  // initial angle

boolean debug = false; //Serial communication for debuging. Set to true for serial communication.
//boolean debug = true; //Serial communication for debuging. Set to true for serial communication.

int contor = 0;
int contormax = 100;   // 40 is for 1 sec
byte miscare = 0;      // if if moving, this vvariable is 1
long initial;          // last moving
long hodina = 3;         // pause for return          

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
//  delayMicroseconds(5);
  delayMicroseconds(10);
  digitalWrite(Rout, LOW);

  Rduration = pulseIn(Rin, HIGH);
  Rcm = Rduration/58;
  if (Rcm > distmax) Rcm = distmax;
  Rinches = Rcm /2.54;
  
  pinMode(Lout, OUTPUT);
  digitalWrite(Lout, LOW);
  delayMicroseconds(2);
  digitalWrite(Lout, HIGH);
//  delayMicroseconds(5);
  delayMicroseconds(10);
  digitalWrite(Lout, LOW);

  Lduration = pulseIn(Lin, HIGH);
  Lcm = Lduration/58;
  if (Lcm > distmax) Lcm = distmax;
  Linches = Lcm /2.54;
    
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
//  follow();

  if (Lcm <= threshold || Rcm <= threshold)
  {
    if (Lcm + hist < Rcm) 
    {
      angle = angle - anglestep;
      miscare = 1;
    }
    if (Rcm + hist < Lcm)
    {
      angle = angle + anglestep;
      miscare = 1;
    }
 // }
  if (angle >= angmax)
  {
    angle = angmax;
    miscare = 0;
    contor = contor +1;
  }
  if (angle <= angmin)
  {
    angle = angmin;
    miscare = 0;
    contor = contor +1;
  }

if (miscare == 1) initial = millis()/1000;

if (miscare == 0) contor = contor + 1;        // increase the counter
miscare = 0; 

 if (debug)
  {
    Serial.print("miscare: ");
    Serial.print(miscare);
    Serial.print(" counter: ");
    Serial.println(contor);  
  }  
if (contor > contormax) {  // if counter is too much
  contor = 0;              // reset the counter
  angle = 90;              // put in center
  delay(500);
}
if (initial + hodina  < millis()/1000) {  // if counter is too much
  contor = 0;              // reset the counter
  angle = 90;              // put in center
  delay(500);
}

  myservo.write(angle);
  miscare =0;
}  // end of good distance
  
  
  if (debug)
  {
    Serial.println("Angle: ");
    Serial.print(angle);
    Serial.println(" degree");
  } 
//miscare = 0;
if (initial + hodina  < millis()/1000) {  // if counter is too much
  contor = 0;              // reset the counter
  angle = 90;              // put in center
  myservo.write(angle);
  miscare =0;
  delay(500);
}
  
} // end of program
