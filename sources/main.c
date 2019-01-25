#include <string.h>
#include "runtime.h"
#include "days.h"

void sleep(u8 frames) { }

void main() {
	gsinit();
	day_1();
	halt();
}
