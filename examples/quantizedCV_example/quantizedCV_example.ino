/* 
Basic use of modularCV library to implement a quantized CV
- syntax uses 4 octaves of MIDI notes, range from 24 to 72
- no internal DAC implementation; you are responsible for scaling output for your DAC!
- CV values are stored:
   1V = 1000
   1 midi step = 1000/12
   
Monitor in Arduino's serial plotter
*/

#include <modularCV.h>

modularCV chan1 = modularCV();

void setup()
{
  Serial.begin(115200);  // initialize serial interface for print()
  }



void loop(){

  static int val = 0;
  static int phasor=0;
  
  chan1.loop(); //loop has an internal 1kHz timer

  writeCV();

  //monitor output
  static uint32_t timer = 0;
  int interval = 10;
  if(millis()-timer>interval){
    timer=millis();
    Serial.println(chan1.get());
  }

}

//write a midi value to chan1 CV
//basically acting as a sample and hold
void writeCV(){
  static uint32_t timer = 0;
  int interval = 100;
  static int val = 0;
  static int phasor = 0;

  if(millis()-timer>interval){

    int scale[] = {0,2,4,5,7,9,11,12};
    timer=millis();
    chan1.midi(scale[val] + 24);

    val = sin(phasor/3)*3+3;
    phasor +=18;  
  }
}
