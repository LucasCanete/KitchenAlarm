#ifndef DEFINES_H
#define DEFINES_H

/* 4 DIGIT 7 SEGMENT DISPLAY*/

#define A_PIN     PD5
#define A_PORT    DDRD

#define B_PIN     PB1
#define B_PORT    DDRB

#define C_PIN     PC3
#define C_PORT    DDRC

#define D_PIN     PC5
#define D_PORT    DDRC

#define E_PIN     PD2
#define E_PORT    DDRD

#define F_PIN     PD6
#define F_PORT    DDRD

#define G_PIN     PC2
#define G_PORT    DDRC

#define DPX_PIN   PC4
#define DPX_PORT  DDRC

#define CA1_PIN     PB7
#define CA1_PORT    DDRB

#define CA2_PIN     PD7
#define CA2_PORT    DDRD

#define CA3_PIN     PB0
#define CA3_PORT    DDRB

#define CA4_PIN     PC1
#define CA4_PORT    DDRC

/* BUZZER */

#define BUZZER_PORT PORTC
#define BUZZER_DDR  DDRC
#define BUZZER_PIN  PC0


/* ROTARY ENCODER */

#define DT_DIR DDRD
#define DT_PIN PD4

#define CLK_DIR DDRD
#define CLK_PIN PD3

#define BUTTON_DIR DDRB
#define BUTTON_PIN PB2

#endif
