#include <SerialCommand.h>

#define arduinoLED 13       // Arduino LED on board
SerialCommand sCmd(Serial); // The SerialCommand object, initialize with any Stream object


int samplingPeriod=1; // the number of seconds to measure 555 oscillations

// number of samples to average
#define DEFAULT_AVENUM 1
#define DEFAULT_SAMPLE_DURATION_MILLIS 1000
//int fivefivefive = 13; // the pin that powers the 555 subcircuit

void setup()
{
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",          command_LED_on);          // Turns LED on
  sCmd.addCommand("OFF",         command_LED_off);         // Turns LED off
  //sCmd.addCommand("MEAS_PULSE_MICROS",  command_MEAS_PULSE_MICROS);     // Measure average pulse period
  sCmd.addCommand("MEAS_PULSE_MICROS",  dummy_MEAS_PULSE_MICROS);     // Measure average pulse period
  sCmd.setDefaultHandler(command_unrecognized);           // Handler for command that isn't matched  (says "What?")
  //startup comm
  Serial.begin(9600);
  
     // pinMode(fivefivefive, OUTPUT);   
           
}

void loop()
{
  //turn on the 555 system
  //digitalWrite(fivefivefive,HIGH); //turns on the 555 timer subcircuit
  sCmd.readSerial();     // We don't do much, just process serial commands
}
//------------------------------------------------------------------------------
// pulse measurement utilities
volatile long pulseCount = 0;  //a pulse counter variable
volatile unsigned long pulseTime,lastTime, duration, totalDuration;

float pulseAve=0.;
int pulseStats=0;


float measureAveragePeriod(unsigned int sampleDuration_millis){
  pulseCount=0;     //reset the pulse counter
  totalDuration=0;  //reset the totalDuration of all pulses measured
  pulseTime=micros(); // start the stopwatch
  attachInterrupt(0,irq_onPulse,RISING); //attach an interrupt counter to interrupt pin 0 (digital pin #2) -- the only other possible pin on the 328p is interrupt pin #1 (digital pin #3)
  //FIXME do NOT use delayMicroseconds, gives weird readings
  delay(sampleDuration_millis); //give ourselves samplingPeriod seconds to make this measurement, during which the "onPulse" function will count up all the pulses, and sum the total time they took as 'totalDuration' 
  detachInterrupt(0); //we've finished sampling, so detach the interrupt function -- don't count any more pulses
  return (totalDuration)/pulseCount;  //returns value as microseconds
}

void irq_onPulse()
{
  pulseCount++;
  //Serial.print("pulsecount=");
  //Serial.println(pulseCount);
  lastTime = pulseTime;
  pulseTime = micros();
  duration=pulseTime-lastTime;
  totalDuration+=duration;
  //Serial.println(totalDuration);
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

void command_MEAS_PULSE_MICROS(SerialCommand this_scmd) {
  int period; 
  int sampleDuration_millis = DEFAULT_SAMPLE_DURATION_MILLIS;
  char *arg = this_scmd.next();
  if (arg != NULL) {
    sampleDuration_millis = atoi(arg);
  }
  period = measureAveragePeriod(sampleDuration_millis);
  //send back result
  this_scmd.println(period);
}

void dummy_MEAS_PULSE_MICROS(SerialCommand this_scmd) {
  int period;
  int sampleDuration_millis = DEFAULT_SAMPLE_DURATION_MILLIS;
  char *arg = this_scmd.next();
  if (arg != NULL) {
    sampleDuration_millis = atoi(arg);
  }
  //simulate delay of period measurement
  delay(sampleDuration_millis);
  //period = measureAveragePeriod(sampleDuration_millis);
  period = 90 + random(0,20);
  //send back result
  this_scmd.println(period);
}

// This gets set as the default handler, and gets called when no other command matches.
void command_unrecognized(const char *command, SerialCommand this_scmd) {
  this_scmd.print("***ERROR *** Did not recognize \"");
  this_scmd.print(command);
  this_scmd.println("\" as a command.");
}
