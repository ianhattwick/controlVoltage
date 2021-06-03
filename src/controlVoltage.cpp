/*


*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "controlVoltage.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object. Initialize buffers
*/
controlVoltage::controlVoltage()
{

}

controlVoltage::controlVoltage(uint8_t bitDepth){
  _bitDepth = bitDepth;
}

void controlVoltage::setup() {
  
}


void controlVoltage::loop() {
  if( _updateFlag ){ //only calc new sample after previous is read
    byte direction;

    switch(_state){
      case 0: //no activity
      //Serial.println("0" + String(_outVal));
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
        direction = _goal > _prev;
        _lineLength = _riseTime+1;
        if(direction == 0) _lineLength = _fallTime+1;
       //Serial.println(" progress: " + String(_progress) + " " + String(_progress/_lineLength));
        if(_progress > _lineLength){
          _outVal = _goal;
          _state = 0;
          break;
        } else{
          _outVal = (float)(_goal - _prev) * ((float)_progress/_lineLength) + _prev;
        }
        break;

      case 4: //AR attack
        _progress = millis() - _lineBegin;
        _lineLength = _riseTime+1;
        if(_progress > _lineLength){
          _outVal = (1<<_bitDepth) - 1;
          _prev = _outVal;
          _lineBegin = millis();
          _state = 5;
          _goal = 0;
          break;
        } else{
          _outVal = (float)(_goal - _prev) * ((float)_progress/_lineLength) + _prev;
        }
        //Serial.println("attack " + String(_progress));
        //Serial.println("att" + String(_outVal));
        break;

      case 5: //AR decay
        _progress = millis() - _lineBegin;
        _lineLength = _fallTime+1;
        if(_progress > _lineLength){
          _outVal = 0;
          _state = 0;
          break;
        } else{
          _outVal = (float)(_goal - _prev) * ((float)_progress/_lineLength) + _prev;
          if( _outVal<0 ) _outVal = 0;
        }
        //Serial.println("rel" + String(_outVal));
        break;

    }//switch
    //Serial.println("state" + String(_state));

    if(_curve != 1.){
        float val = (float)_outVal / ((1<<_bitDepth)-1);
        // Serial.print(_outVal);
        // Serial.print(" ");
        // Serial.print(val);
        // Serial.print(" ");
        val = pow(val, _curve);
        //Serial.println(val);
        _outVal = val * (1<<_bitDepth)-1;
        if( _outVal<0 ) _outVal = 0;
    }
    _updateFlag = 0;
  }

}

void controlVoltage::trigger() {
  _outVal = (1<<_bitDepth) - 1;
  _triggerTimer = millis();
  _state = 1;
}

void controlVoltage::gate(int val) {
  if( val > 0 ) _outVal = (1<<_bitDepth) - 1;
  else _outVal = 0;
  _state = 0;
}

void controlVoltage::timedGate(int gateLen) {
  _outVal = (1<<_bitDepth) - 1;
  _gateLen = gateLen;
  _state = 2;
}

// void controlVoltage::gate(int val, int gateLen) {
//   if( val > 0 ) _outVal = (1<<_bitDepth) - 1;
//   else _outVal = 0;
//   _gateLen = gateLen;
//   _state = 2;
// }

void controlVoltage::cv(int val){
  if( val > (1<<_bitDepth) - 1) val = (1<<_bitDepth) - 1;
  _prev = _outVal;
  _goal = val;
  _state = 3;
  _riseTime = 0;
  _fallTime = 0;
  _lineBegin = millis();
}

void controlVoltage::cv(int val, int lineTime){
  if( val > (1<<_bitDepth) - 1) val = (1<<_bitDepth) - 1;
  _prev = _outVal;
  _goal = val;
  _state = 3;
  _riseTime = lineTime;
  _fallTime = lineTime;
  _lineBegin = millis();
}

void controlVoltage::cv(int val, int attack, int decay){
  if( val > (1<<_bitDepth) - 1) val = (1<<_bitDepth) - 1;
  _prev = _outVal;
  _goal = val;
  _state = 3;
  _riseTime = attack;
  _fallTime = decay;
  _lineBegin = millis();
}

void controlVoltage::midi(byte _val){
  byte val = _val<24 ? 24 : _val > 72 ? 72 : _val;
  _prev = _outVal;
  _goal = (val-24) * (4000/48);
  _state = 3;
  _riseTime = 0;
  _fallTime = 0;
  _lineBegin = millis();
}

void controlVoltage::AR(int attack, int decay){
  _prev = 0;
  _outVal = 0;
  _goal = (1<<_bitDepth) - 1;
  _state = 4;
  _riseTime = attack;
  _fallTime = decay;
  _lineBegin = millis();
}

int controlVoltage::get(){
  _updateFlag = 1;
  return _outVal;
}

void controlVoltage::curve(float val){
  _curve = constrain( val, 0.01, 10.);
}

void controlVoltage::bitDepth( byte depth){ _bitDepth = depth; }


/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/*
Get current values (input register and eeprom) of controlVoltage
*/
