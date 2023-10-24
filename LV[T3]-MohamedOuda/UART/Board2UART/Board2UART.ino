#include <Servo.h>

#define PB_PIN 5
#define SERVO_PIN 9


#define OFF 0
#define ON 1


byte g_pbState =OFF; /*Button State*/
byte g_potReading=0; /*Received Potentiometer Reading*/

Servo ServoMotor; /*Creating a servo object to control it*/

void setup()
{
  Serial.begin(9600); /*Beginnig serial communicaton for
              UART*/
  
  /*Setting the PB pin as an input
  pin with internal pullup resistance enabled*/
  pinMode(PB_PIN,INPUT_PULLUP); 
  
  /*Attaching the created servo object to the servo pin*/
  ServoMotor.attach(SERVO_PIN);
}

void loop()
{
  /*This nested if is to check for the button state as well
  as take measures against debouncing, the PB is pulled up
  meaning if it's pressed the digital pin reads low*/
  if(digitalRead(PB_PIN)==LOW){ 
    delay(30); /*Dealy to wait out debouncing*/
    if(digitalRead(PB_PIN)==LOW){
      /*Send to Board 1 that the button is presed*/
      Serial.write(ON); 
    }
  }
  else{
    /*Send to Board 1 that the button is released*/
    Serial.write(OFF);
  }
  
  /*Checks for incoming Potentiometer reading*/
  if(Serial.available()){
    g_potReading=Serial.read();
  } 
  /*Moves the servo according to the potentiometer reading*/
  ServoMotor.write(g_potReading);
}
