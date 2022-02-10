/*


*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "LFO.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object. Initialize buffers
*/
LFO::LFO(){ init(TRI, 1., 4095); }
LFO::LFO(WAVE_SHAPE wave) { init(wave, 1., 4095); }
LFO::LFO(WAVE_SHAPE wave, float freq) { init(wave, freq, 4095); }
LFO::LFO(WAVE_SHAPE wave, float freq, int16_t amplitude) { init(wave, freq, amplitude); }

void LFO::init(WAVE_SHAPE wave, float freq, int16_t amplitude){
  waveform = wave;
  _freq = freq;
  _amplitude = amplitude;
  updateDivisor();
  //Serial.println(_divisor);
  calcIncrement();
}

//retrieves the value at the current step
uint16_t LFO::get(){
  _updateFlag = 1;
  return (uint16_t) _outVal;
}

//retrieves the value at the current step without triggering a new sample
uint16_t LFO::peek(){
  return (uint16_t) _outVal;

}

//main loop
void LFO::loop(){
  if( _updateFlag ){
    _updateFlag = 0;
    //Serial.print(_incA);
    if( _freq != _prevFreq) calcIncrement();
    //Serial.println("loop");
    _prev = _acc;
    if( _acc < (int32_t)1<<15 ) _acc += _incA;
    else _acc += _incB;
    //_acc += _inc;
    if( _acc < _prev ) _EOC = 1;

    // _skew = _acc;

    //todo: this is the wrong way to do skew
    //skew should scale _inc value
    //but needs to not change frequency of LFO. . .
    // if( shape != 0.5){
    //   uint32_t val = shape * (1<<16);
    //   if(_skew < val){
    //     _skew = scale(_skew, 0, val, 0, 1<<15 );
    //   } else {
    //     _skew = scale(_skew, val, 1<<16, 1<<15, 1<<16);
    //   }
    // }

    
    switch( waveform ){
      case TRI: 
      case TRIANGLE:
      if( _acc < (uint32_t)1 << (15 )) _outVal = _acc*2;
      else _outVal = (((uint32_t)1 << 16)  - _acc ) * 2;
      break;

      case SQUARE:
      case RECT: 
      case PULSE:
      _outVal = (_acc > ((uint32_t)1 <<  15 )) * ((1<<16)-1);
      break;

      case RAMP:
      _outVal = _acc;
      break;

      case SAW:
      _outVal = ( (uint32_t)1 << 16 ) - _acc;
      break;

      case SINE:
      case SIN:
      float curInc = ((float)_acc/((uint32_t)1 << 16 )+0.25) * TWO_PI;
      _outVal = (sin(curInc) + 1) * ( (uint32_t)1 <<  15 );
      break;
    }//waveform switch

    _outVal =  _outVal / _divisor;
  }//updateFlag
}

uint8_t LFO::endOfCycle() {
  switch(_EOC){
    case 0:
    break;

    case 1:
    _EOC = 0;
    return 1;
    break;
  }
  return _EOC;
}

void LFO::freq(float frequency){
  _freq = frequency;
  calcIncrement();
}

void LFO::amplitude(uint32_t amp){
  _amplitude= amp;
  updateDivisor();
}

void LFO::bitDepth(byte depth){
  _bitDepth = depth;
  updateDivisor();
}

void LFO::reset(){
  _acc = 0;
}

void LFO::reset(float phase){
  _acc = (uint16_t)phase * (1<<16);
}




/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/*
increment calculation, assuming the update rate is 1kHz
1000/freq = period of waveform
samplingRate/period = fraction of period per sample
increment = fraction * bitDepth

10HZ = 100ms period
1kHz sampling rate = 1/100 of period per sample
amplitude = 65536
increment = 655536/100
*/
void LFO::calcIncrement(){
  _prevFreq = _freq;
  int period = 1000./ _freq;
  _inc = ((uint32_t)((uint32_t)1 << ( 16 ) ) / samplingRate) * _freq;
  //Serial.println(5);
  if( _shape == 0 ) {
    _incA = _inc;
    _incB = _inc;
  }else { //calc skew
    byte dir = _shape > 0.;
    float scalar = pow(1/_shape,2);
    if( dir ) {
      _incA = scalar/(scalar-1) * _inc;
      _incB = scalar/(scalar+1) * _inc;
    } else{
      _incB = scalar/(scalar-1) * _inc;
      _incA = scalar/(scalar+1) * _inc;
    }

  }
  //Serial.println("inc " + String(_inc));
}

//((1<<32) / (1<<20)) * ((1<<12 )/ 1500)
//((1<<20)) /(2000/(1<<12))
void LFO::updateDivisor(){
  _bitDivisor = (uint32_t)1 << (16 - _bitDepth);
  if( _amplitude  > (uint32_t)1 << _bitDepth) _amplitude = (uint32_t)1 << _bitDepth; //check amp is in range
  float scalar = (float)_amplitude / ((uint32_t)1 << _bitDepth);
  _divisor = (float)_bitDivisor / scalar;
  //Serial.println("divisor " + String(_divisor));
}

uint32_t LFO::scale(uint32_t in, uint32_t inLow, uint32_t inHigh, uint32_t outLow, uint32_t outHigh ){
  float val = (float)(in - inLow) / (inHigh - inLow);
  val = val * (outHigh - outLow) + outLow;
  return (uint32_t) val;
}

void LFO::shape(float skew){
  if (skew >= 0.99) skew = 0.99;
  else if (skew <= -0.99) skew = -0.99;
  _shape = skew;
  calcIncrement();
}


/*

*/



