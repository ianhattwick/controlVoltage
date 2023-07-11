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
  _peakVal = (1<<_bitDepth) - 1;
}

void controlVoltage::setup() {
  
}


void controlVoltage::loop() {
  //Serial.println("loop /t state " + String(_state));
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
        if(_goal == _outVal){
          _state = 0;
        } else if( _goal > _outVal){ 
          if( _goal - _outVal < _riseInc){
            _outVal = _goal;
            _state = 0;
          } else _outVal += _riseInc;
        } else {
          if( _outVal - _goal < _fallInc){
            _outVal = _goal;
            _state = 0;
          } else  _outVal -= _fallInc;
        }
        // Serial.println("case 3 " + 
        //   String(_outVal) + " " + 
        //   String(_riseInc));
        break;

      case 4: //AR attack
        _outVal += _riseInc;
        if((_32bit - _outVal) < _riseInc){
          _state = 5;
          _goal = 0;
        }

        break;

      case 5: //AR decay
        _outVal -= _fallInc;
        if( _outVal < _fallInc ){
          _outVal = 0;
          _state = 0;
        }

        break;

    }//switch
    //Serial.println("state" + String(_state));

    _updateFlag = 0;
  }

}

void controlVoltage::trigger() {
  _outVal = _32bit;
  _triggerTimer = millis();
  _state = 1;
}

void controlVoltage::gate(int val) {
  if( val > 0 ) _outVal = _32bit;
  else _outVal = 0;
  _state = 0;
}

// void controlVoltage::timedGate(int gateLen) {
//   _outVal = _32bit;
//   _gateLen = gateLen;
//   _triggerTimer = millis();
//   _state = 2;
// }

void controlVoltage::timedGate(int gateLen) {
_outVal = (1<<_bitDepth) - 1;
_gateLen = gateLen;
_triggerTimer = millis();
_state = 2;
}

// void controlVoltage::gate(int val, int gateLen) {
//   if( val > 0 ) _outVal = (1<<_bitDepth) - 1;
//   else _outVal = 0;
//   _gateLen = gateLen;
//   _state = 2;
// }

void controlVoltage::cv(int val){
  if( val > _peakVal) val = _peakVal;
  _outVal = val << (32 - _bitDepth);
  _goal = val << (32 - _bitDepth);
  _state = 3;
}

void controlVoltage::cv(int val, int lineTime){
  if( val > _peakVal) val = _peakVal;
  _goal = ((uint32_t)val) << (32 - _bitDepth);
  _state = 3;
  if( _goal > _outVal) _riseInc = ((_goal - _outVal) / lineTime)  * (1000/sampleRate);
  else _fallInc = ((_outVal - _goal) / lineTime)  * (1000/sampleRate);
  // Serial.println("cv: " + String(_riseInc) + " " +
  //   String(_fallInc) + " " +
  //   String(_goal) + " " +
  //   String(_outVal) + " " + 
  //   String(_peakVal) + " " +
  //   String(val));
}

void controlVoltage::cv(int val, int attack, int decay){
  if( val > (1<<_bitDepth) - 1) val = (1<<_bitDepth) - 1;
  _goal = val;
  _state = 3;
  if( _goal > _outVal) _riseInc = ((_goal - _outVal) / attack)  * (1000/sampleRate);
  else _fallInc = ((_outVal - _goal) / decay)  * (1000/sampleRate);
}

void controlVoltage::midi(byte _val){
  byte val = _val < 24 ? 24 : _val > 72 ? 72 : _val;
  _outVal = val;
  _state = 0;
  _riseInc = 0;
  _fallInc = 0;
}

void controlVoltage::AR(int attack, int decay){
  _outVal = 0;
  _state = 4;
  _riseInc = ( _32bit / attack) * (1000/sampleRate);
  _fallInc = (_32bit / decay)  * (1000/sampleRate);
  //Serial.println(String(_peakVal) + " " + String(_riseInc) + " " + String(sampleRate));
  //Serial.println("attack release: " + String(_riseInc) + " " + String(_fallInc));
}

int controlVoltage::get(){
  _updateFlag = 1;
  uint32_t returnVal = _outVal >> (32 - _bitDepth );

  if(_curve != 1.){
    float val = (float)returnVal / _peakVal;
    // Serial.print(_outVal);
    // Serial.print(" ");
    // Serial.print(val);
    // Serial.print(" ");
    val = pow(val, _curve);
    //Serial.println(val);
    returnVal = val * _peakVal;
    }
  return returnVal;
}

void controlVoltage::curve(float val){
  _curve = constrain( val, 0.01, 10.);
}

void controlVoltage::bitDepth( byte depth){ _bitDepth = depth; }

void controlVoltage::riseTime(int16_t val){ _riseInc = (_32bit / val) * (1000/sampleRate);}
void controlVoltage::fallTime(int16_t val){ _fallInc = (_32bit / val) * (1000/sampleRate);}



/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/*
Get current values (input register and eeprom) of controlVoltage
*/
