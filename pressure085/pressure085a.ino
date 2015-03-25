#include <SFE_BMP180.h>
#include <Wire.h>


SFE_BMP180 pressure;

#define ALTITUDE 1655.0 

// ------ for Serial Command library

#include <SerialCommand.h>

#define arduinoLED 13      
SerialCommand sCmd(Serial); 

//---------




int samplingPeriod=1; // the number of seconds between measurements

String status="";


void setup()
{
  
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",          command_LED_on);          // Turns LED on
  sCmd.addCommand("OFF",         command_LED_off);         // Turns LED off
  //sCmd.addCommand("INIT_BMP",         command_INIT_BMP);         // Turns LED off
  sCmd.setDefaultHandler(command_unrecognized);           // Handler for command that isn't matched  (says "What?")
  //startup comm
  Serial.begin(9600);
  
  
 
  
           
}

void loop()
{
  //turn on the 555 system
  //digitalWrite(fivefivefive,HIGH); //turns on the 555 timer subcircuit
  sCmd.readSerial();     // We don't do much, just process serial commands
}

//------------------------------------------------------------------------------
// serial command handlers
void command_LED_on(SerialCommand this_scmd) {
  this_scmd.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}

void command_LED_off(SerialCommand this_scmd) {
  this_scmd.println("LED off");
  digitalWrite(arduinoLED, LOW);
}


// This gets set as the default handler, and gets called when no other command matches.
void command_unrecognized(const char *command, SerialCommand this_scmd) {
  this_scmd.print("***ERROR *** Did not recognize \"");
  this_scmd.print(command);
  this_scmd.println("\" as a command.");
}
