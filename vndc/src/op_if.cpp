#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements if vnds function.
 * if var op val
 * op is a c-style compare
 * ('<=':0 '<':1 '==':2 '!=':3 '>': '>=':5)
 */

void op_if(char* left, int* op, char* right) {
    if (GetData()->if_fail != 0) {
        GetData()->if_fail += 1;
        return;
    }

    int r_val = 0, ret = 0;
    
    ret = sscanf(right, "%d", &r_val);

    if(ret == 0 && GetData()->vndc_enabled) { // This is a var name. We couldn't scan a number.
        r_val = GetData()->s_flags[0][std::string(right)];
    }

    int l_val = GetData()->s_flags[0][std::string(left)];

    //printf("op_if(%s, %d, %d)\n", var, op[0], val[0]);
    //printf("GetData()->s_flags[0][%s] = %d\n", var, GetData()->s_flags[0][std::string(var)]);

    switch (op[0]) {
        case 0:
            if ( !(l_val <= r_val) )
                GetData()->if_fail += 1;
            break;
        case 1:
            if ( !(l_val < r_val) )
                GetData()->if_fail += 1;
            break;
        case 2:
            if ( !(l_val == r_val) )
                GetData()->if_fail += 1;
            break;
        case 3:
            if ( !(l_val != r_val) )
                GetData()->if_fail += 1;
            break;
        case 4:
            if ( !(l_val > r_val) )
                GetData()->if_fail += 1;
            break;
        case 5:
            if ( !(l_val >= r_val) )
                GetData()->if_fail += 1;
            break;
    }
}
