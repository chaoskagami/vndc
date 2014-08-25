#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements gsetvar vnds function.
 * gsetvar var mod val
 * Modifier is '=' '+' or '-' in script, simplify to ('-':-1, '=':0, '+':1).
 * MAY need to be UTF8 friendly
 */

void op_setvar(char* var, int* modifier, char* value) {
	if (GetData()->if_fail != 0)
		return;

	int value_r = 0, ret;
	if(value == NULL)
		ret = 0;
	else
		ret = sscanf(value, "%d", &value_r);

	if(ret == 0) { // value is a variable not a number
		if(*modifier == 0) {
			GetData()->s_flags[0][std::string(var)] = GetData()->s_flags[0][std::string(value)];
		}
		else if (*modifier == -1) {
			GetData()->s_flags[0][std::string(var)] -= GetData()->s_flags[0][std::string(value)];
		}
		else if (*modifier == 1) {
			GetData()->s_flags[0][std::string(var)] += GetData()->s_flags[0][std::string(value)];
		}
	}
	else {
		if(*modifier == 0) {
			GetData()->s_flags[0][std::string(var)] = value_r;
		}
		else if (*modifier == -1) {
			GetData()->s_flags[0][std::string(var)] -= value_r;
		}
		else if (*modifier == 1) {
			GetData()->s_flags[0][std::string(var)] += value_r;
		}
		else if (*modifier == -2) {
			// There's a rare case on program start where a resetall
			// happens.

			if(!strcmp(var, "~")) {
				// We'll handle it by searching through and deleting all non-G values.
				// g values are global, so they by definition survive this.
	
				if(!GetData()->s_flags[0].empty()) {
					std::map<std::string, int>::iterator item = GetData()->s_flags[0].begin();
					while(item != GetData()->s_flags[0].end()) {
						if(item->first.c_str()[0] != 'g')
							GetData()->s_flags[0].erase(item++);
						else
							item++;
					}
				}
				return;
			}

			GetData()->s_flags[0][std::string(var)] = 0;
		}
	}
}
