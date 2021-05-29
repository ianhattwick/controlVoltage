/* 
Basic use of modularCV library to implement a trigger and ramp CV
- trigger duration is 3ms by default
- the ramp CV is updated at the same time as the trigger (100ms interval)
- uses cv(val, rampTime) to smooth the transition between CV values
- basically, a slew limiter
   
Monitor in Arduino's serial plotter
*/

#include <modularCV.h>

modularCV chan1 = modularCV(); //CV
modularCV chan2 = modularCV(); //trigger

void setup()
{
  Serial.begin(115200);  // initialize serial interface for print()
  }



void loop(){
 
  chan1.loop(); //loop has an internal 1kHz timer
  chan2.loop();
  
  writeTrigger();

  //monitor output
  static uint32_t timer = 0;
  int interval = 1;
  if(millis()-timer>interval){
    timer=millis();
    Serial.print("cv ");
    Serial.print( chan1.get());
    Serial.print("\ttrig ");
    Serial.println( chan2.get());

  }

}

//generate a trigger signal
//and update CV ramp
void writeTrigger(){
  static uint32_t timer = 0;
  int interval = 100;
  static int val = 0;

  if(millis()-timer>interval){
    timer=millis();
    chan2.trigger();

    chan1.cv(val,interval);
    val+=100;
    if(val > 4000) val = 0;
  }
}
