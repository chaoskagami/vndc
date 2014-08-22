#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements bgload vnds function.
 * bgload file [fadetime] (default 16)
 */

void op_bgload(char* file, int* fadetime) {
	if (GetData()->if_fail || GetData()->ctx->GetQuit())
		return;

	// Fadeout not implemented yet.

	memset(GetData()->current_bg, 0, 400);

	snprintf(GetData()->current_bg, 400, "background/%s", file);

	//printf("Attempt to load file %s as BG\n", path);

	// Load displayable.
	SDL_Surface* sfc = IMG_Load(GetData()->current_bg);

	uint8_t transp_incr = 16;
	if(fadetime != NULL)
		transp_incr = 255 / *fadetime;

	// Transition effect.

	SDL_Texture* tx = NULL;
	tx = SDL_CreateTextureFromSurface(GetData()->ctx->Renderer(), sfc);
	SDL_SetTextureBlendMode(tx, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(sfc);

	int delay = 1;
	for(int tr = 0; tr < 255; tr += transp_incr) {
		SDL_SetTextureAlphaMod(tx, tr);
		GetData()->ctx->Blit(tx, NULL, NULL);
		op_delay(&delay);
	}

	SDL_DestroyTexture(tx);
}
