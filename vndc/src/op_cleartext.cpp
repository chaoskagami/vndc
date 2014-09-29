#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements cleartext vnds function.
 * cleartext mod
 * However, we ignore mod for now.
 */

void op_cleartext() {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;

	GetData()->text_x = GetData()->render_x1;
	GetData()->text_y = GetData()->render_y1;
	GetData()->ctx->ClearOverlay();

	// Dim transparent overlay
	GetData()->text_box_base->Blit();
}
