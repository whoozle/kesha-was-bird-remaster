#include <string.h>
#include "runtime.h"
#include "days.h"
#include "phone.h"

void call_lab(void);

void main(void)
{
	gsinit();

	day_1();
	day_2();
	while(1)
	{
		srand_r();
		phone_call();
		dispatch_call();

		day_increment();
		day_intertitle();

		dispatch_event();
	}
}
