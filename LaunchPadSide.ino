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
//uint16_t statedata = 0b1000000000000000; //The 16 bit register I'm using to store the button states to be received
FastCRC8 CRC8; //Some kind of setup for CRC8 usage
void receivecommand();
void parsetransmission(String);
uint16_t parsedata(String);
int parsechecksum(String);
bool datacheck(uint16_t, int);

int lastTime = 0, currentTime = 0, delayTime = 1000; //Delays the loop() for 500 milliseconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 2; i < 14; i++) { //Setup I/O pins 2 through 13, the max number of I/Os we can get
    pinMode(i, OUTPUT); //Configure all avaliable I/Os for output 
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
  currentTime = millis(); //This whole block here is a delay
  if(currentTime - lastTime >= delayTime) {
    receivecommand();
    lastTime = currentTime;
  }
}

void receivecommand(){ //This function receives the register
    
  //String incomingCom = Serial.read(); // The actual receiving of the data!
  String incomingCom = "<CP22GroundCom>32768234"; // A test string to play with
  if (incomingCom.startsWith("<CP22GroundCom>", 0)) { // This if statement confirms the CPSS ground control header
    //Serial.println("Yup!");
    incomingCom.remove(0, 15); // Remove packet header for additional processing
    parsetransmission(incomingCom);
    }
    else { // If didn't receive tag, try again
      //Serial.println("Nope!");
      //delay(1000);
      receivecommand();
      }
       
}

// String -> Void
// Takes in the data+checksum, then runs parse and checksum functions
void parsetransmission(String incomingCom){
 
  int receivechecksum;
  

  uint16_t receivedata = parsedata(incomingCom); // Extract that data!
  //Serial.println(receivedata, BIN);
  int receivecheck = parsechecksum(incomingCom); // Extract that checksum!
  bool checkcheck = datacheck(receivedata, receivecheck);
  Serial.println(checkcheck);

}
// String -> int
// Removes the last two index positions of the String where the checksum is, and return the data as a 16 bit unsigned integer
uint16_t parsedata(String incomingCom) {
  // In the example, 32768 is the data
  String rawdata = incomingCom;
  int datalength = rawdata.length();
  datalength -= 3;
  rawdata.remove(datalength); // Delete the last two digits
  int intdata = rawdata.toInt(); // Convert the data to int
  //Serial.println(rawdata);
  uint16_t inunsighned;
  inunsighned = (uint16_t) intdata; // Cast int to unsigned 16 bit integer
  //Serial.println(inunsighned, BIN);
  return inunsighned;
  
}

// String -> int
// Removes everything before the last two index positions in the string object and returns it as an integer
int parsechecksum(String incomingCom) {
  // In the example, 234 is the checksum
  String rawstuff = incomingCom;
  int datalength = incomingCom.length();
  datalength -= 3;
  rawstuff.remove(0, datalength); // Remove everything before the last two index positions
  int sumdata = rawstuff.toInt();
  //Serial.println(sumdata);
  return sumdata;
}

// Int, Int -> bool
// Verifies the data with the checksum
bool datacheck(uint16_t indata, int inchecksum) {
  //int checksum = CRC8.smbus(indata, 16);
  //Serial.println(checksum);
  //Serial.println(inchecksum);
  if (CRC8.smbus(indata, 16) == inchecksum) {
    //Serial.println("True");
    return true;
  }
  else {
    //Serial.println("False");
    return false;
  }
}
