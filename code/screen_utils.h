#ifndef SCREEN_UTILS_H
#define SCREEN_UTILS_H

#include <stdbool.h>
#include "screen.h"

typedef struct {
	int index;
	int max_index;
	bool cycle;
} Line_Cursor;

typedef struct {
	int index;
	char name[255];
	int value;
	int min_value;
	int max_value;
	bool cycle;
	enum Screen_Key key_increase;
	enum Screen_Key key_decrease;
	const char *(*value_to_string)(int value);

} Int_Chooser;

struct Chooser_Bundle {
	int width_chooser_name;
	int width_chooser_value;
	Line_Cursor cursor;
	Int_Chooser *chooser_list;
	size_t       chooser_count;
};

void line_cursor_up(Line_Cursor *cursor);
void line_cursor_down(Line_Cursor *cursor);
void line_cursor_reset(Line_Cursor *cursor);


void update_and_draw_chooser(struct Screen *screen, struct Chooser_Bundle *bundle);
Int_Chooser *get_chooser_from_bundle(struct Chooser_Bundle *bundle, int index);

void int_chooser_increase(Int_Chooser *chooser);
void int_chooser_decrease(Int_Chooser *chooser);


#endif // SCREEN_UTILS_H
