#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements gsetvar vnds function.
 * gsetvar var mod val
 * Modifier is '=' '+' or '-' in script, simplify to ('-':-1, '=':0, '+':1).
 * MAY need to be UTF8 friendly
 */

void op_setvar(char* var, int* modifier, int* value) {
	if (GetData()->if_fail != 0)
		return;

	if(*modifier == 0) {
		GetData()->s_flags[0][std::string(var)] = value[0];
	}
	else if (*modifier == -1) {
		GetData()->s_flags[0][std::string(var)] -= value[0];
	}
	else if (*modifier == 1) {
		GetData()->s_flags[0][std::string(var)] += value[0];
	}
	else if (*modifier == -2) {
		// There's a rare case on program start where a resetall
		// happens.

		if(!strcmp(var, "~")) {
			// We'll handle it by doing a delete & new on s_local
			GetData()->s_flags[0].clear();
			return;
		}

		GetData()->s_flags[0][std::string(var)] = 0;
	}
}
