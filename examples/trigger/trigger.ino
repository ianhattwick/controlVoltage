/* 
Basic use of controlVoltage library to implement a trigger and ramp CV
- trigger duration is 3ms by default
- the ramp CV is updated at the same time as the trigger (100ms interval)
- uses cv(val, rampTime) to smooth the transition between CV values
- basically, a slew limiter

Every controlVoltage object has three functions:
- get() returns the current sample
- loop() generates a new sample once the previous sample is read
- some other function to create a CV event. possibilities include:
  - trigger()
  - gate(int val ) - static gate signal, outputs HIGH if val>0 else LOW
  - timedGate( int length ) - HIGH for length ms, then LOW
  - cv(int val) - static cd
  - cv(int val, int rampTime) slew limited CV
  - AR(int attack, int release) - attack release envelope
  - midi( byte val ) - quantized CV output (1000/octave), with val defined as a midi note
   
Monitor in Arduino's serial plotter
*/

#include <controlVoltage.h>

//define cv objects
controlVoltage chan1 = controlVoltage(); //CV
controlVoltage chan2 = controlVoltage(); //trigger

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  Serial.println("controlVoltage trigger example");
}



void loop(){
  //call the controlVoltage objects loop
  chan1.loop();
  chan2.loop();
 
  //generate cv signals at control rate
  static uint32_t controlTimer = 0;
  int controlInterval = 100;
  if(millis()-controlTimer>controlInterval){
    controlTimer = millis();

    chan2.trigger();

    static int val = 0;
    chan1.cv(val,controlInterval); //ramp to val over controlInterval ms
    val+=100;
    if(val > 4000) val = 0;
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer>signalInterval){
    signalTimer = millis();
    Serial.print("cv:");
    Serial.print( chan1.get());
    Serial.print(",");
    Serial.print("trig:");
    Serial.println( chan2.get());
  }
}
