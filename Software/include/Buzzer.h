#ifndef BUZZER_H
#define BUZZER_H

#include <util/delay.h>

// Notas musicales
#define NOTE_E5 659
#define NOTE_D5 587
#define NOTE_C5 523
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_E4 330
#define NOTE_FS4 370
#define NOTE_GS4 415
#define NOTE_C4 261
#define NOTE_F4 349
#define NOTE_GS5 830
#define NOTE_CS5 554
#define NOTE_AS4 466
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_F5 698
#define REST 0

// Melodía Mario (simplificada)
int melody[] = {
  NOTE_E5, NOTE_E5, REST, NOTE_E5,
  REST, NOTE_C5, NOTE_E5, REST,
  NOTE_G5, REST, REST,  REST,
  NOTE_G4, REST, REST, REST,

  NOTE_C5, REST, REST, NOTE_G4,
  REST, REST, NOTE_E4, REST,
  REST, NOTE_A4, REST, NOTE_B4,
  REST, NOTE_AS4, NOTE_A4, REST,

  NOTE_G4, NOTE_E5, NOTE_G5,
  NOTE_A5, REST, NOTE_F5, NOTE_G5,
  REST, NOTE_E5, REST, NOTE_C5,
  NOTE_D5, NOTE_B4, REST, REST
};

int noteDurations[] = {
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  112, 112, 112,
  150, 150, 112, 112,
  150, 112, 150, 112,
  112, 112, 150, 150
};

void delay_us_variable(uint16_t us) {
    while (us--) {
        // Each iteration takes about 4 clock cycles on 16 MHz CPU
        // So 4 cycles = 0.25 µs → loop 4 times per µs
        for (uint8_t i = 0; i < 4; i++) {
            __asm__ __volatile__("nop");
        }
    }
}


void pause(unsigned int ms) {
    while (ms--) _delay_ms(1);
}

void tone(volatile uint8_t *port, uint8_t pin, unsigned int frequency, unsigned long duration_ms) {

  if (frequency == 0) {
     // Pausa (sin tono)
     for (unsigned long i = 0; i < duration_ms; i++) _delay_ms(1);
     return;
 }
    unsigned long delay_us = 1000000UL / (frequency * 2);  // medio período
    unsigned long cycles = (frequency * duration_ms) / 1000;

    for (unsigned long i = 0; i < cycles; i++) {
        (*port) |= (1 << pin);   // HIGH
        delay_us_variable(delay_us);
        (*port) &= ~(1 << pin);  // LOW
        delay_us_variable(delay_us);
    }
}


#endif
