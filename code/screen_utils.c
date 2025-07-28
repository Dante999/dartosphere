#include "screen_utils.h"

#include <assert.h>

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



void update_and_draw_chooser(struct Screen *screen, struct Chooser_Bundle *bundle)
{

	for (size_t i=0; i < bundle->chooser_count; ++i) {

		Int_Chooser *chooser = &bundle->chooser_list[i];

		bool is_selected = (bundle->cursor.index == chooser->index);

		if (is_selected) {
			if (screen->key_pressed == chooser->key_increase) {
				int_chooser_increase(chooser);
			}
			else if (screen->key_pressed == chooser->key_decrease) {
				int_chooser_decrease(chooser);
			}
		}

		screen_draw_option(
			screen,
			bundle->width_chooser_name,
			bundle->width_chooser_value,
			chooser->index,
			is_selected,
			chooser->name,
			chooser->value_to_string(chooser->value));



	}


}


Int_Chooser *get_chooser_from_bundle(struct Chooser_Bundle *bundle, int index)
{
	for (size_t i=0; i < bundle->chooser_count; ++i) {
		Int_Chooser *chooser = &bundle->chooser_list[i];

		if (chooser->index == index) return chooser;
	}

	assert(false);
}



void score_input_append(struct Score_Input *input, char value)
{
	for (size_t i=0; i < sizeof(input->value)-1; ++i) {
		if (input->value[i] == '\0') {
			input->value[i+0] = value;
			input->value[i+1] = '\0';
		}
	}
}

bool score_input_drop_last(struct Score_Input *input)
{
	(void) input;
	return false;
}

