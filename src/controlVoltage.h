/*
  controlVoltage.h - library for generating modular CV signals
  Copyright (c) 2021 Ian Hattwick.  All right reserved.

  This library will be distributed under the MIT License. . . .
*/
#ifndef controlVoltage_h
#define controlVoltage_h

#include "Arduino.h"
#include "Sequencer.h"
#include "LFO.h"

class controlVoltage
{
  public:
    controlVoltage();
    controlVoltage(uint8_t bitDepth);

    void setup();
    void loop();
    void trigger();
    void gate(int val);
    void timedGate(int gateLen);
    //void gate(int val, int gateLen);
    void cv(int val);
    void cv(int val, int lineTime);
    void cv(int val, int attack, int decay);
    void AR(int attack, int decay);
    void midi( byte _val );
    
    int get();
    int peakVolt = 4095;

    void curve(float val);
    float _curve = 1.;

    void bitDepth( byte depth);
    int sampleRate = 1000;
    void riseTime(int16_t val);
    void fallTime(int16_t val);
    

  private:
    uint8_t _bitDepth = 12;
    uint8_t _state = 0;
    uint32_t _outVal = 0;
    uint8_t _updateFlag = 1;
    uint32_t _peakVal = (1<<12) - 1;
    uint32_t _32bit = -1;

    
    //trigger/gate variables
    uint32_t _triggerTimer = 0;
    uint8_t _trigLen = 3;
    int _gateLen = 100;
    
    //cv variables
    uint32_t _goal = 0;
    uint32_t _lineBegin = 0;
    uint32_t _progress = 0;
    uint32_t _riseInc = 0;
    uint32_t _fallInc = 0;
    int _lineLength = 100;
    
};
#endif
