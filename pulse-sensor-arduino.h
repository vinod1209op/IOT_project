#ifndef PULSE_SENSOR_ARDUINO_H
#define PULSE_SENSOR_ARDUINO_H

#include "Arduino.h"

class PulseSensor
{
public:
  static void begin(int pulsePin);

  static volatile int BPM;           // used to hold the pulse rate
  static volatile int Signal;        // holds the incoming raw data
  static volatile int IBI;           // holds the time between beats, must be seeded!
  static volatile boolean QS;        // becomes true when Arduoino finds a beat.
};

#endif // PULSE_SENSOR_ARDUINO_H
