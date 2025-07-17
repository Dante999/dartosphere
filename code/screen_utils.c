#include "screen_utils.h"


void line_cursor_up(Line_Cursor *cursor)
{
	if (cursor->index > 0) {
		cursor->index--;
	}
	else if (cursor->index == 0 && cursor->cycle) {
		cursor->index = cursor->max_index;
	}
}

void line_cursor_down(Line_Cursor *cursor)
{
	if (cursor->index < cursor->max_index) {
		cursor->index++;
	}
	else if (cursor->index == cursor->max_index  && cursor->cycle) {
		cursor->index = 0;
	}
}


void int_chooser_increase(Int_Chooser *chooser)
{
	if (chooser->value < chooser->max_value) {
		chooser->value++;
	}
	else if (chooser->value == chooser->max_value && chooser->cycle) {
		chooser->value = chooser->min_value;
	}
}


void int_chooser_decrease(Int_Chooser *chooser)
{
	if (chooser->value > chooser->min_value) {
		chooser->value--;
	}
	else if (chooser->value == 0 && chooser->cycle) {
		chooser->value = chooser->max_value;
	}
}
