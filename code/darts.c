#include "darts.h"

char multiplicator_as_char(Multiplicator m)
{
	switch(m) {
		case DARTS_SINGLE : return 'S';
		case DARTS_DOUBLE : return 'D';
		case DARTS_TRIPPLE: return 'T';
	}

	return '?';
}

