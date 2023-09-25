/*
 * SevSeg.c
 *
 * Created: 8/8/2022 11:19:24
 *  Author: User
 */ 
#include <SevSeg.h>


void SevSeg_display_digit(volatile uint8_t *port,uint8_t dig){
	uint8_t segment[10][7] = {
		{0,0,0,0,0,0,1}, //0
		{1,0,0,1,1,1,1}, //1
		{0,0,1,0,0,1,0}, //2
		{0,0,0,0,1,1,0}, //3
		{1,0,0,1,1,0,0}, //4
		{0,1,0,0,1,0,0}, //5
		{0,1,0,0,0,0,0}, //6
		{0,0,0,1,1,1,1}, //7
		{0,0,0,0,0,0,0}, //8
		{0,0,0,1,1,0,0}, //9
		
	};
	switch(dig){
		case 0:
		for (uint8_t i = 0; i<7; i++){
			if(segment[0][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 1:
		for (uint8_t i = 0; i<7; i++){
			if(segment[1][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 2:
		for (uint8_t i = 0; i<7; i++){
			if(segment[2][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 3:
		for (uint8_t i = 0; i<7; i++){
			if(segment[3][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 4:
		for (uint8_t i = 0; i<7; i++){
			if(segment[4][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 5:
		for (uint8_t i = 0; i<7; i++){
			if(segment[5][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 6:
		for (uint8_t i = 0; i<7; i++){
			if(segment[6][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 7:
		for (uint8_t i = 0; i<7; i++){
			if(segment[7][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 8:
		for (uint8_t i = 0; i<7; i++){
			if(segment[8][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
		case 9:
		for (uint8_t i = 0; i<7; i++){
			if(segment[9][i]==0) *port &= ~(1<<i);
			else *port |= (1<<i);
		}
		break;
	}
};

//Turn off all pins from port B
void SevSeg_clear(volatile uint8_t *port){
	
	for(uint8_t i = 0; i < 7; i++){
		*port |= (1<<i);
	}
}