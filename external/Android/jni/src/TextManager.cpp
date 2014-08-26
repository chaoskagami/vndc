#include "Zero.hpp"

TextManager::TextManager(ContextManager* ct) {

	fonts = NULL;
	current_font = 0;
	fonts_loaded = 0;
	outline = false;
	outline_px = 0;

	this->ctx = ct;

	if(TTF_Init() == -1) {
		printf("TTF Init failed. %s\n", TTF_GetError());
		exit(-8);
	}

	LoadFont((char*)"default.ttf", 20);

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

}

TextManager::~TextManager() {

	while(fonts_loaded > 0) {
		TTF_CloseFont(fonts[fonts_loaded-1]);
		--fonts_loaded;
	}

	free(fonts);

	TTF_Quit();
}

int TextManager::LoadFont(char* fname, int size) {

	++fonts_loaded;

	fonts = (TTF_Font**)realloc(fonts, sizeof(TTF_Font*) * fonts_loaded);

	if ( !(fonts[fonts_loaded - 1] = TTF_OpenFont(fname, size) ) ) {
		printf("Font Open Failed. Load attempt: %s. Msg: %s\n", fname, TTF_GetError());
	}

	TTF_SetFontHinting(fonts[fonts_loaded - 1], TTF_HINTING_NONE);

	return fonts_loaded - 1;
}


// Implement properly later with wrapped coordinates.
void TextManager::Render(char* text) {
	Render(text, 0, 0);
}

void TextManager::Render(char* text, int x, int y) {
	if (text == NULL || fonts[current_font] == NULL)
		return;

	SDL_Surface *sf1 = NULL, *sf2 = NULL;
	sf1 = TTF_RenderUTF8_Blended(fonts[current_font], text, color);
	if(outline) {
		SDL_Color n_color;
		n_color.r = 255 - color.r;
		n_color.g = 255 - color.g;
		n_color.b = 255 - color.b;
		n_color.a = color.a;
		TTF_SetFontOutline(fonts[current_font], outline_px);
		sf2 = TTF_RenderUTF8_Blended(fonts[current_font], text, n_color);
		TTF_SetFontOutline(fonts[current_font], 0);
	}

	SDL_Texture *tmp1 = NULL, *tmp2 = NULL;

	tmp1 = SDL_CreateTextureFromSurface(ctx->Renderer(), sf1);
	if(outline)
		tmp2 = SDL_CreateTextureFromSurface(ctx->Renderer(), sf2);

	SDL_Rect src, dst, src2, dst2;
	src.x = 0;
	src.y = 0;
	src.w = sf1->w;
	src.h = sf1->h;

	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	if(outline) {
		src2.x = 0;
		src2.y = 0;
		src2.w = sf2->w;
		src2.h = sf2->h;

		dst2.x = x - (outline_px);
		dst2.y = y - (outline_px);
		dst2.w = src2.w;
		dst2.h = src2.h;
		SDL_FreeSurface(sf2);
		ctx->OverlayBlit(tmp2, &src2, &dst2);
		SDL_DestroyTexture(tmp2);
	}

	SDL_FreeSurface(sf1);
	ctx->OverlayBlit(tmp1, &src, &dst);
	SDL_DestroyTexture(tmp1);

}

int TextManager::TestLen(char* text) {
	int w, h;
	TTF_SizeText(fonts[current_font], text, &w, &h);

	return w;
}

void TextManager::Outline(int pixels) {
	if(pixels == 0) {
		outline = false;
	}
	else {
		outline = true;
		outline_px = pixels;
	}
}

void TextManager::SetColor(int r, int g, int b, int a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void TextManager::SetFontUsed(int index)
{
	current_font = index;
}
