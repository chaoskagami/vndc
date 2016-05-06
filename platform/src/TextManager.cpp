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
    if (text == NULL || fonts[current_font] == NULL) {
        fprintf(stderr, "[WARN] Null text or font passed.\n");
        return;
    }


    SDL_Surface *sf1 = NULL, *sf2 = NULL;

    sf1 = TTF_RenderUTF8_Blended(fonts[current_font], text, color);

    if(outline) {
        // Invert normal color.
        SDL_Color n_color;
        n_color.r = 255 - color.r;
        n_color.g = 255 - color.g;
        n_color.b = 255 - color.b;
        n_color.a = color.a;

        // Outline, so increase size.
        TTF_SetFontOutline(fonts[current_font], outline_px);
        sf2 = TTF_RenderUTF8_Blended(fonts[current_font], text, n_color);

        // Disable Outline.
        TTF_SetFontOutline(fonts[current_font], 0);
    }

    if(outline) {
        UDisplayable* r_sf2 = new UDisplayable(ctx, Normal, sf2);
        r_sf2->SetOverlay(true);
        r_sf2->SetXY(x-outline_px, y-outline_px);

        r_sf2->Blit();
        
        delete r_sf2;
    }

    UDisplayable* r_sf1 = new UDisplayable(ctx, Normal, sf1);
    r_sf1->SetOverlay(true);
    r_sf1->SetXY(x, y);

    r_sf1->Blit();

    delete r_sf1;
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

// This splits a screen so that each line will fit to a width.
// Note that this will modify the string in place by inserting \0
// So your string will likely not be usable as before.
void TextManager::SplitStringByWidth(char* string, int max_w, int* OUT_num, char*** OUT_ptrs) {
    if(TestLen(string) > max_w) {
        /* new algo */
        char** ptrs = NULL;
        int lines = 0;
        int len = strlen(string);

        int counted = 0;

        while(counted < len) {
            char* pt_start = &string[counted];
            char* pt_end = &pt_start[strlen(pt_start)];

            while(pt_end > pt_start && TestLen(pt_start) > max_w) {
                *pt_end = ' ';
                --pt_end;
                while (*pt_end != ' ' && pt_end > pt_start) --pt_end;

                *pt_end = '\0';
            }

            #ifdef DEBUG_OVERKILL
            printf("Reduced line %d: %s\n", lines, pt_start);
            #endif

            ptrs = (char**)realloc(ptrs, sizeof(char*)*(lines+1));

            ptrs[lines] = pt_start;

            counted += strlen(pt_start) + 1;

            ++lines;
        }

        *OUT_num = lines;

        *OUT_ptrs = ptrs;
    }
    else {
        char** ptrs = (char**)calloc(sizeof(char*), 1);
        ptrs[0] = string;

        // We pass thru values anyways, regardless of there being one line.
        *OUT_num = 1;
        *OUT_ptrs = ptrs;
    }
}
