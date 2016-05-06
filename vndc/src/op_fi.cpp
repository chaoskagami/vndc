#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements fi vnds function.
 * if var op val
 */

void op_fi() {
    if(GetData()->if_fail != 0)
        GetData()->if_fail -= 1;
}
