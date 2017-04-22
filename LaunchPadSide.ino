#include <FastCRC.h>
/*
   __________  __________ __                           __    ____            __
  / ____/ __ \/ ___/ ___// /   ____ ___  ______  _____/ /_  / __ \____ _____/ /
 / /   / /_/ /\__ \\__ \/ /   / __ `/ / / / __ \/ ___/ __ \/ /_/ / __ `/ __  / 
/ /___/ ____/___/ /__/ / /___/ /_/ / /_/ / / / / /__/ / / / ____/ /_/ / /_/ /  
\____/_/    /____/____/_____/\__,_/\__,_/_/ /_/\___/_/ /_/_/    \__,_/\__,_/   
   Ethan Chao 2017  
------------------------------------------------------------------------------
List of things this program must do ($Tested, #Implemented, %To do):
%Receive the register and parse using StringObject: https://www.arduino.cc/en/Reference/StringObject
%Confirm message fidelity with the checksum
%Read register state and apply it to the pins (LOW When triggered)

                                                                             
 */
//uint16_t a = 0b1000000000000000; //The 16 bit register I'm using to store the button states to be received
FastCRC8 CRC8; //Some kind of setup for CRC8 usage
void receivecommand();
void parsetransmission(String);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 2; i < 14; i++) { //Setup I/O pins 2 through 13, the max number of I/Os we can get
    pinMode(i, OUTPUT); //Configure all avaliable I/Os for output 
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  receivecommand();
  delay(1000);
}

void receivecommand(){ //This function receives the register
    
  // read the incoming byte:
  //String incomingCom = Serial.read(); //The actual receiving of the data!
  String incomingCom = "<CP22GroundCom>100000000000000023"; //A test string to play with
  if (incomingCom.startsWith("<CP22GroundCom>", 0)) { //This if statement confirms the CPSS ground control header
    //Serial.println("Yup!");
    incomingCom.remove(0, 15); //Remove header for additional processing
    parsetransmission(incomingCom);
    }
    else {
      Serial.println("Nope!");
      receivecommand();
      }
       
}

void parsetransmission(String incomingCom){
  String receivedata = incomingCom;
  String checksum = incomingCom;

  receivedata.remove(16); //Isolating the data
  checksum.remove(0, 16); //Isolating the checksum
  checksum.toInt();
  int stuff = 0; //An intermidiate value for the cast from String to uint16_t
  uint16_t data = 0b1000000000000000;
  stuff = receivedata.toInt;
  Serial.println(stuff);
  
}


