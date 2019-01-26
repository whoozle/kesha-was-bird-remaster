#include <string.h>
#include "runtime.h"
#include "days.h"
#include "phone.h"

void main() {
	gsinit();
	//day_1();
	//day_2();
	while(1)
	{
		phone_call();
		dispatch_call();
		do
		{
			day_increment();
			day_intertitle();
		} while (dispatch_event());

	}
	halt();
}
