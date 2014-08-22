#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements sound vnds function.
 * sound file [times]
 */

void op_sound(char* file, int* times) {
	if (GetData()->if_fail || GetData()->ctx->GetQuit())
		return;

	// Fadeout not implemented yet.

	// Halt command
	if (!strcmp(file, "~")) {
		GetData()->ctx->Audio()->FlushSfx();
		return;
	}
	// Play command
	else {
		int count = 1;
		if (times != NULL)
			count = *times;

		char path[400];
		memset(path, 0, 400);

		snprintf(path, 400, "sound/%s", file);

		//printf("[op_sound] path: '%s'\n", path);

		// Load displayable.
		int sfxi = GetData()->ctx->Audio()->LoadSfx(path);

		if (count == -1) {
			GetData()->ctx->Audio()->PlaySfx(sfxi, count);
		}
		else {
			GetData()->ctx->Audio()->PlaySfx(sfxi, count-1);
		}
	}
}
