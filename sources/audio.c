#include "audio.h"
#include "runtime.h"
#include "keyboard.h"
#include "types.h"

const u8 audio_text_beep[16] 	= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
const u8 audio_click[16] 		= { 0x02, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 audio_lab_bull[16] 	= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
const u8 audio_lab_cow[16] 		= { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 audio_noise[16] 		= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xff, 0x00, 0x00, 0x00 };
const u8 audio_sword_1[16] 		= { 0x07, 0xe0, 0x64, 0x3f, 0x80, 0x4c, 0xe0, 0x01, 0x0c, 0xe0, 0x7e, 0x8c, 0xf8, 0x00, 0xe3, 0x01 };
const u8 audio_sword_2[16] 		= { 0xc4, 0x0e, 0x02, 0x32, 0x04, 0x03, 0x81, 0x90, 0x1e, 0x70, 0x01, 0xc0, 0x32, 0x0c, 0x60, 0x02 };

static void audio_play_sync_impl(const u8 *audio, u8 disco)
{
	u8 keys = Keyboard_BNMSymSpace;
	if (!(keys & 1))
		return;
	u8 size = 16;
	while(size--)
	{
		u8 byte = *audio++;
		u8 bits = 8;
		if (disco)
			border(byte);
		while(bits--)
		{
			speaker(byte & 0x80u);

			byte <<= 1;
			u8 delay = 8;
			while(delay--);
		}
	}
}
void audio_play_sync(const u8 *audio)
{ audio_play_sync_impl(audio, 0); }

void audio_play_music(const u16 *indices, const u8 *data, u8 loop)
{
	do
	{
		u16 index = 0;
		while(1)
		{
			u16 offset = indices[index++];
			if (offset == 0xffffu)
				break;
			audio_play_sync_impl(data + offset, 1);
			u8 keys = Keyboard_BNMSymSpace;
			if (!(keys & 1))
				return;
		}
	}
	while(loop);
}

extern u8 audio_phone_tones[20][16];

void audio_play_dtmf(u8 index, u8 n)
{
	while(n--)
		audio_play_sync(audio_phone_tones[index]);
}

void audio_invalid_number(void)
{
	audio_play_dtmf(16, 13);
	audio_play_dtmf(17, 13);
	audio_play_dtmf(18, 19);
	sleep(25);
}
