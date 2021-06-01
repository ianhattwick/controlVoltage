# modularCV

A simple library for generating CV to control modular synthesizers. This library does not send data directly to a DAC, but instead you are expected to poll the library within your code and choose your DAC output yourself. 

Currently I am using the MCP4728 4-channel DAC, using the library https://github.com/BenoitSchillings/mcp4728. But you should be able to use the onboard DAC of an ESP32 etc. 

### Basic functionality

The core library gives you the ability to do generate trigger, gate, quantized CV, slew limiting, AR envelopes, etc.

The Sequencer subclass enables the use of a sequencer with basic functionality

For each CV signal, you must:
1. create a modularCV object, e.g. modularCV chan1 = modularCV();
2. run chan1.loop() inside your main loop
3. generate CV signals by using chan1.trigger(), chan1.AR(0, 100), etc.
4. get the CV output inside your signal loop by calling chan1.get(). This also frees up chan1 to generate a new sample.

## Examples
Examples are provided either with no DAC implementation, or using the MCP4728.
* All examples use the Arduino Serial Plotter to monitor the signal output
* Generally the signal sampling rate is 1kHz

## future plans

Standalone classes for generating typical modular CV functions, including
* LFOs
* Envelopes

My goal is to keep everything as minimal as possible. Since we are working with CV and not audio I am avoiding the use of interrupts and choosing to use millis() as the primary timekeeping mechanism.
