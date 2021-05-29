/*
  Sequencer.h - library for generating modular CV signals
  Copyright (c) 2021 Ian Hattwick.  All right reserved.

  This library will be distributed under the MIT License. . . .
*/

#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"


class Sequencer
{
  public:
    Sequencer(uint8_t steps);

    //retrieves the value at the current step
    int16_t get();
    //return value at a specific step
    int16_t get(uint8_t step);
    //returns the value then updates _cur_step
    int16_t trigger();
    //return current step
    uint8_t getCurStep();

    //resets the sequence to a position
    void reset(); //default step 0
    void reset(uint8_t step);

    //specifies the beginning and ending of the sequence
    uint8_t range(uint8_t begin, uint8_t end);
    void reverse();
    void forward();
    void stepSize(uint8_t step);

    //copy the whole sequence to an array
    uint8_t getArray(int *arr, uint8_t size);

    //set the value of a single step
    void set(uint8_t step, int16_t val);

    //fill the array with a sequence
    //can be a single value
    void fill(int16_t val);
    //an array
    void fill(int16_t *arr, uint8_t size);
    //or an array starting at an aribtrary index
    void fill(int16_t *arr, uint8_t size, uint8_t offset);

    //writes an array into the sequence
    //with optional offset
    void setArray(int16_t *arr, uint8_t size);
    void setArray(int16_t *arr, uint8_t size, uint8_t offset);

    uint8_t endOfCycle();  //end of cycle
    uint8_t startOfCycle(); //start of cycle


  private:
    void incIndex();
    uint8_t checkRange(uint8_t val, uint8_t low, uint8_t high);

    int16_t _values[256];
    uint8_t _num_steps;
    uint8_t _cur_step = 0;
    uint8_t _begin = 0;
    uint8_t _end;
    int8_t _inc = 1; //amount to add to _cur_step when triggered

    uint8_t resetFlag = 0;
  
  
};
#endif
