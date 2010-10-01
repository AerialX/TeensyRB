#include "comm.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Solo frets
//#define ALWAYS_TAPPING

// Button Mapping
typedef struct {
	u8 pin; // MS4B is the port (0 = A, 5 = F), LS4B is the pin
	u8 bit; // Bit offset in buffer
} ButtonMap;

ButtonMap buttons[] = {
	{0x10, 0x01}, // PB0: Green
	{0x11, 0x02}, // PB1: Red
	{0x12, 0x03}, // PB2: Yellow
	{0x13, 0x00}, // PB3: Blue
	{0x14, 0x04}, // PB4: Orange
	{0x15, 0x0b}, // PB5: Strum
	{0x16, 0x0c}, // PB6: Strum
	{0x17, 0x06}, // PB7: Solo frets
	{0x20, 0x0a}, // PD0: Start
	{0x21, 0x09}, // PD1: Select
	{0x22, 0x05}, // PD2: Overdrive
	{0x23, 0x0d}, // PD3: PS Button
	{0, 0}
};

void Communicate_Init()
{
	for (ButtonMap* button = buttons; button->bit || button->pin; button++) {
		switch (button->pin >> 4) { // Port
			case 0: DDRA = 0; PORTA = 0; break;
			case 1: DDRB = 0; PORTB = 0; break;
			case 2: DDRC = 0; PORTC = 0; break;
			case 3: DDRD = 0; PORTD = 0; break;
			case 4: DDRE = 0; PORTE = 0; break;
			case 5: DDRF = 0; PORTF = 0; break;
		}
	}
}

void Communicate(u8* buffer)
{
#ifdef ALWAYS_TAPPING
	buffer[0] |= (1 << 6);
#endif
	for (ButtonMap* button = buttons; button->bit || button->pin; button++) {
		u8 bit = button->bit;
		u8* byte = buffer;
		while (bit >= 0x8) {
			bit -= 0x8;
			byte++;
		}
		u8 pin = button->pin & 0x0F;

		switch (button->pin >> 4) {
			case 0:
				if (PINA & (1 << pin))
					*byte |= (1 << bit);
				break;
			case 1:
				if (PINB & (1 << pin))
					*byte |= (1 << bit);
				break;
			case 2:
				if (PINC & (1 << pin))
					*byte |= (1 << bit);
				break;
			case 3:
				if (PIND & (1 << pin))
					*byte |= (1 << bit);
				break;
			case 4:
				if (PINE & (1 << pin))
					*byte |= (1 << bit);
				break;
			case 5:
				if (PINF & (1 << pin))
					*byte |= (1 << bit);
				break;
		}
	}
}
