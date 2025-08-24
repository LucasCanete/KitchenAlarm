#include "RotaryEncoder.h"


#if 1
RotaryEncoder::RotaryEncoder(volatile uint8_t *buttonDDR, uint8_t buttonPin, volatile uint8_t *DTDDR, uint8_t DTPin, volatile uint8_t *CLKDDR, uint8_t CLKPin):
buttonDDR_(buttonDDR), buttonPin_(buttonPin), DTDDR_(DTDDR), DTPin_(DTPin), CLKDDR_(CLKDDR), CLKPin_(CLKPin){

}


void RotaryEncoder::init(){

  //button, dt and clk as inputs
  *buttonDDR_ &= ~(1 << buttonPin_);
  *DTDDR_ &= ~(1 << DTPin_);
  *CLKDDR_ &= ~(1<< CLKPin_);

  //assign ports from data direction register
  buttonPort_ = buttonDDR_ - 1;
  CLKPort_ = CLKDDR_ - 1;
  DTPort_ = DTDDR_ - 1;

}

//Rising EDGE
rotary_direction_t RotaryEncoder::getDirection() {
  currentclkstate = *CLKPort_ & (1 << CLKPin_);

  rotary_direction_t direction = NONE;

  if (currentclkstate != lastclkstate &&currentclkstate==0){
    uint8_t DT_State = *DTPort_ & (1 << DTPin_);

    if (DT_State != currentclkstate) direction = COUNTERCLOCKWISE; //CLOCKWISE;

    else if (DT_State == currentclkstate) direction = CLOCKWISE; //COUNTERCLOCKWISE;
  }
  lastclkstate = currentclkstate;
  return direction;
}


uint8_t RotaryEncoder::readButton() const {
  return *buttonPort_ & (1 << buttonPin_);

}

#endif
