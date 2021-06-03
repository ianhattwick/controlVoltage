/* 
Basic use of controlVoltage library to implement a quantized CV
- syntax uses 4 octaves of MIDI notes, range from 24 to 72
- no internal DAC implementation; you are responsible for scaling output for your DAC!
- CV values are stored:
   1V = 1000
   1 midi step = 1000/12
- since the esp32 DAC is 8-bit we will have to scale this 1000/octave signal to 1v/octave...

Also demonstrates an attack decay envelope with variable curve on chan2

This example uses the esp32's built in DACs.

Monitor in Arduino's serial plotter
*/

#include <controlVoltage.h>

controlVoltage chan1 = controlVoltage(); //quantized CV
controlVoltage chan2 = controlVoltage(); //attack release envelope

const byte dac1 = 25;
const byte dac2 = 26;

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  chan2.curve(1.8); //change shape of envelope

  chan1.bitDepth( 8 );
  chan2.bitDepth( 8 );
}



void loop(){

  static int val = 0;
  static int phasor=0;
  
  chan1.loop();
  chan2.loop();

  //write a midi value to chan1 CV
  //and an AR envelope to chan2

  static uint32_t controlTimer = 0;
  int interval = 250;
  if(millis()-controlTimer>interval){
    controlTimer=millis();

    static int index = 0;
    int scale[] = {0,2,4,5,7,9,11,12};
    val = scale[index++];
    if(index > 7) index = 0;
   // Serial.println("midi " + String(scale[i) );
    chan1.midi(val + 24);
    chan2.AR( 5, 220);
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer >= signalInterval){
    uint16_t cvVal = chan1.get();
    uint16_t envVal = chan2.get();
    
    //concert 1000/v quantized cv to 8-bits at 3.3v
    float eight_bit_volt = 255/3.3; //1v ~=77
    //cvVal = cvVal / (1000/eight_bit_volt); //this should do the conversion right, BUT
    cvVal = cvVal / (12); //the DAC on the esp32 is not full-range or linear... 12 kinda sounds in tune, but this might need more work...
  
    dacWrite(DAC1, cvVal );
    dacWrite(DAC2, envVal );
    
    signalTimer = millis();
    Serial.print("quant:");
    Serial.print( cvVal);
    Serial.print(",");
    Serial.print("env:");
    Serial.println( envVal );
  }
}
