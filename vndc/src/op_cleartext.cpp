#include "Zero.hpp"

#include "Funcs.hpp"

void ct_transwindow() {

	int width_dr = (GetData()->render_x2 - GetData()->render_x1 + 20);
	int height_dr = (GetData()->render_y2 - GetData()->render_y1 + 30 + 20);

	// if(! GetData()->ctx->Accelerated()) return;
	SDL_Surface* sfc = SDL_CreateRGBSurface(0, width_dr, height_dr, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
	SDL_FillRect(sfc, NULL, SDL_MapRGBA(sfc->format, 0, 0, 0, 100));
	if(!GetData()->ctx->Accelerated()) {
	}

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = sfc->w;
	src.h = sfc->h;

	SDL_Rect dst;
	dst.x = GetData()->render_x1 - 10;
	dst.y = GetData()->render_y1 - 10;
	dst.w = src.w;
	dst.h = src.h;

	if(GetData()->ctx->Accelerated()) {
		SDL_Texture* dim = SDL_CreateTextureFromSurface(GetData()->ctx->Renderer(), sfc);
		//SDL_SetTextureBlendMode(dim, SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(dim, 128);

		GetData()->ctx->OverlayBlit(dim, &src, &dst, NULL);

		SDL_DestroyTexture(dim);
	}
	else {
		GetData()->ctx->OverlayBlit(sfc, &src, &dst, NULL);		
	}

	SDL_FreeSurface(sfc);

}

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
	ct_transwindow();
}
