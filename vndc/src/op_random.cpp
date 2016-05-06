#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements random vnds function.
 * random var low high
 */

bool seeded = false;

void op_random(char* var, int* low, int* high) {
    if (GetData()->if_fail != 0)
        return;
    if (seeded == false)
        srand(time(NULL));

    int num = *low + ( rand() % ( *high - *low ) );

    GetData()->s_flags[0][std::string(var)] = num;
}
