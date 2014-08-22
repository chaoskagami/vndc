#include "Zero.hpp"

	// Creates a new Displayable.
	// The mode is already Normal if we do nothing, so this constructor doesn't have to change.
	UDisplayable::UDisplayable(ContextManager* cx, char* fname) {

		SDL_Surface* bitmap_tmp = IMG_Load(fname);

		if(!bitmap_tmp) {
			printf("[UDisplayable::Ctor] File could not be loaded, this->Error set.\n");
			Error = true;
		}
		else {
			this->x = 0;
			this->y = 0;
			this->loc.x = 0;
			this->loc.y = 0;
			this->ctx = cx;
			this->frameWidth = bitmap_tmp->w;
			this->bmp_w = bitmap_tmp->w;
			this->bmp_h = bitmap_tmp->h;

			// We still allocate the two arrays, regardless.

			// These values are unused, but we'll default them so it will operate normally.
			hitbox = (int*)calloc(sizeof(int), 4);

			// By default, it will fill this with 0, 0, W, H.
			// This will behave identically to a displayable.
			hitbox[0] = 0;
			hitbox[1] = 0;
			hitbox[2] = bitmap_tmp->w;
			hitbox[3] = bitmap_tmp->h;

			this->loc.w = bitmap_tmp->w;
			this->loc.h = bitmap_tmp->w;

			frame = (int*)calloc(sizeof(int), 4);

			frame[0] = 0;
			frame[1] = 0;
			frame[2] = cx->GetWidth();
			frame[3] = cx->GetHeight();

			// Determine if we're on an accelerated context. If so, we create a texture out of the bitmap.
			// Then we store what we'll use to the void* bitmap, either Tex or Surf.

			if (cx->Accelerated()) {
				SDL_Texture* tex = SDL_CreateTextureFromSurface(ctx->Renderer(), bitmap_tmp);
				this->bitmap = (void*)tex;
				SDL_FreeSurface(bitmap_tmp);
			}
			else {
				this->bitmap = (void*)bitmap_tmp;
			}
		}
		#ifdef DEBUG_OVERKILL
		printf("[UDisplayable~Ctor] accel:%d\n", cx->Accelerated());
		#endif
	}

	// Sets the Displayable's mode.
	UDisplayable::UDisplayable(ContextManager* cx, UDisplayableMode mode, char* fname) : UDisplayable(cx, fname) {
		this->dispMode = mode;
	}

	// Load from memory.
	UDisplayable::UDisplayable(ContextManager* cx, UDisplayableMode mode, void* memory, int mSize) {

		SDL_RWops* rwo = SDL_RWFromMem(memory, mSize);
		SDL_Surface* bitmap_tmp = IMG_LoadTyped_RW(rwo, 0, "PNG");
		if(rwo == NULL || bitmap_tmp == NULL) {
			printf("Loading RWops failed. %s", SDL_GetError());
			while(1);
			//exit(-1);
		}

		this->x = 0;
		this->y = 0;
		this->loc.x = 0;
		this->loc.y = 0;
		this->ctx = cx;
		this->frameWidth = bitmap_tmp->w;
		this->bmp_w = bitmap_tmp->w;
		this->bmp_h = bitmap_tmp->h;

		// We still allocate the two arrays, regardless.

		// These values are unused, but we'll default them so it will operate normally.
		hitbox = (int*)calloc(sizeof(int), 4);

		// By default, it will fill this with 0, 0, W, H.
		// This will behave identically to a displayable.
		hitbox[0] = 0;
		hitbox[1] = 0;
		hitbox[2] = bitmap_tmp->w;
		hitbox[3] = bitmap_tmp->h;

		this->loc.w = bitmap_tmp->w;
		this->loc.h = bitmap_tmp->w;

		frame = (int*)calloc(sizeof(int), 4);

		frame[0] = 0;
		frame[1] = 0;
		frame[2] = cx->GetWidth();
		frame[3] = cx->GetHeight();

		// Determine if we're on an accelerated context. If so, we create a texture out of the bitmap.
		// Then we store what we'll use to the void* bitmap, either Tex or Surf.

		if (cx->Accelerated()) {
			SDL_Texture* tex = SDL_CreateTextureFromSurface(ctx->Renderer(), bitmap_tmp);
			this->bitmap = (void*)tex;
			SDL_FreeSurface(bitmap_tmp);
		}
		else {
			this->bitmap = (void*)bitmap_tmp;
		}

		this->dispMode = mode;
	}

	// Sets the position on screen.

	void UDisplayable::SetXY(double x, double y) {
		if (Error)
			return;
		this->x = x;
		this->y = y;

		// Expand that logic above.

		if(this->x < 0)
			this->x = 0;
		if(this->y < 0)
			this->y = 0;

		if(frameIndex == -1) {
			if(this->x > frame[2] - this->bmp_w)
				this->x = (double)(frame[2] - this->bmp_w);
		}
		else {
			if(this->x > frame[2] - this->frameWidth)
				this->x = (double)(frame[2] - this->frameWidth);
		}

		if(this->y > frame[3] - this->bmp_h)
			this->y = (double)(frame[3] - this->bmp_h);

		this->loc.x = (int)this->x;
		this->loc.y = (int)this->y;

		#ifdef DEBUG_OVERKILL
		printf("[UDisplayable::SetXY] x:%d y:%d w:%d h:%d\n", frame[0], frame[1], frame[2], frame[3]);
		#endif
	}

	// Modifies the position on screen. Meant to avoid embedded retrievals.

	void UDisplayable::ModXY(double x, double y) {
		if (Error)
			return;

		this->x += x;
		this->y += y;

		// Expand that logic above.

		if(this->x < 0)
			this->x = 0;
		if(this->y < 0)
			this->y = 0;

		if(frameIndex == -1) {
			if(this->x > frame[2] - this->bmp_w)
				this->x = (double)(frame[2] - this->bmp_w);
		}
		else {
			if(this->x > frame[2] - this->frameWidth)
				this->x = (double)(frame[2] - this->frameWidth);
		}

		if(this->y > frame[3] - this->bmp_h)
			this->y = (double)(frame[3] - this->bmp_h);

		this->loc.x = (int)this->x;
		this->loc.y = (int)this->y;

		#ifdef DEBUG_OVERKILL
		printf("[UDisplayable::SetXY] x:%d y:%d w:%d h:%d\n", frame[0], frame[1], frame[2], frame[3]);
		#endif
	}

	// Get X coord.

	double UDisplayable::GetX() {
		if (Error)
			return 0;

		return this->x;
	}

	// Get Y coord.

	double UDisplayable::GetY() {
		if (Error)
			return 0;
		return this->y;
	}

	// Get aligned X coord.

	int UDisplayable::GetXI() {
		if (Error)
			return 0;
		return this->loc.x;
	}

	// Get aligned Y coord.

	int UDisplayable::GetYI() {
		if (Error)
			return 0;
		return this->loc.y;
	}

	// Blit to an associated context.

	void UDisplayable::Blit() {
		if (Error)
			return;

		// Copy loc and alter position.
		SDL_Rect loc_adj;
		loc_adj.x = loc.x + frame[0];
		loc_adj.y = loc.y + frame[1];
		loc_adj.w = frameWidth;
		loc_adj.h = bmp_h;

		#ifdef DEBUG_OVERKILL
		printf("[UDisplayable::Blit]\n");
		#endif

		if (frameIndex == -1) {
			ctx->Blit(bitmap, NULL, &loc_adj);
			return;
		}

		SDL_Rect frameClip;

		frameClip.x = frameWidth * frameIndex;
		frameClip.y = 0;
		frameClip.w = frameWidth;
		frameClip.h = bmp_h;

		ctx->Blit(bitmap, &frameClip, &loc_adj);
	}

	// Get SDL_Rect for collision calculation. In a base Displayable, it returns the image width.
	// In a derived class, it may return only a hitbox.

	int* UDisplayable::GetHitbox() {
		if (Error)
			return NULL;
		int* rect = (int*)calloc(sizeof(int), 4);

		rect[0] = hitbox[0] + (int)this->x;
		rect[1] = hitbox[1] + (int)this->y;
		rect[2] = hitbox[2];
		rect[3] = hitbox[3];

		return rect;
	}

	// Destroy bitmap.

	UDisplayable::~UDisplayable() {
		if (Error)
			return;
		if(ctx->Accelerated())
			SDL_DestroyTexture((SDL_Texture*)bitmap);
		else
			SDL_FreeSurface((SDL_Surface*)bitmap);
		free(hitbox);
		free(frame);
	}

	void UDisplayable::SetHitbox(int x, int y, int w, int h) {
		if (Error)
			return;
		if(dispMode == Normal || dispMode == Docked || dispMode == Anim || dispMode == AnimDocked)
			return;

		hitbox[0] = x;
		hitbox[1] = y;
		hitbox[2] = w;
		hitbox[3] = h;
	}

	void UDisplayable::SetDock(int x, int y, int w, int h) {
		if (Error)
			return;
		if(dispMode == Normal || dispMode == Hitbox || dispMode == Anim || dispMode == AnimHitbox)
			return;
		frame[0] = x;
		frame[1] = y;
		frame[2] = w;
		frame[3] = h;
	}

	void UDisplayable::NextFrame() {
		if (Error)
			return;
		if (dispMode == Normal || dispMode == Hitbox || dispMode == Docked || dispMode == HitboxDocked)
			return;

		++frameIndex;

		if(frameWidth * frameIndex >= bmp_w)
			frameIndex = 0;
	}

	void UDisplayable::NextFrame(int f) {
		if (Error)
			return;
		if (dispMode == Normal || dispMode == Hitbox || dispMode == Docked || dispMode == HitboxDocked)
			return;
		frameIndex += f;

		if(frameWidth * frameIndex >= bmp_w)
			frameIndex = 0;
	}

	void UDisplayable::ResetFrame() {
		if (Error)
			return;
		if (dispMode == Normal || dispMode == Hitbox || dispMode == Docked || dispMode == HitboxDocked)
			return;
		frameIndex = 0;
	}

	void UDisplayable::SetFrame(int f) {
		if (Error)
			return;
		if (dispMode == Normal || dispMode == Hitbox || dispMode == Docked || dispMode == HitboxDocked)
			return;

		frameIndex = f;
	}

	void UDisplayable::SetFrameWidth(int frameW) {
		if (Error)
			return;
		if (dispMode == Normal || dispMode == Hitbox || dispMode == Docked || dispMode == HitboxDocked)
			return;
		this->frameWidth = frameW;

		#ifdef DEBUG_OVERKILL
		printf("[UDisplayable::SetFrameWidth] fW:%d\n", frameW);
		#endif
	}
