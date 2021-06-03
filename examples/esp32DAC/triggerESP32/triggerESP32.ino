/* 
Basic use of controlVoltage library to implement a trigger and ramp CV
- trigger duration is 3ms by default
- the ramp CV is updated at the same time as the trigger (100ms interval)
- uses cv(val, rampTime) to smooth the transition between CV values
- basically, a slew limiter

This example uses the built-in DAC on the ESP32 as output

Monitor in Arduino's serial plotter
*/

#include <controlVoltage.h>

//define cv objects
controlVoltage chan1 = controlVoltage(); //CV
controlVoltage chan2 = controlVoltage(); //trigger

//define DAC
//esp32 DAc pins are 25 and and 26
const byte dac1 = 25;
const byte dac2 = 26;

void setup(){
  Serial.begin(115200);  // initialize serial interface for print()

  Serial.println("controlVoltage trigger example");

  chan1.bitDepth( 8 );
  chan2.bitDepth( 8 );
}



void loop(){
  //call the controlVoltage objects loop
  chan1.loop();
  chan2.loop();
 
  //generate cv signals at control rate
  static uint32_t controlTimer = 0;
  int controlInterval = 400;
  if(millis()-controlTimer>controlInterval){
    controlTimer = millis();

    chan2.trigger();

    static int val = 0;
    chan1.cv(val,200); //ramp to val over 200 ms
    val+=25;
    if(val > 230) val = 0;
  }

  //monitor output at signal rate
  static uint32_t signalTimer = 0;
  int signalInterval = 1;
  if(millis()-signalTimer >= signalInterval){
    uint16_t cvVal = chan1.get();
    uint16_t trigVal = chan2.get();

    dacWrite(DAC1, cvVal );
    dacWrite(DAC2, trigVal );
    
    signalTimer = millis();
    Serial.print("cv:");
    Serial.print( cvVal);
    Serial.print(",");
    Serial.print("trig:");
    Serial.println( trigVal );
  }
}
