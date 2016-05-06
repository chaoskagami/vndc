/* op_save - saves the game to a save.
   Params; name.
 */

#include "Zero.hpp"
#include "Funcs.hpp"

void op_save(char* name) {
    DumpSave(name); // Just a wrapper. No harm done.
            // TODO - Replace all dumpsave calls with this and move function here
}

