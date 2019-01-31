#ifndef KESHA_AUDIO_H
#define KESHA_AUDIO_H

#include "types.h"

void audio_play_sync(const u8 *audio);
void audio_invalid_number(void);
void audio_play_music(const u16 *indices, const u8 *data, u8 loop);
void audio_play_dtmf(u8 index, u8 n);

extern const u8 audio_text_beep[16];
extern const u8 audio_click[16];
extern const u8 audio_lab_bull[16];
extern const u8 audio_lab_cow[16];
extern const u8 audio_noise[16];
extern const u8 audio_sword_1[16];
extern const u8 audio_sword_2[16];

#endif
