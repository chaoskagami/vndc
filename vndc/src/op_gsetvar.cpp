#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements gsetvar vnds function.
 * gsetvar var mod val
 * Modifier is '=' '+' or '-' in script, simplify to ('-':-1, '=':0, '+':1).
 * is apparently UTF8 friendly
 * Also, for now I simply redirect to setvar. Global values are only set
 * by definition on an end-game event for replays, so when the interpreter hits
 * 'the end' it loops back to the title.
 * So setting local vars is okay, since they'll be reloaded across playthrus.
 */

void op_gsetvar(char* var, int *modifier, int *value) {
	if (GetData()->if_fail != 0)
		return;

	op_setvar(var, modifier, value);
}
