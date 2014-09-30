#include "Zero.hpp"


	void ContextManager::DefaultVariables() {
		inputmode = Medium;
		window = NULL;
		surface = NULL; surface_o = NULL;
		texture = NULL; texture_o = NULL;
		renderer = NULL;
		// use logical size
		logicalRender = false;
		accelerate = false;
		opengl_mode = false;

		StartQuit = false;

		audioMgr = NULL;
		txtMgr = NULL;

		inputMappings = NULL; // This can actually be used instead of InputList now. Weird.
		inputStates = NULL;
		inputCallbacks = NULL;
		mouseCallback = NULL;

		inputRegistered = 0;

		// Logic cap.
		logicRate = 60;
		timeElapsed = 0;
		syncElapsed = 0;

		// Width and height
		WIN_width = 0;
		WIN_height = 0;
		LOG_width = 0;
		LOG_height = 0;
	}

	// Initializes SDL and saves stuff into the context.

	ContextManager::ContextManager(int scr_width, int scr_height, bool fulls, DispMode_t disp) {
		DefaultVariables();

		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
			#ifdef DEBUG_OVERKILL
			printf("[ContextManager~Ctor] SDL Init error w/ VIDEO|AUDIO. Msg rep: %s\n", SDL_GetError());
			#else
			printf("Error initializing SDL. Info:%s\n", SDL_GetError());
			#endif
		}
		else {
			this->InitWindow(scr_width, scr_height, fulls, disp);
		}

		audioMgr = new AudioManager();
	}

	// Delays window creation; Use InitWindow before doing anything.

	ContextManager::ContextManager() {
		DefaultVariables();

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

	void ContextManager::InitWindow(int width, int height, bool fulls, DispMode_t disp) {
		this->WIN_width = width;
		this->WIN_height = height;
		this->LOG_width = width;
		this->LOG_height = height;

		#ifdef DEBUG_OVERKILL
		printf("[ContextManager::InitWindow] Params: w:%d h:%d f:%d a:%d\n", width, height, fulls, disp);
		#endif

		if (disp == Accel2d) {
			_InitWindowAC(width, height, fulls);
			this->texture_o = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
			SDL_SetTextureBlendMode(this->texture_o, SDL_BLENDMODE_BLEND);
		}
		else if (disp == Software) {
			_InitWindowSW(width, height, fulls);
			this->surface_o = SDL_CreateRGBSurface(0, width, height, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
			SDL_SetSurfaceBlendMode(this->surface_o, SDL_BLENDMODE_BLEND);
		}
		else if (disp == OpenGL) {
			_InitWindowGL(width, height, fulls);
		}

		txtMgr = new TextManager(this);
	}

	void ContextManager::_InitWindowSW(int width, int height, bool fulls) {
		// Add title setting function.
		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);
			if(!SDL_CreateRenderer( this->window, -1, 0 )) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}

			this->logicalRender = false;
		}
	}

	void ContextManager::_InitWindowAC(int width, int height, bool fulls) {
		this->accelerate = true;

		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN);
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

	// OpenGL window. Note - a LOT functions different in this mode.
	void ContextManager::_InitWindowGL(int width, int height, bool fulls) {
		opengl_mode = true;

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		// Request OpenGL 3.0 context. Yes, not 3.1, 3.2, 4.0. THREE POINT O'.
		// Why? I use the fixed function pipeline which is gonzo in >3.1
		// and I'm not about to learn a new api that consists of VBOs, FBOs,
		// and shader code (which btw, is retarded.)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);

		glctx = SDL_GL_CreateContext(window);

		glewExperimental = GL_TRUE;
		glewInit();

		if(glGenFramebuffers == NULL || glBindFramebuffer == NULL || glFramebufferTexture == NULL || glDrawBuffers == NULL) {
			fprintf(stderr, "[ERR] Glew didn't load some extensions needed.\n");
			fprintf(stderr, "[ERR] I assume your card doesn't support them.\n");
			exit(-11);
		}


		glEnable( GL_TEXTURE_2D );

		glViewport( 0, 0, width, height );

		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		vertCnt = new VertexController(this);

		// Create Renderable Textures.
		// Unfortunately, GL is an asshole and I have no clue.

		glGenFramebuffers(1, &bg);
		glBindFramebuffer(GL_FRAMEBUFFER, bg);

		glGenTextures(1, &bg_tex);
		glBindTexture(GL_TEXTURE_2D, bg_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, bg_tex, 0);

		GLenum colbuf_bg[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, colbuf_bg);

		glGenFramebuffers(1, &fg);
		glBindFramebuffer(GL_FRAMEBUFFER, fg);

		glGenTextures(1, &fg_tex);
		glBindTexture(GL_TEXTURE_2D, fg_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fg_tex, 0);

		GLenum colbuf_fg[1] = {GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1, colbuf_fg);
	}

	// Init window with a logical size and a real one. Determines which backend to use.
	void ContextManager::InitWindowLogical(int width_win, int height_win, int width_log, int height_log, bool fulls, DispMode_t disp) {
		this->LOG_width = width_log;
		this->LOG_height = height_log;
		this->WIN_width = width_win;
		this->WIN_height = height_win;

		#ifdef DEBUG_OVERKILL
		printf("[ContextManager::InitWindowLogical] phyw:%d phyh:%d logw:%d logh:%d f:%d a:%d\n", width_win, height_win, width_log, height_log, fulls, disp);
		#endif

		if (disp == Accel2d) {
			_InitWindowLogicalAC(width_win, height_win, width_log, height_log, fulls);
			this->texture_o = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_win, height_win);
			SDL_SetTextureBlendMode(this->texture_o, SDL_BLENDMODE_BLEND);
		}
		else if (disp == Software) {
			_InitWindowLogicalSW(width_win, height_win, width_log, height_log, fulls);
			this->surface_o = SDL_CreateRGBSurface(0, width_win, height_win, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
			SDL_SetSurfaceBlendMode(this->surface_o, SDL_BLENDMODE_BLEND);
		}
		else if (disp == OpenGL) {
			_InitWindowLogicalGL(width_win, height_win, width_log, height_log, fulls);
		}

		txtMgr = new TextManager(this);
	}

	// Init window with a logical size and a real one.
	void ContextManager::_InitWindowLogicalSW(int width_win, int height_win, int width_log, int height_log, bool fulls) {
		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_SHOWN);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_FULLSCREEN);
		if(this->window == NULL) {
			fprintf(stderr, "[E] Error creating window. Info:\n");
			fprintf(stderr, "[E] %s\n", SDL_GetError());
			fprintf(stderr, "[E] This is fatal. Dying.");
			exit(-1);
		}
		else {
			this->renderer = SDL_CreateRenderer( this->window, -1, 0 );
			if(this->renderer == NULL) {
				fprintf(stderr, "[E] Error creating renderer. Info:\n");
				fprintf(stderr, "[E] %s\n", SDL_GetError());
				fprintf(stderr, "[E] This is fatal. Dying.");
				exit(-1);
			}
			else {
				this->logicalRender = true;

				this->surface = SDL_CreateRGBSurface(0, width_log, height_log, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
			}
		}
	}

		// Init window with a logical size and a real one.
	void ContextManager::_InitWindowLogicalAC(int width_win, int height_win, int width_log, int height_log, bool fulls) {
		this->accelerate = true;
		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_SHOWN);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_FULLSCREEN);
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

	// OpenGL window. Note - a LOT functions different in this mode.
	void ContextManager::_InitWindowLogicalGL(int width_win, int height_win, int width_log, int height_log, bool fulls) {

		opengl_mode = true;

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		// Request OpenGL 3.0 context. Yes, not 3.1, 3.2, 4.0. THREE POINT O'.
		// Why? I use the fixed function pipeline which is gonzo in >3.1
		// and I'm not about to learn a new api that consists of VBOs, FBOs,
		// and shader code (which btw, is retarded.)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if(!fulls)
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
		else
			this->window = SDL_CreateWindow("Zero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_win, height_win, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);

		glctx = SDL_GL_CreateContext(window);

		glewExperimental = GL_TRUE;
		glewInit();

		if(glGenFramebuffers == NULL || glBindFramebuffer == NULL || glFramebufferTexture == NULL || glDrawBuffers == NULL) {
			fprintf(stderr, "[ERR] Glew didn't load some extensions needed.\n");
			fprintf(stderr, "[ERR] I assume your card doesn't support them.\n");
			exit(-11);
		}

		glEnable( GL_TEXTURE_2D );

		glViewport( 0, 0, width_win, height_win );

		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho(0.0f, width_win, height_win, 0.0f, -1.0f, 1.0f);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		vertCnt = new VertexController(this);

		fprintf(stderr, "[info] Generating FBOs...(bg) ");

		glGenFramebuffers(1, &bg);
		glBindFramebuffer(GL_FRAMEBUFFER, bg);

		glGenTextures(1, &bg_tex);
		glBindTexture(GL_TEXTURE_2D, bg_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_log, height_log, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, bg_tex, 0);

		GLenum colbuf_bg[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, colbuf_bg);

		glGenFramebuffers(1, &fg);
		glBindFramebuffer(GL_FRAMEBUFFER, fg);

		glGenTextures(1, &fg_tex);
		glBindTexture(GL_TEXTURE_2D, fg_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_win, height_win, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fg_tex, 0);

		GLenum colbuf_fg[1] = {GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1, colbuf_fg);

		fprintf(stderr, "[info] OpenGL initialized.\n");
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
		else if (opengl_mode) {
			glBindFramebuffer(GL_FRAMEBUFFER, this->bg);
			glViewport(0,0,LOG_height,LOG_width);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glBindFramebuffer(GL_FRAMEBUFFER, this->fg);
			glViewport(0,0,WIN_height,WIN_width);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0,0,WIN_height,WIN_width);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else {
			if (logicalRender)
				SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0x0, 0x0, 0x0));

			SDL_FillRect(this->surface_o, NULL, SDL_MapRGBA(this->surface_o->format, 0x0, 0x0, 0x0, 0x0));

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
		else if (opengl_mode) {
			glBindFramebuffer(GL_FRAMEBUFFER, this->fg);

			glViewport(0,0,WIN_height,WIN_width);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			SDL_FillRect(this->surface_o, NULL, SDL_MapRGBA(this->surface_o->format, 0, 0, 0, 0));
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
		else if (opengl_mode) {
			// Render prep.

			GLfloat x_tex, y_tex, x2_tex, y2_tex;
			x_tex  = 0.0f;
			y_tex  = 0.0f;
			x2_tex = 1.0f;
			y2_tex = 1.0f;


			GLfloat x_box, y_box, x2_box, y2_box;
			x_box = 0;
			y_box = WIN_height;
			x2_box = WIN_width;
			y2_box = 0;

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f ); //Don't use special coloring

			glBindTexture( GL_TEXTURE_2D, bg_tex );

			glBegin( GL_QUADS );
				//Bottom-left vertex (corner)
				glTexCoord2f( x_tex, y_tex );
				glVertex3f( x_box,  y2_box, 0.0f );

				//Bottom-right vertex (corner)
				glTexCoord2f( x2_tex, y_tex );
				glVertex3f( x2_box, y2_box, 0.0f );

				//Top-right vertex (corner)
				glTexCoord2f( x2_tex, y2_tex );
				glVertex3f( x2_box, y_box,  0.0f );

				//Top-left vertex (corner)
				glTexCoord2f( x_tex, y2_tex );
				glVertex3f( x_box,  y_box,  0.0f );
			glEnd();

			glBindTexture( GL_TEXTURE_2D, fg_tex );

			glBegin( GL_QUADS );
				//Bottom-left vertex (corner)
				glTexCoord2f( x_tex, y_tex );
				glVertex3f( x_box,  y2_box, 0.0f );

				//Bottom-right vertex (corner)
				glTexCoord2f( x2_tex, y_tex );
				glVertex3f( x2_box, y2_box, 0.0f );

				//Top-right vertex (corner)
				glTexCoord2f( x2_tex, y2_tex );
				glVertex3f( x2_box, y_box,  0.0f );

				//Top-left vertex (corner)
				glTexCoord2f( x_tex, y2_tex );
				glVertex3f( x_box,  y_box,  0.0f );
			glEnd();

			glBindTexture( GL_TEXTURE_2D, 0 );

			SDL_GL_SwapWindow(window);
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

	// return the mode.
	bool ContextManager::Accelerated() {
		return accelerate;
	}

	// return the mode.
	bool ContextManager::GLMode() {
		return opengl_mode;
	}

	// Blit data via cast. inp should be a SDL_Surface or SDL_Texture.
	void ContextManager::Blit(void* inp, SDL_Rect* src, SDL_Rect* dst, SDL_Rect* glrect) {
		if(accelerate) {
			if (logicalRender) SDL_SetRenderTarget(renderer, texture);

			SDL_RenderCopy(renderer, (SDL_Texture*)inp, src, dst);

			if (logicalRender) SDL_SetRenderTarget(renderer, NULL);
		}
		else if(opengl_mode) {
			GLuint in = ((GLuint*)inp)[0];

			glBindFramebuffer(GL_FRAMEBUFFER, this->bg);
			glViewport(0,0,LOG_height,LOG_width);

			// printf("[GL] (blit) in: %d\n", in);

			GLfloat x_tex, y_tex, x2_tex, y2_tex;
			if(glrect == NULL) {
				x_tex  = 0.0f;
				y_tex  = 0.0f;
				x2_tex = 1.0f;
				y2_tex = 1.0f;
			}
			else {
				GLfloat tex_width, tex_height;

				tex_width = 1.0f / ((float)glrect->w);
				tex_height = 1.0f / ((float)glrect->h);

				x_tex  = tex_width   * ((float)(src->x));
				y_tex = tex_height   * ((float)(src->y));
				x2_tex = tex_width   * ((float)(src->x + src->w));
				y2_tex  = tex_height * ((float)(src->y + src->h));

				// printf("Detail Calculation] tex_w:%f tex_h:%f x_t:%f y_t:%f x2_t:%f y2_t:%f\n", tex_width, tex_height, x_tex, y_tex, x2_tex, y2_tex);
			}

			GLfloat x_box, y_box, x2_box, y2_box;
			x_box = dst->x;
			y_box = dst->y + dst->h;
			x2_box = dst->x + dst->w;
			y2_box = dst->y;

			//printf("[GL] Box: x.%f y.%f x2.%f y2.%f \n", x_box, y_box, x2_box, y2_box);

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f ); //Don't use special coloring

			// Bind the texture to which subsequent calls refer to
			glBindTexture( GL_TEXTURE_2D, in );

			glBegin( GL_QUADS );
				//Bottom-left vertex (corner)
				glTexCoord2f( x_tex, y_tex );
				glVertex3f( x_box,  y2_box, 0.0f );

				//Bottom-right vertex (corner)
				glTexCoord2f( x2_tex, y_tex );
				glVertex3f( x2_box, y2_box, 0.0f );

				//Top-right vertex (corner)
				glTexCoord2f( x2_tex, y2_tex );
				glVertex3f( x2_box, y_box,  0.0f );

				//Top-left vertex (corner)
				glTexCoord2f( x_tex, y2_tex );
				glVertex3f( x_box,  y_box,  0.0f );
			glEnd();

			glBindTexture( GL_TEXTURE_2D, 0 );

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0,0,WIN_height,WIN_width);

		}
		else {
			SDL_BlitSurface((SDL_Surface*)inp, src, surface, dst);
		}
	}

	// Blit data to overlay via cast. inp should be a SDL_Surface or SDL_Texture.
	void ContextManager::OverlayBlit(void* inp, SDL_Rect* src, SDL_Rect* dst, SDL_Rect* glrect) {
		// Recalculate src and dst based on upscale.
		double OUTW_Ratio, OUTH_Ratio;
		OUTW_Ratio = (double)WIN_width  / (double)LOG_width;
		OUTH_Ratio = (double)WIN_height / (double)LOG_height;

		dst->x = (int)(((double)dst->x) * OUTW_Ratio);
		dst->y = (int)(((double)dst->y) * OUTH_Ratio);
		if(OUTW_Ratio < 1) dst->w = (int)(((double)dst->w) * OUTW_Ratio);
		if(OUTH_Ratio < 1) dst->h = (int)(((double)dst->h) * OUTH_Ratio);

		if(accelerate) {
			if (logicalRender) SDL_SetRenderTarget(renderer, texture_o);

			SDL_RenderCopy(renderer, (SDL_Texture*)inp, src, dst);

			if (logicalRender) SDL_SetRenderTarget(renderer, NULL);
		}
		else if(opengl_mode) {
			GLuint in = ((GLuint*)inp)[0];

			glBindFramebuffer(GL_FRAMEBUFFER, this->fg);
			glViewport(0,0,WIN_height,WIN_width);

			GLfloat x, y, x2, y2;
			x = 1.0f / glrect->x;
			y = 1.0f / glrect->y;
			x2 = 1.0f / (glrect->w + glrect->x);
			y2 = 1.0f / (glrect->h + glrect->y);

			// Bind the texture to which subsequent calls refer to
			glBindTexture( GL_TEXTURE_2D, in );

			glBegin( GL_QUADS );
				//Bottom-left vertex (corner)
				glTexCoord2f( x, y );
				glVertex3i( dst->x,          dst->y + dst->h, 0.0f );

				//Bottom-right vertex (corner)
				glTexCoord2f( x2, y );
				glVertex3i( dst->x + dst->w, dst->y + dst->h, 0.0f );

				//Top-right vertex (corner)
				glTexCoord2f( x2, y2 );
				glVertex3i( dst->x + dst->w, dst->y, 0.0f );

				//Top-left vertex (corner)
				glTexCoord2f( x, y2 );
				glVertex3i( dst->x,          dst->y, 0.0f );
			glEnd();

			glBindTexture( GL_TEXTURE_2D, 0 );

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0,0,WIN_height,WIN_width);

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

	void* ContextManager::LoadImage(char* fname) {
		return IMG_Load(fname);
	}

	void* ContextManager::LoadImageMemory(void* mem, size_t len, char* type) {
		SDL_RWops* rwo = SDL_RWFromMem(mem, len);
		SDL_Surface* bitmap_tmp = IMG_LoadTyped_RW(rwo, 0, type);

		return bitmap_tmp;
	}

	void* ContextManager::AccelImage(void* in) {
		if(accelerate) {
			SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, (SDL_Surface*)in);
			SDL_FreeSurface((SDL_Surface*)in);

			return tex;
		}
		return in;
	}

	void* ContextManager::GLTexImage(void* in) {

		if(!opengl_mode) return in;

		GLuint *tex = (GLuint*)calloc(sizeof(GLuint), 1);
		GLenum texture_format;
		GLint  nOfColors;
		SDL_Surface* sfc = (SDL_Surface*)in;

		if ( (sfc->w & (sfc->w - 1)) != 0 ) {
			printf("[WARN] Image width not ^2.\n");
		}

		// Also check if the height is a power of 2
		if ( (sfc->h & (sfc->h - 1)) != 0 ) {
			printf("[WARN] Image height not ^2.\n");
		}

		// get the number of channels in the SDL surface
		nOfColors = sfc->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (sfc->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		}
		else if (nOfColors == 3)     // no alpha channel
		{
			if (sfc->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		}
		else {
			printf("[WARN] Image not truecolor (32bpp). Expect explosions.\n");
			// this error should not go unhandled
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, tex );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, tex[0] );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, sfc->w, sfc->h, 0, texture_format, GL_UNSIGNED_BYTE, sfc->pixels );

		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// printf("[GL] tex: %d\n", tex[0]);

		return tex;
	}
