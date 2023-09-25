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

volatile bool beep_activated = false;
volatile bool was_sleeping = 0;
volatile uint16_t COUNTER_TIMEOUT = 0;
bool alarm_is_set = false;
#define SW PD5
#define DT PD6
#define CLK PD7
uint8_t button_layer = 0; //1: configurate minutes | 2:configurate seconds| 3:exit menu
int8_t unit_sec = 0;
int8_t ten_sec = 0;
	
int8_t unit_min = 0;
int8_t ten_min = 0;

ISR(TIMER1_COMPA_vect){
	
		COUNTER_TIMEOUT++;
		if(alarm_is_set && (unit_min + ten_min + unit_sec+ten_sec > 0)) unit_sec--;
		 else if(COUNTER_TIMEOUT == 60 && !alarm_is_set && button_layer == 0  ){
	
			SevSeg_clear(&PORTB);
			setBit(PORTC,PC4); //deactivate decimal point
			COUNTER_TIMEOUT = 0;
			was_sleeping = 1;
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // Set power-down sleep mode
			cli(); //cli before sleep enable to avoid interferance with interrupts
			sleep_enable();   // Enable sleep mode
			sei();
			sleep_cpu();
		
		}
		
		

};


ISR(PCINT2_vect ) {
	//wake uC up
	beep_activated = false;
	printString("waking up!!! \r\n");

}


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
	
		//4ms x 250 = 1s
	
		//for(uint8_t i = 0; i <= 120; i++){
			
			setBit(PORTC,PC2);
			SevSeg_display_digit(&PORTB,*ten_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC2);
			
			setBit(PORTC,PC3);
			SevSeg_display_digit(&PORTB,*unit_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC3);
			
			setBit(PORTC,PC0);
			SevSeg_display_digit(&PORTB,*ten_min);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC0);
			
			setBit(PORTC,PC1);
			SevSeg_display_digit(&PORTB,*unit_min);
			clearBit(PORTC,PC4);// activate decimal point
			_delay_ms(1);
			setBit(PORTC,PC4);// deactivate decimal point
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC1);
			
			
			
		//}
		//(*unit_sec)--;
	
	
}
//returns if rotary is rotating clockwise 1 or counterclockwise 0
uint8_t rotaryEncoderdirection(uint8_t *current_clkstate, uint8_t *last_clkstate){
	uint8_t dir = 3;
	if(*current_clkstate != *last_clkstate && *current_clkstate == 0){
		uint8_t dt_state = bit_is_set(PIND,DT);
		if(dt_state != *current_clkstate){//clockwise
			//printString("Horario \r\n"); 
			dir = 1;
			
			} 
		else if(dt_state == *current_clkstate){//counterclockwise
			//printString("Antihorario \r\n");
			dir = 2;
			
			} 
		
	}
	*last_clkstate = *current_clkstate;

	return dir;
}

//for debouncing purposes
uint8_t button_pressed(){
	if(bit_is_clear(PIND,SW)){
		_delay_ms(10);
		if(bit_is_clear(PIND,SW)) {return (1);}
		
	}
	
	return (0);
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

		//Configurate Minutes
		if(button_pressed() && !was_sleeping){
			button_layer++; //layer 1
			_delay_ms(500);//for stabilisation
			while(button_layer == 1){
				
				//Display Minutes, seconds won't be shown
					setBit(PORTC,PC0);
					SevSeg_display_digit(&PORTB,ten_min);
					_delay_ms(1);
					SevSeg_clear(&PORTB);
					clearBit(PORTC,PC0);
							
					setBit(PORTC,PC1);
					SevSeg_display_digit(&PORTB,unit_min);
					clearBit(PORTC,PC4);// activate decimal point
					_delay_ms(1);
					setBit(PORTC,PC4);// deactivate decimal point
					SevSeg_clear(&PORTB);
					clearBit(PORTC,PC1);
				///////////////////			
				
				currentStateClk = bit_is_set(PIND,CLK);

				rotary_direction = rotaryEncoderdirection(&currentStateClk,&lastStateClk);
				//Clockwise
				if(rotary_direction == 1 ){
					unit_min++;
					UpDownCounterMinute(&unit_min, &ten_min);
					} 
				//CounterClockwise
				else if(rotary_direction == 2){
					unit_min--;
					UpDownCounterMinute(&unit_min, &ten_min);					
					}
				//Configurate Seconds
				if(button_pressed()){
					button_layer++; //layer 2
					_delay_ms(500);
					while(button_layer == 2){
					
						//Display Seconds, Minutes won't be shown
						setBit(PORTC,PC2);
						SevSeg_display_digit(&PORTB,ten_sec);
						_delay_ms(1);
						SevSeg_clear(&PORTB);
						clearBit(PORTC,PC2);
					
						setBit(PORTC,PC3);
						SevSeg_display_digit(&PORTB,unit_sec);
						_delay_ms(1);
						SevSeg_clear(&PORTB);
						clearBit(PORTC,PC3);
						////////////////////////////
						currentStateClk = bit_is_set(PIND,CLK);

						rotary_direction = rotaryEncoderdirection(&currentStateClk,&lastStateClk);
						//Clockwise
						if(rotary_direction == 1 ){
							unit_sec++;
							UpDownCounterMinute(&unit_sec, &ten_sec);

						}
						//CounterClockwise
						else if(rotary_direction == 2){
							unit_sec--;
							UpDownCounterMinute(&unit_sec, &ten_sec);						
						}
						if(button_pressed()) button_layer++; //layer 3				
					}
				
				}
	
				
				
			}

			alarm_is_set = true; //activate alarm
			button_layer = 0;
		}
	

		_delay_ms(10);
		
	/*if*/while(alarm_is_set){
		
			alarm_countdown(&ten_min,&unit_min,&ten_sec,&unit_sec); //keep here always before updowncountersecond function
			UpDownCounterSecond(&unit_sec, &ten_sec,&unit_min);
			UpDownCounterMinute(&unit_min, &ten_min);

	
			if((ten_min + unit_min + ten_sec + unit_sec) == 0 ){
				beep_activated = true;
				while(beep_activated){
					setBit(PORTD,PD2);		
					buzzer_peep(&PORTC,PC5);
					_delay_ms(200);
					clearBit(PORTD,PD2);
					buzzer_stop(&PORTC,PC5);
					_delay_ms(200);
				}
				COUNTER_TIMEOUT = 0;
				//unit_sec = 1;
				alarm_is_set = false; //deactivate alarm
				_delay_ms(600);	
			}
		
		}
		
  /*else if*/ if(!alarm_is_set){
			if(was_sleeping){_delay_ms(500);was_sleeping = 0;}
				
			setBit(PORTC,PC2);
			SevSeg_display_digit(&PORTB,ten_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC2);
					
			setBit(PORTC,PC3);
			SevSeg_display_digit(&PORTB,unit_sec);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC3);
						
			setBit(PORTC,PC0);
			SevSeg_display_digit(&PORTB,ten_min);
			_delay_ms(1);
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC0);
						
			setBit(PORTC,PC1);
			SevSeg_display_digit(&PORTB,unit_min);
			clearBit(PORTC,PC4);// activate decimal point
			_delay_ms(1);
			setBit(PORTC,PC4); //deactivate decimal point
			SevSeg_clear(&PORTB);
			clearBit(PORTC,PC1);
			
		}
		
	} //while super loop
	
} //main


