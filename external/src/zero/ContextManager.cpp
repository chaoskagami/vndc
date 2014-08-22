#include "Zero.hpp"


	// Initializes SDL and saves stuff into the context.

	ContextManager::ContextManager(int scr_width, int scr_height, bool fulls, bool accel) {
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
			#ifdef DEBUG_OVERKILL
			printf("[ContextManager~Ctor] SDL Init error w/ VIDEO|AUDIO. Msg rep: %s\n", SDL_GetError());
			#else
			printf("Error initializing SDL. Info:%s\n", SDL_GetError());
			#endif
		}
		else {
			this->InitWindow(scr_width, scr_height, fulls, accel);
		}

		audioMgr = new AudioManager();
	}

	// Delays window creation; Use InitWindow before doing anything.

	ContextManager::ContextManager() {
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
			#ifdef DEBUG_OVERKILL
			printf("[ContextManager~Ctor] SDL Init error w/ VIDEO|AUDIO. Msg rep: %s\n", SDL_GetError());
			#else
			printf("Error initializing SDL. Info:%s\n", SDL_GetError());
			#endif
		}

		audioMgr = new AudioManager();
	}

	// Creates a window.

	void ContextManager::InitWindow(int width, int height, bool fulls, bool accel) {
		this->WIN_width = width;
		this->WIN_height = height;
		this->LOG_width = width;
		this->LOG_height = height;

		#ifdef DEBUG_OVERKILL
		printf("[ContextManager::InitWindow] Params: w:%d h:%d f:%d a:%d\n", width, height, fulls, accel);
		#endif

		if(accel) {
			_InitWindowAC(width, height, fulls);
			this->texture_o = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
			SDL_SetTextureBlendMode(this->texture_o, SDL_BLENDMODE_BLEND);
		}
		else {
			_InitWindowSW(width, height, fulls);
			this->surface_o = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		}

		txtMgr = new TextManager(this);
	}

	void ContextManager::_InitWindowSW(int width, int height, bool fulls) {
		// Add title setting function.
		this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);
			if(!SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED )) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}
		}
	}

	void ContextManager::_InitWindowAC(int width, int height, bool fulls) {
		this->accelerate = true;

		this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			this->renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
			if(this->renderer == NULL) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}
			else {
				this->logicalRender = true;

				this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
			}
		}
	}

	// Init window with a logical size and a real one. Determines which backend to use.
	void ContextManager::InitWindowLogical(int width_win, int height_win, int width_log, int height_log, bool fulls, bool accel) {
		this->LOG_width = width_log;
		this->LOG_height = height_log;
		this->WIN_width = width_win;
		this->WIN_height = height_win;

		#ifdef DEBUG_OVERKILL
		printf("[ContextManager::InitWindowLogical] phyw:%d phyh:%d logw:%d logh:%d f:%d a:%d\n", width_win, height_win, width_log, height_log, fulls, accel);
		#endif

		if (accel) {
			_InitWindowLogicalAC(width_win, height_win, width_log, height_log, fulls);
			this->texture_o = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_win, height_win);
			SDL_SetTextureBlendMode(this->texture_o, SDL_BLENDMODE_BLEND);
		}
		else {
			_InitWindowLogicalSW(width_win, height_win, width_log, height_log, fulls);
			this->surface_o = SDL_CreateRGBSurface(0, width_win, height_win, 32, 0, 0, 0, 0);
		}

		txtMgr = new TextManager(this);
	}

	// Init window with a logical size and a real one.
	void ContextManager::_InitWindowLogicalSW(int width_win, int height_win, int width_log, int height_log, bool fulls) {
		this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_SHOWN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			this->renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
			if(this->renderer == NULL) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}
			else {
				this->logicalRender = true;

				this->surface = SDL_CreateRGBSurface(0, width_log, height_log, 32, 0, 0, 0, 0);
			}
		}
	}

		// Init window with a logical size and a real one.
	void ContextManager::_InitWindowLogicalAC(int width_win, int height_win, int width_log, int height_log, bool fulls) {
		this->accelerate = true;

		this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_SHOWN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			this->renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
			if(this->renderer == NULL) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}
			else {
				this->logicalRender = true;

				this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_log, height_log);
			}
		}
	}

	// Clears the display to black.

	void ContextManager::Clear() {
		if(this->StartQuit) return;

		if (accelerate) {
			if (logicalRender) {
				SDL_SetRenderTarget(renderer, texture);
				SDL_RenderClear(renderer);
				SDL_SetRenderTarget(renderer, NULL);
			}

			SDL_SetRenderTarget(renderer, texture_o);
			SDL_RenderClear(renderer);
			SDL_SetRenderTarget(renderer, NULL);

			SDL_RenderClear(renderer);
		}
		else {
			if (logicalRender)
				SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0x0, 0x0, 0x0));
			SDL_FillRect(this->surface_o, NULL, SDL_MapRGB(this->surface_o->format, 0x0, 0x0, 0x0));

			SDL_FillRect(SDL_GetWindowSurface(this->window), NULL, SDL_MapRGB(SDL_GetWindowSurface(this->window)->format, 0x0, 0x0, 0x0));
		}
	}

	// Clears the display to black.

	void ContextManager::ClearOverlay() {
		if(this->StartQuit) return;

		if (accelerate) {
			SDL_SetRenderTarget(renderer, texture_o);
			SDL_RenderClear(renderer);
			SDL_SetRenderTarget(renderer, NULL);
		}
		else {
			SDL_FillRect(this->surface_o, NULL, SDL_MapRGB(this->surface_o->format, 0x0, 0x0, 0x0));
		}
	}

	// Flush any changes in the surface to screen

	void ContextManager::Flush() {
		if(this->StartQuit) return;
		if (accelerate) {
			if(logicalRender) {
				SDL_Rect dst, src;

				src.x = 0;
				src.y = 0;
				src.w = LOG_width;
				src.h = LOG_height;

				dst.x = 0;
				dst.y = 0;
				dst.w = WIN_width;
				dst.h = WIN_height;

				SDL_RenderCopy(renderer, texture, &src, &dst);
			}

			SDL_RenderCopy(renderer, texture_o, NULL, NULL);

			SDL_RenderPresent(renderer);
		}
		else {
			if(logicalRender)
				SDL_BlitScaled(this->surface, NULL, SDL_GetWindowSurface(this->window), NULL);

			SDL_BlitScaled(this->surface_o, NULL, SDL_GetWindowSurface(this->window), NULL);

			SDL_UpdateWindowSurface(this->window);
		}
	}

	// Return the internal surface for external blits.
	SDL_Surface* ContextManager::Surface() {
		if(this->StartQuit) return NULL;
		if(accelerate) return NULL;

		if( logicalRender )
			return this->surface;

		return SDL_GetWindowSurface(this->window);
	}

	// return the renderer.
	SDL_Renderer* ContextManager::Renderer() {
		return renderer;
	}

		// return the renderer.
	bool ContextManager::Accelerated() {
		return accelerate;
	}

	// Blit data via cast. inp should be a SDL_Surface or SDL_Texture.
	void ContextManager::Blit(void* inp, SDL_Rect* src, SDL_Rect* dst) {
		if(accelerate) {
			if (logicalRender) SDL_SetRenderTarget(renderer, texture);

			SDL_RenderCopy(renderer, (SDL_Texture*)inp, src, dst);

			if (logicalRender) SDL_SetRenderTarget(renderer, NULL);
		}
		else {
			SDL_BlitSurface((SDL_Surface*)inp, src, surface, dst);
		}
	}

	// Blit data to overlay via cast. inp should be a SDL_Surface or SDL_Texture.
	void ContextManager::OverlayBlit(void* inp, SDL_Rect* src, SDL_Rect* dst) {
		// Recalculate src and dst based on upscale.
		double OUTW_Ratio, OUTH_Ratio;
		OUTW_Ratio = (double)WIN_width  / (double)LOG_width;
		OUTH_Ratio = (double)WIN_height / (double)LOG_height;

		dst->x = (int)(((double)dst->x) * OUTW_Ratio);
		dst->y = (int)(((double)dst->y) * OUTH_Ratio);
		dst->w = (int)(((double)dst->w) * OUTW_Ratio);
		dst->h = (int)(((double)dst->h) * OUTH_Ratio);

		if(accelerate) {
			if (logicalRender) SDL_SetRenderTarget(renderer, texture_o);

			SDL_RenderCopy(renderer, (SDL_Texture*)inp, src, dst);

			if (logicalRender) SDL_SetRenderTarget(renderer, NULL);
		}
		else {
			SDL_BlitSurface((SDL_Surface*)inp, src, surface_o, dst);
		}
	}

	// Gets width of display
	int ContextManager::GetWidth() {
		return this->LOG_width;
	}

	// Gets height of display
	int ContextManager::GetHeight() {
		return this->LOG_height;
	}

	// Checks input events for matches and run callbacks.
	bool ContextManager::Input() {
		if(this->StartQuit) return this->StartQuit;

		SDL_Event evt;
		for(int i = 0; i < this->inputmode && SDL_PollEvent(&evt); i++) {
			for(int j = 0; j < inputRegistered; j++) {
				if(inputMappings[j] == evt.key.keysym.sym) {
					bool down = false;
					if(evt.type == SDL_KEYDOWN)
						down = true;
					inputStates[j] = down;
					if((evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP) && inputCallbacks[j] != NULL) {
						(inputCallbacks[j])(down);
					}
				}
			}
			if((evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP) && mouseCallback != NULL) {
				if(evt.button.button == SDL_BUTTON_LEFT) {
					(mouseCallback)(evt.button.x, evt.button.y, (evt.type == SDL_MOUSEBUTTONDOWN), true, false, false);
				}
				if(evt.button.button == SDL_BUTTON_MIDDLE) {
					(mouseCallback)(evt.button.x, evt.button.y, (evt.type == SDL_MOUSEBUTTONDOWN), false, true, false);
				}
				if(evt.button.button == SDL_BUTTON_RIGHT) {
					(mouseCallback)(evt.button.x, evt.button.y, (evt.type == SDL_MOUSEBUTTONDOWN), false, false, true);
				}
			}
		}

		return this->StartQuit;
	}

	// Changes the Input Mode used.
	void ContextManager::InputMode(InMode in) {
		inputmode = in;
	}

	// Register an abstract handler. Returns an int corresponding to the index for the mapping.
	int ContextManager::RegisterInput(SDL_Keycode key, GameCallback func) {

		// Apply input mappings.

		++inputRegistered;

		// allocate space for current keys+1.
		inputMappings = (SDL_Keycode*)realloc(inputMappings, sizeof(SDL_Keycode) * inputRegistered);
		inputCallbacks = (GameCallback*)realloc(inputCallbacks, sizeof(GameCallback) * inputRegistered);
		inputStates = (bool*)realloc(inputStates, sizeof(bool) * inputRegistered);

		// Actually apply the new callback
		inputMappings[inputRegistered-1] = key;
		inputCallbacks[inputRegistered-1] = func;
		inputStates[inputRegistered-1] = false;

		// Return the index.
		return inputRegistered-1;
	}

	void ContextManager::RegisterMouse(MouseCallback func) {
		mouseCallback = func;
	}

	// Get input via abstract ID.
	bool ContextManager::GetInput(int index) {
		return inputStates[index];
	}

	// Remap abstract key.
	void ContextManager::RemapKey(int index, SDL_Keycode to) {
		// Check if something is already mapped to 'to'.
		int reg = -1;
		for(int i = 0; i < inputRegistered; i++) {
			if(inputMappings[i] == to) {
				reg = i;
			}
		}
		// Remap, swap if necessary.
		if(reg > -1)
			inputMappings[reg] = inputMappings[index];
		inputMappings[index] = to;
	}

	// Set Quit flag.
	void ContextManager::SetQuit() {
		this->StartQuit = true;
	}

	bool ContextManager::GetQuit() {
		return this->StartQuit;
	}

	// Destroy context.

	ContextManager::~ContextManager() {
		delete audioMgr;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	// Gets the audio manager.
	AudioManager* ContextManager::Audio() {
		return audioMgr;
	}

	// Gets the audio manager.
	TextManager* ContextManager::Text() {
		return txtMgr;
	}

	void ContextManager::StartSync() {
		timeElapsed = SDL_GetTicks();
	}

	void ContextManager::EndSync() {
		while (1) {
			double currentTime = SDL_GetTicks();
			if(currentTime - timeElapsed > 1000 / logicRate)
				return;
			SDL_Delay((1000 / logicRate) / 8);
		}
	}

	void ContextManager::SetTitle(char* title) {
		SDL_SetWindowTitle(window, title);
	}
