#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements sound vnds function.
 * sound file [times]
 */

void op_sound(char* file, int* times) {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;

	// Fadeout not implemented yet.

	// Halt command
	if (!strcmp(file, "~")) {
		GetData()->ctx->Audio()->FlushSfx();
		return;
	}
	// Extension - Voiced auto-stop. If the next line contains quotes,
	// it is voiced. We set a flag in response. If the flag is already
	// set - e.g. the sound was a voice as well, we stop the previous
	// sound before playing this one.

	if(GetData()->vndc_enabled &&
	   (GetData()->next_line[6] == '"' ||
	    GetData()->next_line[strlen(GetData()->next_line) - 1] == '"')) {
		if(GetData()->is_spoken_line == true) {
			GetData()->ctx->Audio()->FlushSfx();
		}
		GetData()->is_spoken_line = true;
	}
	else
		GetData()->is_spoken_line = false;


	// Play command
	int count = 1;
	if (times != NULL)
		count = *times;

	char path[400];
	memset(path, 0, 400);

	snprintf(path, 400, "sound/%s", file);

	// Load sfx.
	int sfxi = GetData()->ctx->Audio()->LoadSfx(path);

	if (count == -1) {
		GetData()->ctx->Audio()->PlaySfx(sfxi, count);
	}
	else {
		GetData()->ctx->Audio()->PlaySfx(sfxi, count-1);
	}
}
