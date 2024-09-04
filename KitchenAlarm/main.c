/*
 * KitchenAlarm.c
 *
 * Created: 7/8/2022 21:58:45
 * Author : Lucas
 */ 

/*
	AVR		SevSeg Display
	PB0		A
	PB1		B
	PB2		C
	PB3		D
	PB4		E
	PB5		F	
	PB6		G
	
	
	Seven Segment layout:
	--A--
	F	B
	F	B
	--G--
	E	C
	E	C
	--D--
	
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#include <gpio.h>
#include <SevSeg.h>
#include <Buzzer.h>
#include <USART.h>

typedef enum {IDLE, PROG_MIN, PROG_SEC, COUNTDOWN, BEEP} state_t;

state_t state = IDLE;
state_t prev_state = BEEP;
typedef enum {NONE,CLOCKWISE, COUNTERCLOCKWISE} rotary_dir_t;


volatile bool beep_activated = false;
volatile bool was_sleeping = false;
volatile uint16_t COUNTER_TIMEOUT = 0;
bool alarm_is_set = false;
#define SW PD5
#define DT PD6
#define CLK PD7

int8_t unit_sec = 0;
int8_t ten_sec = 0;
	
int8_t unit_min = 0;
int8_t ten_min = 0;



void beep_and_blink(){
	setBit(PORTD,PD2);
	buzzer_peep(&PORTC,PC5);
	_delay_ms(200);
	clearBit(PORTD,PD2);
	buzzer_stop(&PORTC,PC5);
	_delay_ms(200);
}




void display_minutes(){
	clearBit(PORTC,PC0);//setBit(PORTC,PC0);
	SevSeg_display_digit(&PORTB,ten_min);
	_delay_ms(1);
	SevSeg_clear(&PORTB);
	setBit(PORTC,PC0);//clearBit(PORTC,PC0);

	clearBit(PORTC,PC1);//setBit(PORTC,PC1);
	setBit(PORTC,PC4);// activate decimal point
	SevSeg_display_digit(&PORTB,unit_min);
	_delay_ms(1);
	SevSeg_clear(&PORTB);
	clearBit(PORTC,PC4);// deactivate decimal point
	setBit(PORTC,PC1);//clearBit(PORTC,PC1);
}


void display_seconds(){
	clearBit(PORTC,PC2);
	SevSeg_display_digit(&PORTB,ten_sec);
	_delay_ms(1);
	SevSeg_clear(&PORTB);
	setBit(PORTC,PC2);

	clearBit(PORTC,PC3);
	SevSeg_display_digit(&PORTB,unit_sec);
	_delay_ms(1);
	SevSeg_clear(&PORTB);
	setBit(PORTC,PC3);
}

/**
 * @brief Construct a new ISR object
 * 
 * Counts to one minute if it is in IDLE sleep
 * if alarm activavated then decrease counter
 */

ISR(TIMER1_COMPA_vect){
	
		COUNTER_TIMEOUT++;
		if(alarm_is_set && (unit_min + ten_min + unit_sec+ten_sec > 0)) unit_sec--;
		 else if(COUNTER_TIMEOUT == 60 && !alarm_is_set && state == IDLE  ){
	
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC4); //deactivate decimal point
			COUNTER_TIMEOUT = 0;
			was_sleeping = true;
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // Set power-down sleep mode
			cli(); //cli before sleep enable to avoid interferance with interrupts
			sleep_enable();   // Enable sleep mode
			sei();
			sleep_cpu();
		
		}

};

/**
 * @brief 
 * State Handler function 
 * Dependent on switch button
 */
volatile uint8_t prevButtonVal = 1; //default state HIGH because of PULLUP RESISTOR
ISR(PCINT2_vect ) {
	//wake uC up
	printString("waking up!!! \r\n");

	uint8_t currentButtonVal = gpioReadPin(&PIND,SW);
	//Falling Edge
	if(prevButtonVal && !currentButtonVal){
		switch(state){

			case IDLE:
				if(!was_sleeping){
					prev_state = IDLE;
					state = PROG_MIN;
				}
				break;
			case PROG_MIN:
				prev_state = PROG_MIN;
				state = PROG_SEC;
				break;
			case PROG_SEC:
				alarm_is_set = true;
				prev_state = PROG_SEC;
				state = COUNTDOWN;
				break;
			case COUNTDOWN:
				COUNTER_TIMEOUT = 0;
				alarm_is_set = false;
				prev_state = COUNTDOWN;
				state = IDLE;
				break;

			case BEEP:
				prev_state = BEEP;
				state = IDLE;
				break;
		}
	}

	prevButtonVal = currentButtonVal;

}

/**
 * @brief 
 * 
 * @param minute_unit 
 * the unit part of the minute
 * @param minute_ten 
 * the tenth part of the minute
 */

void UpDownCounterMinute(int8_t *minute_unit, int8_t *minute_ten){

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
void UpDownCounterSecond(int8_t *sec_unit, int8_t *sec_ten, int8_t *min_unit ){


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


void alarm_countdown(int8_t *ten_min, int8_t *unit_min, int8_t *ten_sec, int8_t *unit_sec){
	
			clearBit(PORTC,PC2);
			SevSeg_display_digit(&PORTB,*ten_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			setBit(PORTC,PC2);
			
			clearBit(PORTC,PC3);
			SevSeg_display_digit(&PORTB,*unit_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			setBit(PORTC,PC3);
			
			clearBit(PORTC,PC0);
			SevSeg_display_digit(&PORTB,*ten_min);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			setBit(PORTC,PC0);
			
			clearBit(PORTC,PC1);
			setBit(PORTC,PC4);// activate decimal point
			SevSeg_display_digit(&PORTB,*unit_min);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC4);// deactivate decimal point
			setBit(PORTC,PC1);
	
}


//returns if rotary is rotating clockwise 1 or counterclockwise 0
uint8_t rotaryEncoderdirection(uint8_t *current_clkstate, uint8_t *last_clkstate){
	uint8_t dir = 3;
	//rotary_dir_t dir = NONE;
	if(*current_clkstate != *last_clkstate && *current_clkstate == 0){
		uint8_t dt_state = bit_is_set(PIND,DT);
		if(dt_state != *current_clkstate){//clockwise

				dir = CLOCKWISE;
			} 
		else if(dt_state == *current_clkstate){//counterclockwise

				dir = COUNTERCLOCKWISE;
			} 
		
	}
	*last_clkstate = *current_clkstate;

	return dir;
}


void initTimer_16bit(){
	
	TCCR1B |= (1<<WGM12); //set CTC mode CAPTURE COMPARE
	OCR1A |= 976;
	TIMSK1 |= (1<<OCIE1A); //enable interrupt
	//timer every 0.01 seconds. For one minute it has to reache 6000 the ISR.	
	sei(); //enable global interrupts
	TCCR1B |= (1<<CS12) | (1<<CS10); //set prescaler (CPU clock)/1024, CPUCLOCK by default = 1MHz
	
}

int main(void)
{
	//init external Interrupt on SW
	PCICR |= (1<< PCIE2); //any change in any pin in portd will activate interrupt
	PCMSK2 |= (1 << SW); //here it is specified which pin from portd will activate interrupt
	
	initTimer_16bit(); //every second
	
	initUSART();
	uint8_t currentStateClk;
	uint8_t lastStateClk;
	uint8_t rotary_direction;
		
	
    for(uint8_t i = 0; i<7; i++){
		setBit(DDRB,i);
	}
	setBit(DDRC,PC0); //ten minute
	setBit(DDRC,PC1); //unit minute
	
	setBit(DDRC,PC2); //ten second
	setBit(DDRC,PC3); //unit second
	
	setBit(DDRC,PC4); //decimal point
	setBit(PORTC,PC4);//deactivate decimal point
	
	setBit(DDRC,PC5); //buzzer
	
	setBit(DDRD,PD2); //alarm led
	
	
	clearBit(DDRD,DT); //input
	clearBit(DDRD,CLK); //input
	clearBit(DDRD,SW); //switch if pressed a 0 is read
	
	lastStateClk = bit_is_set(PIND,CLK); //return non zero if bit is clear. 0 if bit is set


	

    while (1) 
    {

		switch(state){
			case IDLE:
				if(state != prev_state || was_sleeping){
					was_sleeping = 0;
					unit_sec = 0;
					unit_min = 0;
					ten_sec = 0;
					ten_min = 0;
					_delay_ms(500);
					prev_state = state;
				}
				display_seconds();
				display_minutes();

				break;


			case PROG_MIN:
				if(state != prev_state){
					_delay_ms(500);
					prev_state = state;
				}
				display_minutes();

				currentStateClk = bit_is_set(PIND,CLK);
				rotary_direction = rotaryEncoderdirection(&currentStateClk,&lastStateClk);
				if(rotary_direction == CLOCKWISE ){
					unit_min++;
					UpDownCounterMinute(&unit_min, &ten_min);
					}

				else if(rotary_direction == COUNTERCLOCKWISE){
					unit_min--;
					UpDownCounterMinute(&unit_min, &ten_min);
					}

				break;


			case PROG_SEC:
				if(state != prev_state){
					_delay_ms(500);
					prev_state = state;
				}

				display_seconds();

				currentStateClk = bit_is_set(PIND,CLK);
				rotary_direction = rotaryEncoderdirection(&currentStateClk,&lastStateClk);
				if(rotary_direction == CLOCKWISE){
					unit_sec++;
					UpDownCounterMinute(&unit_sec, &ten_sec);
				}
				else if(rotary_direction == COUNTERCLOCKWISE){
					unit_sec--;
					UpDownCounterMinute(&unit_sec, &ten_sec);
				}

				break;


			case COUNTDOWN:
				if(state != prev_state){
					_delay_ms(500);
					prev_state = state;
				}

				alarm_countdown(&ten_min,&unit_min,&ten_sec,&unit_sec); //keep here always before updowncountersecond function
				UpDownCounterSecond(&unit_sec, &ten_sec,&unit_min);
				UpDownCounterMinute(&unit_min, &ten_min);

				if((ten_min + unit_min + ten_sec + unit_sec) == 0 ) {
					COUNTER_TIMEOUT = 0;
					alarm_is_set = false;
					state = BEEP;}

				break;


			case BEEP:
				if(state != prev_state){
					_delay_ms(500);
					prev_state = state;
				}
				beep_and_blink();

				break;

		}

	} //while super loop
	
} //main


