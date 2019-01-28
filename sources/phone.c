#include "phone.h"
#include "texture.h"
#include "texture_phone.h"
#include "keyboard.h"
#include "_dialogs.h"
#include "font.h"
#include "vars.h"
#include "text.h"
#include "calls.h"
#include <string.h>

#define phone_number_x 		12
#define phone_number_y 		10
#define phone_number_step 	8

static u8 number[4];
static u8 number_size = 0;
static u8 number_pos = phone_number_x;

static void notepad_draw()
{
	u8 x = 64, y = 10;
	text_draw(x, y, text_note_galya);
	if (galina_calls >= 3) {
		y += 8;
		text_draw(x, y, text_note_ninja);
	}
	if (spam_spam_shown) {
		y += 8;
		text_draw(x, y, text_note_spam);
	}
	if (pets_spam_shown) {
		y += 8;
		text_draw(x, y, text_note_pets);
	}
	if (lab_called) {
		y += 8;
		text_draw(x, y, text_note_lab);
	}
}

u8 poll_digit()
{
	u8 byte = Keyboard_54321 & 0x1fu;
	if (byte != 0x1fu)
	{
		if (!(byte & 1))
			return 1;
		else if (!(byte & 2))
			return 2;
		else if (!(byte & 4))
			return 3;
		else if (!(byte & 8))
			return 4;
		else if (!(byte & 16))
			return 5;
	}
	byte = Keyboard_67890 & 0x1fu;
	if (byte != 0x1fu)
	{
		if (!(byte & 1))
			return 0;
		else if (!(byte & 2))
			return 9;
		else if (!(byte & 4))
			return 8;
		else if (!(byte & 8))
			return 7;
		else if (!(byte & 16))
			return 6;
	}
	return 0xffu;
}

u8 read_digit() {
	u8 digit;
	do
	{
		digit = poll_digit();
	}
	while(digit == 0xff);
	while(poll_digit() != 0xff);
	return digit;
}

static void clear_number() {
	number_size = 0;
	memset(number, 0xff, sizeof(number));
	number_pos = phone_number_x;
}

void phone_call(void)
{
	clear_number();
	texture_draw_fullscreen(&texture_phone);
	notepad_draw();
	//phone_draw_number();
	while(number_size < 4) {
		u8 digit = read_digit();
		number[number_size++] = digit;
		font_draw_char(digit + '0' - CHAR_MIN, number_pos, phone_number_y);
		number_pos += phone_number_step;
	}
}

struct Number {
	u8 number[4];
	void (*handler)();
};
typedef struct Number Number;

static const Number numbers[] =
{
	{ { 1, 1, 1, 3 }, &call_galina },
	{ { 1, 3, 3, 7 }, &call_ninja }, //call_ninja
	{ { 3, 4, 7, 4 }, &dialog_no_answer_5 },
	{ { 7, 3, 7, 8 }, &call_pets }, //call pets
	{ { 7, 3, 8, 7 }, &call_pets }, //call pets
	{ { 7, 7, 2, 6 }, &dialog_spam_1 },
	{ { 8, 5, 5, 5 }, &call_lab }, //call lab
	{ {0, 0, 0, 0}, NULL }
};

void dispatch_call(void)
{
	if (call_glitch())
		return;

	const Number * n = numbers;
	while(n->handler)
	{
		if (memcmp(n->number, number, 4) == 0)
		{
			return n->handler();
		}
		++n;
	}
	call_invalid();
}

u8 dispatch_event(void)
{ return 0; }
