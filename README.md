Teensy Rock Band
================

This is a set of programs that allow a computer to simulate a PlayStation
Rock Band instrument using a Teensy device. The ultimate goal of this
project is to enable the use of the Bluetooth Nintendo Wii Guitar Hero
instruments with Rock Band on the PlayStation 3.


Requirements
------------

The instrument program itself requires avr-gcc and avr-libc to compile. It
also requires a USB programmable board such as a Teensy to run.
The controller application runs on a computer to control the instrument
and requires [Parapin](http://parapin.sourceforge.net/) to compile and run.
The included Wiiuse (v0.12, with a small fix for bluez) also requires a
working bluetooth stack.

**Note that this project controls the device using a PC's parallel port.
The parapin library is used to do this and is only compatible with Linux**

### Hardware Requirements
* Linux computer with a parallel port and bluetooth module
* Teensy device
* A Wiimote and Guitar Hero guitar

Running
-------

Make sure you follow the button maps from both projects (instrument/comm.c
and controller/main.cpp) and directly wire your parallel port to the proper
pins on your Teensy. Run instrument/ on the device, and run controller/ on
your computer, syncing it up to a Wii Guitar Hero guitar.


TODO
----

Here are some cool things that need to happen in the future.
* Hub support for multiple instruments.
* Better PC-Teensy communication for more complex features (whammy bar)
* Tilt-activated Overdrive
* Make drums work
* Add gamepad and keyboard support, rather than only using Wii instruments.
* Support for Windows and other platforms


Credits
-------

This project was written by Aaron Lindsay. Consider it all under the WTFPL.

Many thanks to infnorm for documenting the PS3 USB guitars!
http://sites.google.com/site/infnorm/rbguitartechnicaldetails

