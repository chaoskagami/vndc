#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements setimg vnds function.
 * bgload file x y
 */

void op_setimg(char* file, int* x, int* y) {
	if (GetData()->if_fail || GetData()->ctx->GetQuit())
		return;

	// Fadeout not implemented yet.

	char path[400];
	memset(path, 0, 400);

	snprintf(path, 400, "foreground/%s", file);

	// Load displayable.
	UDisplayable* fg_add = new UDisplayable(GetData()->ctx, Normal, path);

	fg_add->SetXY(*x, *y);

	fg_add->Blit();

	// Delete fg_new. We're done with it.
	delete fg_add;
}
