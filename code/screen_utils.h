#ifndef SCREEN_UTILS_H
#define SCREEN_UTILS_H

#include <stdbool.h>

typedef struct {
	int index;
	int max_index;
	bool cycle;
} Line_Cursor;

typedef struct {
	char name[255];
	int value;
	int min_value;
	int max_value;
	bool cycle;
} Int_Chooser;

void line_cursor_up(Line_Cursor *cursor);
void line_cursor_down(Line_Cursor *cursor);
void line_cursor_reset(Line_Cursor *cursor);


void int_chooser_increase(Int_Chooser *chooser);
void int_chooser_decrease(Int_Chooser *chooser);


#endif // SCREEN_UTILS_H
