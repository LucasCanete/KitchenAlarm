
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "SevenSegmentDisplay.h"
#include "RotaryEncoder.h"
#include "defines.h"


int8_t unit_sec = 0;
int8_t ten_sec = 0;

int8_t unit_min = 0;
int8_t ten_min = 0;

volatile uint16_t COUNTER_TIMEOUT = 0;

namespace ButtonState {
  enum State {
    PRESSED = 0,
    NOT_PRESSED = 1
  };
}
ButtonState::State buttonStatus = ButtonState::NOT_PRESSED;

namespace Alarm {
  enum State {
    SET = 1,
    NOT_SET = 0
  };
}
Alarm::State alarmState = Alarm::NOT_SET;

typedef enum {IDLE, PROG_MIN, PROG_SEC, COUNTDOWN, BEEP} state_t;
state_t current_state = IDLE;
state_t prev_state = BEEP;

SevenSegmentDisplay display(A_PIN, &A_PORT, B_PIN, &B_PORT,
                            C_PIN, &C_PORT, D_PIN, &D_PORT,
                            E_PIN, &E_PORT, F_PIN, &F_PORT,
                            G_PIN, &G_PORT, DPX_PIN, &DPX_PORT,
                            CA1_PIN, &CA1_PORT, CA2_PIN, &CA2_PORT,
                            CA3_PIN, &CA3_PORT, CA4_PIN, &CA4_PORT);

RotaryEncoder rotary(&BUTTON_DIR, BUTTON_PIN, &DT_DIR, DT_PIN, &CLK_DIR, CLK_PIN);



volatile uint8_t prevButtonVal = 1; //default state HIGH because of PULLUP RESISTOR
ISR(PCINT0_vect){
    uint8_t currentButtonVal = rotary.readButton();
    //Button was pressed
    if(!currentButtonVal && prevButtonVal){
      // Detect falling edge → button was just pressed
      buttonStatus = ButtonState::PRESSED;
    }

    prevButtonVal = currentButtonVal;
}


ISR(TIMER1_COMPA_vect) {
  COUNTER_TIMEOUT++;

  //reduce countdown by one if alarm is set
  if(alarmState == Alarm::SET && (unit_min + ten_min + unit_sec+ten_sec > 0)){ unit_sec--;}

  else if (alarmState == Alarm::NOT_SET && current_state == IDLE && COUNTER_TIMEOUT == 60 ){
      COUNTER_TIMEOUT = 0;
      display.clear();

      set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // Set power-down sleep mode
			cli(); //cli before sleep enable to avoid interferance with interrupts
			sleep_enable();   // Enable sleep mode
			sei();
			sleep_cpu();

  }
}

/**
 * @brief
 *
 * @param minute_unit
 * the unit part of the minute
 * @param minute_ten
 * the tenth part of the minute
 */

void UpdateMinutesUnit(int8_t *minute_unit, int8_t *minute_ten){

	if(*minute_unit > 9 && *minute_ten != 5) {
		(*minute_ten)++;
		*minute_unit = 0;
	}
	else if (*minute_unit < 0){
		(*minute_ten)--;
		*minute_unit = 9;

	}
	if(*minute_ten == 5 && *minute_unit > 9){
		*minute_ten = 0;
		*minute_unit = 0;
	}
	else if (*minute_ten < 0){
		*minute_ten = 5;
		*minute_unit = 9;
	}

};


/**
 * @brief
 *
 * @param sec_unit
 * the ones
 * @param sec_ten
 * the tens
 * @param min_unit
 * to bind it to the minutes variable
 */
void UpdateSecondsUnit(int8_t *sec_unit, int8_t *sec_ten, int8_t *min_unit ){


	 if(*sec_unit > 9 && *sec_ten != 5) {
		(*sec_ten)++;
		*sec_unit = 0;
	}
	else if (*sec_unit < 0){
		(*sec_ten)--;
		*sec_unit = 9;

	}

	if(*sec_ten == 5 && *sec_unit > 9){
			(*min_unit)++;
			*sec_ten = 0;
			*sec_unit = 0;
	}

	 else if (*sec_ten < 0){
			(*min_unit)--;
			*sec_ten = 5;
			*sec_unit = 9;
	}


};

void display_minutes(){
  display.first_digit(ON);
  display.show_value(ten_min);
  _delay_ms(1);
  display.clear();
  display.first_digit(OFF);

  display.decimal_point(ON);
  display.second_digit(ON);
  display.show_value(unit_min);
  _delay_ms(1);
  display.clear();
  display.second_digit(OFF);
  display.decimal_point(OFF);
}


void display_seconds(){

  display.third_digit(ON);
  display.show_value(ten_sec);
  _delay_ms(1);
  display.clear();
  display.third_digit(OFF);


  display.fourth_digit(ON);
  display.show_value(unit_sec);
  _delay_ms(1);
  display.clear();
  display.fourth_digit(OFF);
}


void initTimer_16bit(){

	TCCR1B |= (1<<WGM12); //set CTC mode CAPTURE COMPARE
	OCR1A = 976;
	TIMSK1 |= (1<<OCIE1A); //enable interrupt
	//timer every 1 second. For one minute it has to reache the ISR 60 times.
	sei(); //enable global interrupts
	TCCR1B |= (1<<CS12) | (1<<CS10); //set prescaler (CPU clock)/1024, CPUCLOCK by default = 1MHz

}

void initInterruptSW(){
  PCICR |= (1 << PCIE0); //activate interrupts from PCINT0 to PCINT7
  PCMSK0 |= (1 << PCINT2); //interrupt on PB2
}

 int main(){
   display.init();
   display.all_digits_off();

   rotary.init();

   initTimer_16bit();
   initInterruptSW();
   wdt_disable();

   BUZZER_DDR |= (1 << BUZZER_PIN);

   //disable Analog Comparator
   ACSR |= (1 << ACD);

   //turn off ADC
   ADCSRA &= ~(1 << ADEN);



   while(1){

     rotary_direction_t direction = rotary.getDirection();
     switch(current_state){
       case IDLE:

             if(current_state != prev_state){
               display.show_value(prev_state);
               _delay_ms(500);
               prev_state = current_state;
             }

            display_minutes();
            display_seconds();

            if(buttonStatus == ButtonState::PRESSED){
              buttonStatus = ButtonState::NOT_PRESSED;
              //prev_state = IDLE;
              current_state = PROG_MIN;
            }

            break;

       case PROG_MIN:

           if(current_state != prev_state){
    					_delay_ms(500);
    					prev_state = current_state;
    				}

            display_minutes();
            if(direction == CLOCKWISE){
              unit_min++;
              UpdateMinutesUnit(&unit_min, &ten_min);
            }
            else if (direction == COUNTERCLOCKWISE){
              unit_min--;
              UpdateMinutesUnit(&unit_min, &ten_min);
            }

            else if(buttonStatus == ButtonState::PRESSED){
              buttonStatus = ButtonState::NOT_PRESSED;
              prev_state = PROG_MIN;
              current_state = PROG_SEC;
            }

            break;

       case PROG_SEC:
             if(current_state != prev_state){
                _delay_ms(500);
                prev_state = current_state;
              }

              display_seconds();

             if(direction == CLOCKWISE){
               unit_sec++;
               UpdateMinutesUnit(&unit_sec, &ten_sec);
             }
             else if (direction == COUNTERCLOCKWISE){
               unit_sec--;
               UpdateMinutesUnit(&unit_sec, &ten_sec);
             }

             else if(buttonStatus == ButtonState::PRESSED){
               buttonStatus = ButtonState::NOT_PRESSED;
               prev_state = PROG_SEC;
               current_state = COUNTDOWN;
               alarmState = Alarm::SET; //start the alarm once for the next state
             }

             break;

       case COUNTDOWN:
            if(current_state != prev_state){
      				_delay_ms(500);
      				prev_state = current_state;
      			}
            display_minutes();
            display_seconds();


            UpdateSecondsUnit(&unit_sec, &ten_sec, &unit_min);
            UpdateMinutesUnit(&unit_min, &ten_min);

            if(buttonStatus == ButtonState::PRESSED){
              buttonStatus = ButtonState::NOT_PRESSED;
              prev_state = COUNTDOWN;
              current_state = IDLE;
              alarmState = Alarm::NOT_SET;

              unit_sec = 0;
              ten_sec = 0;

              unit_min = 0;
              ten_min = 0;
            }
            else if ((ten_min + unit_min + ten_sec + unit_sec) == 0){
              prev_state = COUNTDOWN;
              current_state = BEEP;
              alarmState = Alarm::NOT_SET;
            }
            break;

       case BEEP:
          BUZZER_PORT |= (1 << BUZZER_PIN);
          _delay_ms(300);
          BUZZER_PORT &= ~(1 << BUZZER_PIN);
          _delay_ms(500);

          if(buttonStatus == ButtonState::PRESSED){
            buttonStatus = ButtonState::NOT_PRESSED;
            prev_state = BEEP;
            current_state = IDLE;
          }

          break;
     }
   }
}
