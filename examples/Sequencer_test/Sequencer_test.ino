/* 
Basic use of modularCV library to implement a sequencer and trigger
- sequencer is loaded with 16 steps from 0 to 4v (dependent on DAC)
- remember you need to implement your own DAC output!
- every end of cycle the step range of the sequencer is changed
- this only enables the sequencer steps from (begin to finish)
- note the values in the sequencer are never being changed in this example
- this example uses the sequencers internal index (updated when the sequencer is triggered)
- you could also implement an external index and get the sequencer values by calling:
  seq.get(step to read)
   
Monitor in Arduino's serial plotter
*/

#include <modularCV.h>

modularCV chan1; //trigger signal
modularCV chan2; //CV from sequencer
Sequencer seq(16); //16-step sequencer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  for(int i=0;i<16;i++){
    seq.set(i,i* (4000/15)); //fill with 4V CV
  }
  Serial.println("setup complete");
}

void loop() {

  chan1.loop(); //loop has an internal 1kHz timer
  chan2.loop();
  
  writeTrigger();

  //monitor output
  static uint32_t timer = 0;
  int interval = 1;
  if(millis()-timer>interval){
    timer=millis();
    Serial.print("trig ");
    Serial.print( chan1.get());
    Serial.print(" seq ");
    Serial.println( chan2.get());

  }
}

//generate a trigger signal
//trigger sequencer
//and update CV with current seq value
void writeTrigger(){
  static uint32_t timer = 0;
  int interval = 100;
  static int val = 0;

  if(millis()-timer>interval){
    timer=millis();
    chan1.trigger();
    chan2.cv( seq.trigger() ); //trigger() returns the current seq value

    //every time we reach the end of the cycle we will change the range 
    //of the sequencer
    if ( seq.endOfCycle() ){
      static byte start = 0;
      static byte finish = 15;
      if (start>15) start = 0;
      if( finish == 255) finish = 15;
      seq.range( start++, finish--);
    }
  }
}

//use to print the contents of an array to serial monitor
void printArray(int *array, byte size){
  Serial.print("sequence: ");
  for(int i=0;i<size;i++){
    Serial.print(String(array[i]) + "\t");
  }
  Serial.println();
}
