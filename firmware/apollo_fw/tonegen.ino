#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef BUZZER_AUTOMATIC
#if PIN_BUZZER < 8
#define TONEGEN_PORTx  PORTD
#define TONEGEN_PINNUM PIN_BUZZER
#elif PIN_BUZZER <= 13
#define TONEGEN_PORTx  PORTB
#define TONEGEN_PINNUM (PIN_BUZZER - 8)
#endif
#endif

void tonegen_on()
{
	#ifdef BUZZER_AUTOMATIC
	digitalWrite(PIN_BUZZER, HIGH);
	#else
	OCR2A  = 122; // calculated for 2.048 kHz with clk/64 prescaler
	TCCR2A = (1 << WGM21); // CTC mode
	TCCR2B = (1 << CS21) | (1 << CS20); // clk/64 prescaler
	TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
	#endif
}

void tonegen_off()
{
	digitalWrite(PIN_BUZZER, LOW);
	#ifndef BUZZER_AUTOMATIC
	TCCR2B = 0; // disable timer
	TIMSK2 &= ~(1 << OCIE2A); // disable timer compare interrupt
	#endif
}

#ifndef BUZZER_AUTOMATIC
ISR(TIMER2_COMPA_vect)
{
	TONEGEN_PORTx ^= (1 << TONEGEN_PINNUM);
}
#endif
