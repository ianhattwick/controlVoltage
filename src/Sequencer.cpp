/*


*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "Sequencer.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object. Initialize buffers
*/
Sequencer::Sequencer(uint8_t steps)
{
  _num_steps = steps;
  _end = steps-1;
  _begin=0;
  _cur_step=0;
  _inc = 1;
}


//retrieves the value at the current step
int16_t Sequencer::get(){
  return _values[_cur_step];
}
//return the value at a specific step
int16_t Sequencer::get(uint8_t step){ return _values[constrain(step,0,_num_steps-1)];}
// int16_t Sequencer::get(uint16_t step){ return _values[constrain(step,0,_num_steps-1)];}
// int16_t Sequencer::get(int16_t step){ return _values[constrain(step,0,_num_steps-1)];}

//returns the value then updates _cur_step
int16_t Sequencer::trigger(){
  if(!resetFlag) incIndex();
  else resetFlag = 0;
  return _values[_cur_step];
}

uint8_t Sequencer::getCurStep(){ return _cur_step;}


//resets the sequence to a position
void Sequencer::reset(){ _cur_step = _begin; resetFlag = 1;}
void Sequencer::reset(uint8_t step){ _cur_step = checkRange(step,_begin,_end); resetFlag = 1;}

//specifies the beginning and ending of the sequence
uint8_t Sequencer::range(uint8_t begin, uint8_t end){
  //Serial.println("input, begin " + String(begin) + " end " + String(end));
  if(begin > end){//reverse direction
    _inc = -1;
    _begin = constrain(end,0,_num_steps-1);
    _end = constrain(begin,0,_num_steps-1);
  } else { //normal direction
    _inc = 1;
    _begin = constrain(begin,0,_num_steps-1);
    _end = constrain(end,0,_num_steps-1);
  }
  if( _cur_step > _end) _cur_step = _end;
  else if ( _cur_step < _begin ) _cur_step = _begin;
  //Serial.println("output, begin " + String(_begin) + " end " + String(_end));
  return _end - _begin;
}
void Sequencer::reverse(){ if (_inc > 0) _inc*= -1; }
void Sequencer::forward(){ if (_inc < 0) _inc*= -1;}
void Sequencer::stepSize(uint8_t step){ _inc = step;}

//copy the whole sequence to an array
uint8_t Sequencer::getArray(int16_t *arr, uint8_t size){
  int i=0;
  for(i=0;i<size;i++) {
    if(i<_num_steps-1) arr[i] = _values[i];
  }
  return i;
}

//set the value of a single step
void Sequencer::set(uint8_t step, int16_t val){ 
  _values[ constrain(step, 0, _num_steps-1) ] = val;
}

//fill the array with a sequence
//can be a single value
void Sequencer::fill(int16_t val){
  for(int i=0;i<_num_steps-1;i++) _values[i]=val;
}
//an array
void Sequencer::fill(int16_t *arr, uint8_t size){
  size = constrain(size,0,_num_steps-1);
  for(int i=0;i<_num_steps;i++) {
    _values[i]=arr[i%size];
  }
}
//or an array starting at an aribtrary index
void Sequencer::fill(int16_t *arr, uint8_t size, uint8_t offset){
  size = constrain(size,0,_num_steps-1);
  for(int i=0;i<_num_steps;i++) {
    uint8_t curStep = (i+offset)%_num_steps;
    _values[curStep]=arr[i%size];
  }
}

//writes an array into the sequence
//with optional offset
void Sequencer::setArray(int16_t *arr, uint8_t size){
  size = constrain(size,0,_num_steps-1);
  for(int i=0;i<size;i++) {
    _values[i]=arr[i];
  }
}
void Sequencer::setArray(int16_t *arr, uint8_t size, uint8_t offset){
  size = constrain(size,0,_num_steps-1);
  for(int i=0;i<size;i++) {
    uint8_t curStep = (i+offset)%_num_steps;
    _values[curStep]=arr[i];
  }
}
void Sequencer::setArray(int16_t *arr, int16_t size){ setArray(arr, (uint8_t) size); }
void Sequencer::setArray(int16_t *arr, int16_t size, uint8_t offset) {setArray(arr, (uint8_t) size, offset); }


uint8_t Sequencer::endOfCycle(){ return _cur_step == _end; }  //end of cycle
uint8_t Sequencer::startOfCycle(){ return _cur_step == _begin; } //start of cycle


/* _____PRIVATE FUNCTIONS_____________________________________________________ */

void Sequencer::incIndex(){
  _cur_step += _inc;
  //Serial.println("index " + String(_cur_step));
  _cur_step = checkRange(_cur_step, _begin, _end);
  //Serial.println("index2 " + String(_cur_step));
}

uint8_t Sequencer::checkRange(uint8_t val, uint8_t low, uint8_t high){
  //Serial.println("range " + String(val));
  while( val < low || val > high){
    if(val > high) val -= ( high - low + 1);
    if(val < low) val += ( high - low + 1);
  }
  return val;
}
/*

*/



