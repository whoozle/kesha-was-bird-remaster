#include "calls.h"
#include "vars.h"
#include "_dialogs.h"

void ninja_fight(void);

void call_lab(void)
{
	set_flag(&lab_called);
	dialog_lab_security_1();

	ninja_fight();
}
