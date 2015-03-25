
// Requires the Arduino-SerialCommand libarary, here: https://github.com/p-v-o-s/Arduino-SerialCommand
// In order to test in the Arduino IDE's serial monitor, make sure to use "Newline" instead of "No line ending"


#include <SerialCommand.h>

#define arduinoLED 13       // Arduino LED on board
SerialCommand sCmd(Serial); // The SerialCommand object, initialize with any Stream object

int LED_STAT=0;

void setup()
{
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",          command_LED_on);          // Turns LED on
  sCmd.addCommand("OFF",         command_LED_off);         // Turns LED off
  sCmd.addCommand("LED_STATUS", command_LED_status);  //returns status of LED
  sCmd.setDefaultHandler(command_unrecognized);           // Handler for command that isn't matched  (says "What?")
  //startup comm
  Serial.begin(9600);
           
}

void loop()
{
  
  sCmd.readSerial();     // We don't do much, just process serial commands
}

// serial command handlers
void command_LED_on(SerialCommand this_scmd) {
  this_scmd.println("LED on");
  digitalWrite(arduinoLED, HIGH);
  LED_STAT=1;
}

void command_LED_off(SerialCommand this_scmd) {
  this_scmd.println("LED off");
  digitalWrite(arduinoLED, LOW);
  LED_STAT=0;
}


void command_LED_status(SerialCommand this_scmd) {
  this_scmd.println(LED_STAT);
}


// This gets set as the default handler, and gets called when no other command matches.
void command_unrecognized(const char *command, SerialCommand this_scmd) {
  this_scmd.print("***ERROR *** Did not recognize \"");
  this_scmd.print(command);
  this_scmd.println("\" as a command.");
}

