#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements bgload vnds function.
 * bgload file [fadetime] (default 16)
 */

void op_bgload(char* file, int* fadetime) {
    if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
        return;

    memset(GetData()->current_bg, 0, 400);

    snprintf(GetData()->current_bg, 400, "background/%s", file);

    // Load displayable.

    UDisplayable* disp = new UDisplayable(GetData()->ctx, GetData()->current_bg);

    // SDL_Surface* sfc = IMG_Load(GetData()->current_bg);

    uint8_t transp_incr = 16;
    if(fadetime != NULL)
        transp_incr = 255 / *fadetime;

    // FIXME - Implement proper fading out functionality.

    int delay = 1;
    for(int tr = 0; tr < 255; tr += transp_incr) {        
        disp->Blit();
        op_delay(&delay);
    }
}
