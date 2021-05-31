/* 
Basic use of modularCV library to implement a quantized CV
- syntax uses 4 octaves of MIDI notes, range from 24 to 72
- no internal DAC implementation; you are responsible for scaling output for your DAC!
- CV values are stored:
   1V = 1000
   1 midi step = 1000/12

Also demonstrates an attack decay envelope with variable curve on chan2

Monitor in Arduino's serial plotter
*/

#include <modularCV.h>

modularCV chan1 = modularCV(); //quantized CV
modularCV chan2 = modularCV(); //attack release envelope

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  chan2.curve(1.8); //change shape of envelope
}



void loop(){

  static int val = 0;
  static int phasor=0;
  
  chan1.loop();
  chan2.loop();

  //write a midi value to chan1 CV
  //and an AR envelope to chan2

  static uint32_t controlTimer = 0;
  int interval = 150;
  if(millis()-controlTimer>interval){
    controlTimer=millis();

    //basically acting as a sample and hold
    static int val = 0;
    static int phasor = 0;
    int scale[] = {0,2,4,5,7,9,11,12};
    val = sin(phasor/3)*3+3;
    phasor +=18;
    
    chan1.midi(scale[val] + 24);
    chan2.AR( 20, 100);
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer>=signalInterval){
    signalTimer=millis();
    Serial.print("quantized:"),
    Serial.print(chan1.get());
    Serial.print(",");
    Serial.print("envelope:"),
    Serial.println(chan2.get());
  }
}
