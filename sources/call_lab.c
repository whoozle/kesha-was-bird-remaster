#include "calls.h"
#include "vars.h"
#include "runtime.h"
#include "phone.h"
#include "font.h"
#include "_dialogs.h"

void ninja_fight(void);

static u8 pin[4] = { 0xff, 0xff, 0xff, 0xff };

inline static u8 rand_digit(u8 index)
{
	while(1)
	{
		u8 d = rand() & 0x0fu;
		if (d > 9)
			d -= 6;

		for (u8 i = 0; i < index; ++i)
			if (pin[i] == d)
				continue;
		return d;
	}
}

void call_lab(void)
{
	set_flag(&lab_called);

	if (pin[0] == 0xff)
	{
#if 1
		pin[0] = rand_digit(0);
		pin[1] = rand_digit(1);
		pin[2] = rand_digit(2);
		pin[3] = rand_digit(3);
#else
		pin[0] = 1;
		pin[1] = 2;
		pin[2] = 3;
		pin[3] = 4;
#endif
	}

	dialog_lab_security_1();

	while(1)
	{
		for(u8 i = 0; i < 4; ++i)
		{
			u8 digit = read_digit();
			if (digit > 9)
				return;
			font_draw_char(11 /* * */, 10 + (i << 3), 40);
		}
	}

	u8 bulls = 0, cows = 0;

}
