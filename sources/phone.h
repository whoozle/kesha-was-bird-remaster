#ifndef KESHA_PHONE_H
#define KESHA_PHONE_H

#include "types.h"

u8 poll_digit(void);
u8 read_digit(void);
void phone_call(void);
void dispatch_call(void);
u8 dispatch_event(void);


#endif
