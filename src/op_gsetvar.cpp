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
 *
 * As an exception, setvar ~ ~ will ignore all vars prefixed with g
 * because these are globals.
 *
 * You can set global vars with setvar because my implementation just
 * works different here, but still.
 *
 * Any local vars will obviously be nuked by setvar ~ ~.
 */

void op_gsetvar(char* var, int *modifier, char *value) {
    if (GetData()->if_fail != 0)
        return;

    op_setvar(var, modifier, value);
}
