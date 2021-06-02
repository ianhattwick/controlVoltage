/*
  LFO.h - library for generating modular CV signals
  Copyright (c) 2021 Ian Hattwick.  All right reserved.

  This library will be distributed under the MIT License. . . .
*/

#ifndef LFO_h
#define LFO_h

#include "Arduino.h"


enum WAVE_SHAPE {TRI, TRIANGLE, SQUARE, RAMP, SAW, SINE, SIN, RECT, PULSE};

class LFO
{
  public:

    LFO();
    LFO(WAVE_SHAPE wave);
    LFO(WAVE_SHAPE wave, float freq);
    LFO(WAVE_SHAPE wave, float freq, int16_t amplitude);

    //calculate a new sample
    void loop();

    //retrieves the current value 
    uint16_t get();
    uint16_t peek(); //returns value but doesn't generate a new sample

    uint8_t endOfCycle();  //end of cycle

    int samplingRate = 1000;
    
    WAVE_SHAPE waveform = TRI;

    void freq(float frequency);
    void amplitude(uint32_t amp);
    void bitDepth(byte depth);
    void shape( float skew);
    void reset();
    void reset(float phase);

  private:
    
    void init(WAVE_SHAPE wave, float freq, int16_t amplitude);
    void incIndex();
    uint8_t checkRange(uint8_t val, uint8_t low, uint8_t high);
    void calcIncrement();
    void updateDivisor();
    uint32_t scale(uint32_t in, uint32_t inLow, uint32_t inHigh, uint32_t outLow, uint32_t outHigh );
    

    uint32_t _outVal = 0;
    int _samplingRate = 1000;
    byte _bitDepth = 12;

    uint16_t _acc = 0; //accumulator
    uint16_t _inc = 0; //phase increment
    uint16_t _incA = 0; //phase increment
    uint16_t _incB = 0; //phase increment
    uint16_t _prev = 0; //prev accum value
    uint16_t _skew = 0; //skewed accumulator
    float _shape = 0.; //bipolar, 0 is no alteration of waveform
    float _prevFreq = 1.;

    uint32_t _amplitude = 4095;
    float _freq = 1.;
    uint32_t _bitDivisor = 1<<20;
    uint32_t _divisor = 1<<20;

    byte _EOC; //end of cycle

    uint8_t resetFlag = 0;
    uint8_t _updateFlag = 1;
  
};
#endif
