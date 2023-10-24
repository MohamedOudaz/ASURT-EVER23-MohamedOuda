#include <Servo.h>
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication

#define PB_PIN 5
#define SERVO_PIN 9


#define OFF 0
#define ON 1


byte g_pbState =OFF; /*Button State*/
byte g_potReading=0; /*Received Potentiometer Reading*/

Servo ServoMotor; /*Creating a servo object to control it*/

struct can_frame canMsgSend; /*Structure to hold message to be sent*/
struct can_frame canMsgReceive; /*Structure to hold message to be received*/
 
MCP2515 mcp2515(10);

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  SPI.begin();               //Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();
  
  /*Setting the PB pin as an input
  pin with internal pullup resistance enabled*/
  pinMode(PB_PIN,INPUT_PULLUP); 
  
  /*Attaching the created servo object to the servo pin*/
  ServoMotor.attach(SERVO_PIN);
}

void loop()
{
  /*Msg to be sent, Holds the state of Push Button*/
  canMsgSend.can_id  = 0x012;           //CAN id as 0x012
  canMsgSend.can_dlc = 1;               //CAN data length as 1
  
  /*This nested if is to check for the button state as well
  as take measures against debouncing, the PB is pulled up
  meaning if it's pressed the digital pin reads low*/
  if(digitalRead(PB_PIN)==LOW){ 
    delay(30); /*Dealy to wait out debouncing*/
    if(digitalRead(PB_PIN)==LOW){
      /*Send to Board 1 that the button is presed*/
      canMsgSend.data[0] = ON;    //Update Push Button State as ON in [0]
    }
  }
  else{
    /*Send to Board 1 that the button is released*/
    canMsgSend.data[0] = OFF;    //Update Push Button State as OFF in [0]
  }
  
  mcp2515.sendMessage(&canMsgSend);     //Sends the CAN message
  
  /*Checks for incoming Potentiometer reading*/
  if (mcp2515.readMessage(&canMsgReceive) == MCP2515::ERROR_OK) {
    g_potReading=canMsgReceive.data[0]; /* checks potentiometer reading on board 1*/
  } 
  /*Moves the servo according to the potentiometer reading*/
  ServoMotor.write(g_potReading);
}
