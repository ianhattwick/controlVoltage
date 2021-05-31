/* threeTriggers.ino
 *  Ian Hattwick
 *  May 30, 2021
 *  
 *  Example which generates three triggers signals from a common clock.
 *  Each signal has a subdivider controlled by a potentiometer
 *  
 *  This example uses an mcp4728 - replace the code in dacSetup and the signal loop for your DAC.
*/


#include <Wire.h>
#include "mcp4728.h"
#include "modularCV.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

modularCV trig1 = modularCV();
modularCV trig2 = modularCV();
modularCV trig3 = modularCV();

byte pot1 = 33;
byte pot2 = 32;
byte pot3 = 39;

void setup()
{
  Serial.begin(115200);  // initialize serial interface for print()

  dacSetup();

  //potentiometers
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
 
}


void loop()
{
  trig1.loop();
  trig2.loop();
  trig3.loop();
  
  //control loop
  static uint32_t controlTimer;
  int bpm = 90;
  int subdivisions = 8;
  
  if(millis()-controlTimer > 60*1000 / (bpm * subdivisions) ){
    controlTimer = millis();
    static int clock = 0;
    clock+=1;
    if( clock >= 16) clock = 0;

    //my ADC is 12-bit, so 4095/273 = 15
    //+1 so we don't divide by 0
    int subDivide1 = analogRead(pot1)/273 + 1;
    int subDivide2 = analogRead(pot2)/273 + 1;
    int subDivide3 = analogRead(pot3)/273 + 1;

    if( clock % subDivide1 == 0 ) trig1.trigger();
    if( clock % subDivide2 == 0 ) trig2.trigger();
    if( clock % subDivide3 == 0 ) trig3.trigger();
  }
  
  //1khz signal loop
  static uint32_t signalTimer = 0;
  if(millis()-signalTimer > 0){
    signalTimer = millis();

    dac.analogWrite(1, trig1.get() );
    dac.analogWrite(2, trig2.get() );
    dac.analogWrite(3, trig3.get() );

    monitorCV();
  }
}

  
void monitorCV(){
    Serial.print("trig1:");
    Serial.print(trig1.get() );
    Serial.print(",");
    Serial.print("trig2:");
    Serial.print(trig2.get() );
    Serial.print(",");
    Serial.print("trig3:");
    Serial.println(trig3.get() );
}

  

/*********************************************
 * DAC
 *********************************************/

void dacSetup(){
  if(dac.begin(21,22)) Serial.println("MCP4728 found");  // initialize i2c interface
  else Serial.println("not found");
  
  dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(1,1,1,1); // set to use internal voltage reference (2.048V)
  dac.setGain(1,1,1,1); // set the gain of internal voltage reference ( 0 = gain x1, 1 = gain x2 )
}
