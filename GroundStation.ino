#include <FastCRC.h>

/*   _____ _____   _____ _____    _____                           _    _____ _        _   _             
  / ____|  __ \ / ____/ ____|  / ____|                         | |  / ____| |      | | (_)            
 | |    | |__) | (___| (___   | |  __ _ __ ___  _   _ _ __   __| | | (___ | |_ __ _| |_ _  ___  _ __  
 | |    |  ___/ \___ \\___ \  | | |_ | '__/ _ \| | | | '_ \ / _` |  \___ \| __/ _` | __| |/ _ \| '_ \ 
 | |____| |     ____) |___) | | |__| | | | (_) | |_| | | | | (_| |  ____) | || (_| | |_| | (_) | | | |
  \_____|_|    |_____/_____/   \_____|_|  \___/ \__,_|_| |_|\__,_| |_____/ \__\__,_|\__|_|\___/|_| |_|
  Ethan Chao 2017
------------------------------------------------------------------------------------------------------
Ground control to Major Tom...
List of things this program must do ($Tested, #Implemented, %To do):

#Store all button and switch states in a 16 bit register for Arduino pins 2 to 13 
#Calculate a CRC8 checksum of the two bytes (16 bit register) of data using the FastCRC library
#Configure the format of the packet to be sent
%Setup the command confirmation on this end, received from the pad

WARNING: CHECKSUM ISN'T RETURNING CONSISTANT VALUES DUE TO DELAY()
  
*/
                                                                                                                                                                                                       
uint16_t a = 0b1000000000000000; //The 16 bit register I'm using to store the button states to transmit

FastCRC8 CRC8; //Some kind of setup for CRC8 usage
FastCRC16 CRC16; //Setup for CRC16 usage
 
void buttoncheck(void);
void sendcommand(int);

void setup() {
  //Put your setup code here, to run once:
  Serial.begin(9600);
    for (int i = 2; i < 14; i++) { //Setup I/O pins 2 through 13, the max number of I/Os we can get
      pinMode(i, INPUT_PULLUP); //Arduino provides pullups for the buttons. No need to include them in circuit
      //Serial.println(i); //Just checking to make sure the pins are set up!
    }

}

void loop() {
  //Put your main code here, to run repeatedly:
  buttoncheck(); //Run the buttoncheck function to get all button states and store in 16 bit register
  int checksum = CRC8.smbus(a, 16); //Calculation of the checksum, courtesy of the FastCRC library, stored in the "checksum" integer
  //int checksum = CRC16.ccitt(a, 16); //CRC16 test
  //Serial.println(checksum);
  sendcommand(checksum);
  delay(200); //PROBLEM SPOT FOR CHECKSUM

}

void buttoncheck() { //A function that uses a loop to check the state of each button, and stores the state in that 16 bit register I'm using
  for (int i = 2; i < 14; i++) {
    int state = digitalRead(i); //Takes in the state of whatever pin is being covered in the loop

    if (state == LOW) { //Switches are pulled to low when triggered
      a | (10^i); //OR operator to set the bit to one
    }
    else {
      a &=~ (1 << 2); //Really complicated thing to turn the bit to zero
    }
  }
}

void sendcommand(int checksum){
  Serial.print("CP22GroundCom");
  Serial.print(a); //Sends the register
  Serial.print(checksum); //Sends the checksum
  Serial.print("\n");
  //Serial.print("GroundCom Transmission Successful!"); //Just for testing!
}
