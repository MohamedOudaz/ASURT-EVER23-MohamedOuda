#include <Servo.h>


#define POT_PIN A0
#define LED_PIN 12

#define OFF 0
#define ON 1


byte g_pbState =OFF; /*Received Button State*/

/*Flag to check if the button is held
to avoid continous toggling for one press*/
char g_flagPBHeld=0; 

char g_ledPrevState=OFF;/*Previous state of led for referance*/
byte g_potReading=0; /*Potentiometet reading*/


void setup()
{
  Serial.begin(9600);/*Beginnig serial communicaton for
              UART*/ 
  pinMode(LED_PIN,OUTPUT); /*Setting the LED pin as output*/
  digitalWrite(LED_PIN,LOW);/*Initializing the led*/
}

void loop()
{
  /*getting the reading of potentiometer then 
  mapping it from range (0,1023) to (0,255)*/ 
  int potValue=analogRead(POT_PIN); 
  g_potReading=map(potValue,0,1023,0,255);
  
  /*sending mapped potentiometer reading to board 2*/
  Serial.write(g_potReading);
  
  
  if(Serial.available()){
    g_pbState=Serial.read(); /* checks button state on board 2*/
  } 
  /*This nested if checks for button state connected on board 
  2 then toggles the LED if the button is pressed while taking
  measures against continous toggling for long press*/
  if((g_pbState==ON)&&(g_flagPBHeld==0)){
    g_flagPBHeld=1; /*Sets the flag to 1 to avoid constant
            toggling*/
     if(g_ledPrevState==ON){
       digitalWrite(LED_PIN,LOW);
        g_ledPrevState=OFF;
      }
     else{
        digitalWrite(LED_PIN,HIGH);
        g_ledPrevState=ON;
     }
   }
   else{
      g_flagPBHeld=0; /*Resets the flag*/
   }
}
