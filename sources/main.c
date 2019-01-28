#include <string.h>
#include "runtime.h"
#include "days.h"
#include "phone.h"

void ninja_fight();

void main() {
	gsinit();
	ninja_fight();
	//day_1();
	//day_2();
	while(1)
	{
		phone_call();
		dispatch_call();

		day_increment();
		day_intertitle();

		dispatch_event();
	}
}
