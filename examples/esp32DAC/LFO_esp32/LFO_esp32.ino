/* 
Basic use of controlVoltage library to implement an LFO
Ian Hattwick
May 31, 2021

Note that opening the serial monitor/plotter may cause slow processing. . . 
- 32-bit processor recommended. Tested with an ESP32.

LFO methods:
constructor - all arguments optional
LFO (waveform, float freq (Hz),int amplitude)
lfo.freq( float freq (Hz) )
lfo.amplitude( int amplitude )
lfo.shape(float shape) :default 0. range -1 to 1, skews the waveform. 0. is no skew

lfo.bitDepth( byte depth ) - bit resolution of the output signal
lfo.endOfCycle() - returns one after cycle completes, 0 otherwise
lfo.reset(optional float phase) - reset waveform to beginning, or to an arbitrary phase

lfo.loop() - generate a new sample once the previous sample is read
lfo.get() - read and return a sample
lfo.peek() - read a sample value without triggering a new sample


Properties:
lfo.waveform : TRI, SINE, SQUARE, RAMP, SAW
lfo.samplingRate : int, default 1000, HZ, number of samples/second


Monitor in Arduino's serial plotter
*/

#include <controlVoltage.h>

//define cv objects
LFO lfo1 = LFO(TRI, 4, 255); //waveform, freq (Hz), amplitude
LFO lfo2 = LFO(SIN, 12, 128); //waveform, freq (Hz), amplitude

const byte dac1 = 25;
const byte dac2 = 26;

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  Serial.println("controlVoltage LFO example");

  lfo1.bitDepth( 8 );
  lfo2.bitDepth( 8 );
}

void loop(){
  //call the controlVoltage object's loop
  lfo1.loop();
  lfo2.loop();

  static float shape = 0.;
 
  //generate cv signals at control rate
  static uint32_t controlTimer = 0;
  int controlInterval = 100;
  if(millis()-controlTimer>controlInterval){
    controlTimer = millis();
  
    //we don't need to do anything for our lfo here, but can change parameters if we want
    if(1){
      //here we will change the frequency of our LFO
      if( lfo2.endOfCycle() ){
        static int freq = 4;
        lfo2.freq( freq+=1 );
        if( freq > 20) freq = 1;
      }
    }

    if(1){
      //here we will change the shape/slope/pulsewdith of our LFO
      //note this will introduce discontinuities when changed. . .
      
      lfo1.shape (shape);
      shape += 0.03;
      if(shape>=0.95) shape = -0.95;
      
    }

    if(1){
      static int state = 0;
      static int stateCount = 0;
      //here we will change the shape of our waveforms
      if(state == 0) lfo1.waveform = TRI;
      else if(state == 1) lfo1.waveform = RAMP;
      else if(state == 2) lfo1.waveform = SAW;
      else if(state == 3) lfo1.waveform = SQUARE;
      else if(state == 4) lfo1.waveform = SINE;
      stateCount++;
      if( stateCount >= 200) {
        state++;
        stateCount = 0;
        lfo1.reset();
      }
      if( state >= 5 ) state = 0;
    }
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer >= signalInterval){
    signalTimer = millis();

    uint16_t lfoVal1 = lfo1.get();
    uint16_t lfoVal2 = lfo2.get();

    dacWrite(DAC1, lfoVal1 );
    dacWrite(DAC2, lfoVal2 );
    
    signalTimer = millis();
    Serial.print(shape*100 + 100 );
    Serial.print(",");
    Serial.print( lfoVal1);
    Serial.print(",");
    Serial.println( lfoVal2 );


  }
}
