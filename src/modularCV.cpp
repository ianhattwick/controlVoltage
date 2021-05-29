/*


*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "modularCV.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object. Initialize buffers
*/
modularCV::modularCV()
{

}

void modularCV::setup() {
  
}


void modularCV::loop() {
  if(millis()-_timer > _loopInterval){
  switch(_state){
    case 0: //no activity
    break;

    case 1: //in trigger delay
    if(millis() - _triggerTimer > _trigLen){
      _outVal = 0;
      _state = 0;
    }
    break;

    case 2: //in gate delay
    if(millis() - _triggerTimer > _gateLen){
      _outVal = 0;
      _state = 0;
    }
    break;

    case 3: //smooth CV
      _progress = millis() - _lineBegin;
     //Serial.println(" progress: " + String(_progress) + " " + String(_progress/_lineLength));
      if(_progress > _lineLength){
        _outVal = _goal;
        _state = 0;
        break;
      } else{
        _outVal = (float)(_goal - _prev) * ((float)_progress/_lineLength) + _prev;
      }
      break;
    }//switch
    //Serial.println("state" + String(_state));
  }//timer
}

void modularCV::trigger() {
  _outVal = 4095;
  _triggerTimer = millis();
  _state = 1;
}

void modularCV::gate(int val) {
  if( val > 0 ) _outVal = 4095;
  else _outVal = 0;
  _state = 0;
}

void modularCV::gate(int val, int gateLen) {
  if( val > 0 ) _outVal = 4095;
  else _outVal = 0;
  _gateLen = gateLen;
  _state = 2;
}

void modularCV::cv(int val){
  _prev = _outVal;
  _goal = val;
  _state = 3;
}

void modularCV::cv(int val, int lineTime){
  _prev = _outVal;
  _goal = val;
  _state = 3;
  _lineLength = lineTime;
  _lineBegin = millis();
}

void modularCV::midi(byte _val){
  byte val = _val<24 ? 24 : _val > 72 ? 72 : _val;

  _outVal = (val-24) * (4000/48);
  _state = 0;
}

int modularCV::get(){
  return _outVal;
}

byte modularCV::available(){
  return _available;
}


/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/*
Get current values (input register and eeprom) of modularCV
*/
