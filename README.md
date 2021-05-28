# modularCV

A simple library for generating CV to control modular synthesizers. This library does not send data diretly to a DAC, but instead you are expected to poll the library within your code and choose your DAC output yourself. 

Currently I am using the MCP4728 4-channel DAC, using the library https://github.com/BenoitSchillings/mcp4728.

But you should be able to use the onboard DAC of an ESP32 etc. 

The core library gives you the ability to do the following:
* trigger() - generate a trigger signal
* gate(val, length (optional)) - generate a gate signal, either with a fixed gate length or you can manually set the gate low
* cv(val, (optional) rampTime) - output a CV signal, with the ability to set a ramp time to smooth the transition between values
* midi (num, (optional) rampTime) - output a quantized CV called with a MIDI note number. Optional rampTime to enable glide

## future plans

Standalone classes for generating typical modular CV functions, including
* LFOs
* Sequencers
* Envelopes

My goal is to keep everything as minimal as possible. Since we are working with CV and not audio I am avoiding the use of interrupts and choosing to use millis() as the primary timekeeping mechanism.
