#ifndef KESHA_AUDIO_H
#define KESHA_AUDIO_H

#include "types.h"

void audio_play_sync(const u8 *audio);
void audio_invalid_number(void);

extern const u8 audio_text_beep[16];
extern const u8 audio_click[16];
extern const u8 audio_lab_bull[16];
extern const u8 audio_lab_cow[16];
extern const u8 audio_lab_noise[16];
extern const u8 audio_lab_sword_1[16];
extern const u8 audio_lab_sword_2[16];

#endif
