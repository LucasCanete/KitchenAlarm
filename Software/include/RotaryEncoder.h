/*
 4 digits Seven Segment display
 July 2025
 Lucas Canete
*/

#if 1

#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
	CLOCKWISE, //default value 0
	COUNTERCLOCKWISE, //default value 1
	NONE
} rotary_direction_t;

class RotaryEncoder{
    public:
      RotaryEncoder(volatile uint8_t *buttonDDR, uint8_t buttonPin, volatile uint8_t *DTDDR, uint8_t DTPin, volatile uint8_t *CLKDDR, uint8_t CLKPin);

      void init();
      rotary_direction_t getDirection();
      uint8_t readButton() const;

    private:
      uint8_t currentclkstate;
      uint8_t lastclkstate = 0;

      volatile uint8_t *buttonDDR_;
      uint8_t buttonPin_;
      volatile uint8_t *buttonPort_;

      volatile uint8_t *DTDDR_;
      uint8_t DTPin_;
      volatile  uint8_t *DTPort_;

      volatile uint8_t *CLKDDR_;
      uint8_t CLKPin_;
      volatile uint8_t *CLKPort_;

};

#endif
#endif
