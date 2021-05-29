/*
  modularCV.h - library for generating modular CV signals
  Copyright (c) 2021 Ian Hattwick.  All right reserved.

  This library will be distributed under the MIT License. . . .
*/
#ifndef modularCV_h
#define modularCV_h

#include "Arduino.h"


class modularCV
{
  public:
    modularCV();

    void setup();
    void loop();
    void trigger();
    void gate(int val);
    void gate(int val, int gateLen);
    void cv(int val);
    void cv(int val, int lineTime);
    void midi( byte _val );
    
    int get();
    byte available();
    int peakVolt = 4095;
    

  private:
    uint32_t _timer = 0;
    byte _loopInterval = 0;
    byte _available = 0;
    
    uint32_t _triggerTimer = 0;
    byte _trigLen = 3;
    int _gateLen = 100;
    byte _state = 0;
    
    int _prev = 0;
    int _goal = 0;
    uint32_t _lineBegin = 0;
    uint32_t _progress = 0;
    int _lineLength = 100;
    int _outVal = 0;
  
    
};
#endif
