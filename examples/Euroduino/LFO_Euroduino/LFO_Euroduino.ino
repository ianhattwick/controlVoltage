/* 
Basic use of controlVoltage library to implement an LFO
Ian Hattwick
Oct 15 2021

Note that opening the serial monitor/plotter may cause slow processing. . . 
- This sketch written for using with the Euroduino module

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
controlVoltage chan1 = controlVoltage(8); //trigger to keep track of time
LFO lfo = LFO(TRI, 4, 255); //waveform, freq (Hz), amplitude

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  Serial.println("controlVoltage LFO example");
}

void loop(){
  //call the controlVoltage object's loop
  chan1.loop();
  lfo.loop();

  //generate cv signals at control rate
  static uint32_t controlTimer = 0;
  int controlInterval = 100;
  if(millis()-controlTimer>controlInterval){
    controlTimer = millis();

    chan1.trigger();
    
    //we don't need to do anything for our lfo here, but can change parameters if we want
    if(1){
      static int LFO_shape = 0;
      int temp = analogRead(A2);
      if( abs(temp - LFO_shape) > 2){
        LFO_shape = temp;
        lfo.shape ( (float)LFO_shape/1024 );
        //Serial.println("LFO_shape " + String(LFO_shape));
      }
      //here we will change the frequency of our LFO
      static int LFO_rate = 4;
      temp = analogRead(A3);
      if( abs(temp - LFO_rate) > 2 ){
        LFO_rate = temp;
        float temp2 =  pow((float)LFO_rate/1024,2) * 20;
        lfo.freq( temp2 );
        //Serial.println("LFO_rate " + String( temp2 ));
      }
    }
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer >= signalInterval){
    signalTimer = millis();

    analogWrite(5, lfo.get() );
    digitalWrite(3, chan1.get() );

//  Serial.print(lfo.get() );
//  Serial.print(",");
//    Serial.println(chan1.get() );
  }
}
