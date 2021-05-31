/* 
Example using a genetic algorithm to morph between sequences

Three CV outputs:
1: trigger
2: quantized CV
3: note length CV

Inputs:
pot[0]: evolution rate
pot[1]: note density
pot[2]: pitch range

buttons 0-3 select goal sequence
- short press sets a new goal
- long press saves current sequence as a goal
*/

const byte NUM_STEPS = 12;

#include <Wire.h>
#include "mcp4728.h"
#include "modularCV.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

modularCV cv[3] = {
  modularCV(),
  modularCV(),
  modularCV()
};

Sequencer seq = Sequencer(NUM_STEPS);
Sequencer rhy = Sequencer(NUM_STEPS);

enum buttStates {UP, PUSHED, DOWN, RELEASED};

byte scale[] = {0,3,5,7,10};
byte scaleSize = 5;
byte pitchRange = 24;

void setup()
{
  Serial.begin(115200);  // initialize serial interface for print()

  if(dac.begin(21,22)) Serial.println("MCP4728 found");  // initialize i2c interface
  else Serial.println("not found");
  
  dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(1,1,1,1); // set to use internal voltage reference (2.048V)
  dac.setGain(1,1,1,1); // set the gain of internal voltage reference ( 0 = gain x1, 1 = gain x2 )

  sensorSetup();
  algoSetup();
  rhySetup();
}

byte updateDAC = 0;
int DACval[4];

byte curGoal = 0;

void loop()
{
  sensorLoop();
  for(int i=0;i<3;i++) cv[i].loop() ;
  
  //use buttons to select new goal sequence
  for(int i=0;i<4;i++){
    if( getSw(i) == PUSHED){ 
      curGoal = i;
      //Serial.println("new goal " + String(i));
    }
  }

  //change range of sequencer
  int prevPitchRange = pitchRange;
  pitchRange = getPot(2)/64;
  //if(pitchRange != prevPitchRange) Serial.println("pitchRange " + String(pitchRange));
  
  //control loop
  static uint32_t timer = 0;
  int interval = 175;
  if(millis()-timer>interval){
    timer= millis();
    algoLoop();
    rhyLoop();

    //get pitch CV
    int rawCV = seq.trigger();
    static byte prevQuantized = 0;
    int quantized = quantizeCV(rawCV);
    cv[0].midi(quantized+24);
    if(prevQuantized!= quantized) cv[2].trigger();
    
    prevQuantized = quantized;

    //Get Rhythm CV
    int threshold = getPot(1)*3/4+100;
    int envelope = getPot(1)/32 + 4;
      envelope *= envelope;
      
    if(rhy.trigger() > threshold){
      
     cv[1].AR(0,envelope);
    }
  }

  monitorCV();

  //values are written to the DAC in DACloop()
  DACloop();
}

int quantizeCV(long val){
  int val2 = (int)(val*pitchRange/4095);

  int val3 = scale[val2 % scaleSize] + val2/scaleSize*12;

  //Serial.println("quant " + String(val) + " " + String(val2) + " " + String(val3));
  return val3;
}

/******
 * DAC functions
 * This uses the fastwrite capability of the mcp4728
 * so first we need to store the values to be sent in setDAC
 * and then call DACloop
 ******/
void setDAC(byte channel, int val){
  if( DACval[channel] != val){
    DACval[channel] = val>4095 ? 4095 : val;
    updateDAC = 1;
  }
}

void DACloop(){
  static uint32_t timer = 0;
  int DACinterval = 0;

  if(millis()-timer>DACinterval){
    for(int i=0;i<3;i++) setDAC(i+1, cv[i].get() );
    timer=millis();
    //printf("DAC %d %d %d %d\n", DACval[0], DACval[1], DACval[2], DACval[3]);
    dac.voutWrite(DACval[0], DACval[1], DACval[2], DACval[3]);
  }
}

void printArray(int16_t *array, byte size){
  for(int i=0;i<size;i++){
    Serial.print(String(array[i]) + "\t");
  }
  Serial.println();
}

void monitorCV(){
  static uint32_t timer = 0;
  int interval = 1;

  if(millis()-timer>interval){
    Serial.print("quantizedCV:");
    Serial.print(cv[0].get() );
    Serial.print(",");
    Serial.print("ARenvelope:");
    Serial.print(cv[1].get() );
    Serial.print(",");
    Serial.print("trigger:");
    Serial.print(cv[2].get() );
    Serial.println("");
//    for(int i=0;i<3;i++){
//      //Serial.print(String(i) + " ");
//      Serial.print(cv[i].get());
//      Serial.print(" ");
//    }
//    Serial.println();

//    for(int i=0;i<4;i++){
//      //Serial.print(String(i) + " ");
//      Serial.print(DACval[i]);
//      Serial.print(" ");
//    }
//    Serial.println();
  }
}
