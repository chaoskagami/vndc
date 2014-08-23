#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements if vnds function.
 * if var op val
 * op is a c-style compare
 * ('<=':0 '<':1 '==':2 '!=':3 '>': '>=':5)
 */

void op_if(char* var, int* op, int* val) {
	if (GetData()->if_fail != 0) {
		GetData()->if_fail += 1;
		return;
	}

	int var_val = GetData()->s_flags[0][std::string(var)];

	//printf("op_if(%s, %d, %d)\n", var, op[0], val[0]);
	//printf("GetData()->s_flags[0][%s] = %d\n", var, GetData()->s_flags[0][std::string(var)]);

	switch (op[0]) {
		case 0:
			if ( !(var_val <= val[0]) )
				GetData()->if_fail += 1;
			break;
		case 1:
			if ( !(var_val < val[0]) )
				GetData()->if_fail += 1;
			break;
		case 2:
			if ( !(var_val == val[0]) )
				GetData()->if_fail += 1;
			break;
		case 3:
			if ( !(var_val != val[0]) )
				GetData()->if_fail += 1;
			break;
		case 4:
			if ( !(var_val > val[0]) )
				GetData()->if_fail += 1;
			break;
		case 5:
			if ( !(var_val >= val[0]) )
				GetData()->if_fail += 1;
			break;
	}
}
