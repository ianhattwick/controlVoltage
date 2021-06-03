/* 
Basic use of controlVoltage library to implement a sequencer and trigger
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

#include <controlVoltage.h>

controlVoltage chan1; //trigger signal
controlVoltage chan2; //CV from sequencer
Sequencer seq(16); //16-step sequencer

//define DAC
//esp32 DAc pins are 25 and and 26
const byte dac1 = 25;
const byte dac2 = 26;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  for(int i=0;i<16;i++){
    seq.set(i,i); //fill sequencer
  }
  Serial.println("setup complete");

  chan1.bitDepth( 8 );
  chan2.bitDepth( 8 );
}

void loop() {

  chan1.loop(); 
  chan2.loop();
  
 //generate a trigger signal
 //trigger sequencer
 //and update CV with current seq value
  static uint32_t controlTimer = 0;
  int interval = 150;
  if(millis()-controlTimer>interval){
    controlTimer=millis();

    static int val = 0;
    chan1.trigger();
    chan2.midi( seq.trigger() + 24 ); //trigger() returns the current seq value and updates seq step

    //every time we reach the end of the cycle we will change the range 
    //of the sequencer
    if ( seq.endOfCycle() ){
      static byte start = 0;
      static byte finish = 15;
      if (start>15) start = 0;
      if( finish > 15) finish = 15;
      seq.range( start++, finish--);
    }
  }//control loop


  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if( millis()-signalTimer >= signalInterval){
    signalTimer=millis();
    
    uint16_t trigVal = chan1.get();
    uint16_t seqVal = chan2.get();

    seqVal = seqVal / (12); //converting to 8-bit quantized 
    
    dacWrite(DAC1, trigVal );
    dacWrite(DAC2, seqVal );
    
    signalTimer = millis();
    Serial.print("cv:");
    Serial.print( trigVal);
    Serial.print(",");
    Serial.print("trig:");
    Serial.println( seqVal );
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
