#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements delay vnds function.
 * delay frames
 */

void op_delay(int* frames) {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;
	for(int i = 0; i < *frames; i++) {
		// This is one frame.
		GetData()->ctx->StartSync();
		GetData()->ctx->EndSync();
	}
}
