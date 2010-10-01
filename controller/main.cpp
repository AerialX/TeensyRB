#include <parapin.h>
#include <wiiuse.h>

#include <stdio.h>

#define TRB_VERSION "0.1"

struct ButtonMap
{
	int expansion;
	int button;
	int pin;
};

ButtonMap Buttons[] = {
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_GREEN,			LP_PIN01},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_RED,			LP_PIN02},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_YELLOW,		LP_PIN03},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_BLUE,			LP_PIN04},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_ORANGE,		LP_PIN05},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_STRUM_UP,		LP_PIN06},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_STRUM_DOWN,	LP_PIN07},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_PLUS,			LP_PIN08},
	{EXP_GUITAR_HERO_3,	GUITAR_HERO_3_BUTTON_MINUS,			LP_PIN09},
	{EXP_NONE,			WIIMOTE_BUTTON_HOME,				LP_PIN14},
	//{EXP_NONE,		WIIMOTE_BUTTON_,					LP_PIN16},
	//{EXP_NONE,		WIIMOTE_BUTTON_,					LP_PIN17},
	{0, 0}
};

int main(int argc, const char* argv[])
{
	wiimote* Wiimote;

	printf("Teensy Rock Band Controller v" TRB_VERSION "\n");

	printf("\tInitializing parallel port...\n");
	if (pin_init_user(LPT1)) {
		printf("\t\tFailed! Are you sure you're running as root?\n");
		return 1;
	}
	// Set as many pins as we can to output
	pin_output_mode(LP_PIN01 | LP_DATA_PINS | LP_PIN14 | LP_PIN16 | LP_PIN17);
	printf("\t\tDone!\n");

	printf("\tInitializing wiiuse...\n");
	wiimote** wiimotes = wiiuse_init(1);
	if (!wiimotes) {
		printf("\t\tFailed!\n");
		return 1;
	}
	printf("\tPut Wiimote in discovery mode by pressing 1 and 2...\n");
	if (!wiiuse_find(wiimotes, 1, 5)) {
		printf("\t\tFailed to find a wiimote.\n");
		return 1;
	}
	if (!wiiuse_connect(wiimotes, 1)) {
		printf("\t\tFailed to connect to wiimote.\n");
		return 1;
	}
	Wiimote = wiimotes[0];

	wiiuse_set_leds(Wiimote, WIIMOTE_LED_1 | WIIMOTE_LED_4);
	wiiuse_rumble(Wiimote, 0);

	printf("\t\tDone!\n");

	printf("Now mapping to parallel port, ^C to exit.\n");
	while (true) {
		if (!wiiuse_poll(wiimotes, 1))
			continue;
		if (Wiimote->event != WIIUSE_EVENT)
			continue;

		for (ButtonMap* button = Buttons; button->button || button->pin; button++) {
			int pressed = 0;
			int btn = button->button;
			// This just shows how poorly the library was designed
			switch (button->expansion) {
				case EXP_NONE:
					if (IS_JUST_PRESSED(Wiimote, btn))
						pressed = 1;
					if (IS_RELEASED(Wiimote, btn))
						pressed = -1;
					break;
				case EXP_NUNCHUK:
					if (Wiimote->exp.type == EXP_NUNCHUK) {
						if (IS_JUST_PRESSED((&Wiimote->exp.nunchuk), btn))
							pressed = 1;
						if (IS_RELEASED((&Wiimote->exp.nunchuk), btn))
							pressed = -1;
					}
					break;
				case EXP_CLASSIC:
					if (Wiimote->exp.type == EXP_CLASSIC) {
						if (IS_JUST_PRESSED((&Wiimote->exp.classic), btn))
							pressed = 1;
						if (IS_RELEASED((&Wiimote->exp.classic), btn))
							pressed = -1;
					}
					break;
				case EXP_GUITAR_HERO_3:
					if (Wiimote->exp.type == EXP_GUITAR_HERO_3) {
						if (IS_JUST_PRESSED((&Wiimote->exp.gh3), btn))
							pressed = 1;
						if (IS_RELEASED((&Wiimote->exp.gh3), btn))
							pressed = -1;
					}
					break;
			}
			if (pressed > 0)
				set_pin(button->pin);
			else if (pressed < 0)
				clear_pin(button->pin);
		}
	}
	return 0;
}
