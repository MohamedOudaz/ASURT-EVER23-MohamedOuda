#include <Servo.h>
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication

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

struct can_frame canMsgSend; /*Structure to hold messege to be sent*/
struct can_frame canMsgReceive; /*Structure to hold messege to be received*/
 
MCP2515 mcp2515(10);


void setup()
{
  while (!Serial);
  Serial.begin(9600);
  SPI.begin();               //Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();
  pinMode(LED_PIN,OUTPUT); /*Setting the LED pin as output*/
  digitalWrite(LED_PIN,LOW);/*Initializing the led*/
}

void loop()
{
  /*getting the reading of potentiometer then 
  mapping it from range (0,1023) to (0,255)*/ 
  int potValue=analogRead(POT_PIN); 
  g_potReading=map(potValue,0,1023,0,255);
  
  canMsgSend.can_id  = 0x014;           //CAN id as 0x014
  canMsgSend.can_dlc = 1;               //CAN data length as 1
  canMsgSend.data[0] = g_potReading;    //Update Potentiometer value in [0]

 
  mcp2515.sendMessage(&canMsgSend);     //Sends the CAN message
  
  /*Checks for any sent messages*/
  if (mcp2515.readMessage(&canMsgReceive) == MCP2515::ERROR_OK) {
    g_pbState=canMsgReceive.data[0]; /* checks button state on board 2*/
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
