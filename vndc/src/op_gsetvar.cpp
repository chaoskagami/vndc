#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements gsetvar vnds function.
 * gsetvar var mod val
 * Modifier is '=' '+' or '-' in script, simplify to ('-':-1, '=':0, '+':1).
 * MAY need to be UTF8 friendly
 */

void op_gsetvar(char* var, int *modifier, int *value) {
	if (GetData()->if_fail)
		return;

	if(*modifier == 0) {
		GetData()->g_flags[0][std::string(var)] = value[0];
	}
	else if (*modifier == -1) {
		GetData()->g_flags[0][std::string(var)] -= value[0];
	}
	else if (*modifier == 1) {
		GetData()->g_flags[0][std::string(var)] += value[0];
	}
}
