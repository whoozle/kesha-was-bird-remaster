#include "calls.h"
#include "vars.h"
#include "_dialogs.h"

void call_pets(void)
{
	if (!pets_plan)
	{
		dialog_pets_1();
	}
	else
	{
		if (!pets_got_galina)
		{
			set_flag(&pets_called);
			dialog_pets_2();
		}
		else
			dialog_pets_3();
	}
}
