#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usb.h"

#include <string.h>
#define true 1
#define false 0
#define bool uint8_t

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

volatile bool press = false;
int main(void)
{
	uint8_t buffer[0x10];
	CPU_PRESCALE(0);

	usb_init();
	while (!usb_configured())
		;
	_delay_ms(1000);

	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TIMSK0 = (1 << TOIE0);

	Communicate_Init();
	while (true) {
		int8_t r = usb_rawhid_recv(buffer, 0);

		memset(buffer, 0, sizeof(buffer));
		Communicate(buffer);
		usb_rawhid_send(buffer, 50);
	}
}

// This interrupt routine is run approx 61 times per second.
ISR(TIMER0_OVF_vect)
{
}

