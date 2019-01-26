#include "phone.h"
#include "texture.h"
#include "texture_phone.h"

void phone_call(void)
{
	texture_draw_fullscreen(&texture_phone);
}

void dispatch_call(void)
{ }
u8 dispatch_event(void)
{ return 0; }
