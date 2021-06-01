/*
  modularCV.h - library for generating modular CV signals
  Copyright (c) 2021 Ian Hattwick.  All right reserved.

  This library will be distributed under the MIT License. . . .
*/
#ifndef modularCV_h
#define modularCV_h

#include "Arduino.h"
#include "Sequencer.h"
#include "LFO.h"

class modularCV
{
  public:
    modularCV();
    modularCV(uint8_t bitDepth);

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
    

  private:
    uint8_t _bitDepth = 12;
    uint8_t _state = 0;
    int _outVal = 0;
    uint8_t _updateFlag = 1;

    
    //trigger/gate variables
    uint32_t _triggerTimer = 0;
    uint8_t _trigLen = 3;
    int _gateLen = 100;
    
    //cv variables
    int _prev = 0;
    int _goal = 0;
    uint32_t _lineBegin = 0;
    uint32_t _progress = 0;
    int16_t _riseTime = 0;
    int16_t _fallTime = 0;
    int _lineLength = 100;
    
};
#endif
