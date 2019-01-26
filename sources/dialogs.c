#include "types.h"
#include "font.h"
#include "texture.h"
#include "runtime.h"
#include "text.h"
#include "tiles.h"
#include "days.h"

u8 audio_click;
u8 pets_plan;

void room_draw(void) { }
void drinking_draw(void) { }
void professor_show_banner(void) { }

void ninja_show_banner(void) { }
void ninja_show_eraser(void) { }
void ninja_show_fday_device(void) { }

void set_flag(u8 *flag) { (void)flag; }

void audio_play_sync(void *audio) { (void)audio; }
void audio_invalid_number() { }

void glitch_fill() { }
void glitch_voice() { }
void glitch_voice_next() { }

#include "_dialogs.c"
