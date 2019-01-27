#include "calls.h"
#include "runtime.h"
#include "_dialogs.h"

void call_invalid(void)
{
	u8 r = get_r() & 7;
	if (r & 1)
		return dialog_no_answer_1();

	r >>= 1;
	switch(r)
	{
		case 0:
			return dialog_no_answer_5();
		case 1:
			return dialog_no_answer_2();
		case 2:
			return dialog_no_answer_3();
		case 3:
			return dialog_no_answer_4();
	}
}
