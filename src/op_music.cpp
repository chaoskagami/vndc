#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements music vnds function.
 * music file
 */

void op_music(char* file) {
    if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
        return;

    memset(GetData()->current_music, 0, 400);

    snprintf(GetData()->current_music, 400, "sound/%s", file);

    // Halt command
    if (!strcmp(file, "~")) {
        GetData()->ctx->Audio()->FlushMusic();
    }
    // Play command
    else {
        GetData()->ctx->Audio()->FlushMusic();

        int index = GetData()->ctx->Audio()->LoadMusic(GetData()->current_music);

        GetData()->ctx->Audio()->PlayMusicLoop(index);
    }
}
