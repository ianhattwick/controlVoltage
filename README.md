# controlVoltage

A simple library for generating CV to control modular synthesizers. This library does not send data directly to a DAC, but instead you are expected to poll the library within your code and choose your DAC output yourself. 

Currently I am using the MCP4728 4-channel DAC, using the library https://github.com/BenoitSchillings/mcp4728. But you should be able to use the onboard DAC of an ESP32 etc. 
* Using a 32-bit microcontrollers is highly recommended, as the signal loop will work best at higher sampling rates, 1kHz for example.
* Using any Serial function will slow down your code - ok for debugging and monitoring while doing development but not so good when actually playing. Just FYI!

## Who is this for?

I created this library as I wanted to be able to easily implement basic CV functionality without having to keep track of timers, etc. The plan is for this functionality to be used to explore different kinds of sequencing, using sensors as control input, etc. You could use this to create simple LFOs etc but there are probably better ways of doing that.

I prefer to keep the implementation simple and readable rather than efficient. No uses of timers or interrupts (other than millis()), minimal shorthand, etc.

## Basic functionality

### The core library gives you the ability to generate cv signals: trigger, gate, quantized CV, slew limiting, AR envelopes, etc.

For each CV signal, you must:
1. create a modularCV object, e.g. modularCV chan1 = modularCV();
2. run chan1.loop() inside your main loop
3. generate CV signals by using chan1.trigger(), chan1.AR(0, 100), etc.
4. get the CV output inside your signal loop by calling chan1.get(). This also frees up chan1 to generate a new sample.

### Sequencer
* seq.get() - retrieves the value at the current step  
* seq.get(uint8_t step) - return value at a specific step  
* seq.trigger() - returns the value then updates _cur_step 
* seq.getCurStep() - return current step
    
* seq.reset(); //default step 0
* seq.reset(uint8_t step); //resets the sequence to a position

* seq.range(uint8_t begin, uint8_t end) - specifies the beginning and ending of the sequence
* seq.reverse()
* seq.forward()
* seq.stepSize(uint8_t step)
* seq.getArray(int16_t *arr, uint8_t size) - opy the whole sequence to an array

* seq.set(uint8_t step, int16_t val) - set the value of a single step
    
* fill the array with a sequence  
    * seq.fill(int16_t val)- //can be a single value
    * seq.fill(int16_t *arr, uint8_t size) - //or an array
    * seq.fill(int16_t *arr, uint8_t size, uint8_t offset) - //or an array starting at an aribtrary index

* seq.setArray(int16_t *arr, uint8_t size);
* seq.setArray(int16_t *arr, uint8_t size, uint8_t offset);

* seq.endOfCycle();  //end of cycle
* seq.startOfCycle(); //start of cycle

### LFO
#### LFO methods:
* constructor - all arguments optional
* LFO (waveform, float freq (Hz),int amplitude)
* lfo.freq( float freq (Hz) )
* lfo.amplitude( int amplitude )
* lfo.bitDepth( byte depth ) - bit resolution of the output signal
* lfo.endOfCycle() - returns one after cycle completes, 0 otherwise
* lfo.loop() - generate a new sample once the previous sample is read
* lfo.get() - read and return a sample
* lfo.peek() - read a sample value without triggering a new sample
* lfo.reset(optional float phase) - reset waveform to beginning, or to an arbitrary phase
* lfo.shape(float shape) :default 0. range -1 to 1, skews the waveform. 0. is no skew

#### Properties:
* lfo.waveform : TRI, SINE, SQUARE, RAMP, SAW
* lfo.samplingRate : int, default 1000, HZ, number of samples/second

# Examples
Examples are provided either with no DAC implementation, or using the MCP4728.
* All examples use the Arduino Serial Plotter to monitor the signal output
* Generally the signal sampling rate is 1kHz

# future plans

The basic functionality is finished. Suggestions for additions are welcome, but I'm trying to keep this library minimal so as to be a platform to build on.

Documentation for everything still forthcoming. The examples should give you a sense of what is possible.

