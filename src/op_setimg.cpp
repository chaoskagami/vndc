#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements setimg vnds function.
 * setimg file x y
 */

void op_setimg(char* file, int* x, int* y) {
    if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
        return;

    // Fadeout not implemented yet.

    char path[400];
    memset(path, 0, 400);

    snprintf(path, 400, "foreground/%s", file);

    // Load displayable.
    UDisplayable* fg_add = new UDisplayable(GetData()->ctx, Normal, path);

    // Centered NDS adapted
    double adp_x = ((double)x[0]) * (GetData()->screen_w / 256);
    double adp_y = ((double)y[0]) * (GetData()->screen_h / 192);
    fg_add->SetXY((int)adp_x, (int)adp_y);

    // Raw
    // fg_add->SetXY(x[0], y[0]);

    fg_add->Blit();

    // Delete fg_new. We're done with it.
    delete fg_add;
}
