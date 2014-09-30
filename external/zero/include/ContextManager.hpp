#ifndef CONTEXTMANAGER_HPP
#define CONTEXTMANAGER_HPP

typedef void (*GameCallback)(bool);
typedef void (*MouseCallback)(int, int, bool, bool, bool, bool); // X, Y, up/down, isLeft, isMiddle, isRight

class VertexController;

typedef enum {
	Software = 0,
	Accel2d  = 1,
	OpenGL   = 2
} DispMode_t;

class TextManager; // Forward decl

	// Input Mode enum. This is the max events allowed in one go.
	typedef enum InMode_e {
		Slow = 20,
		Medium = 10,
		Burst = 5
	} InMode;

	class ContextManager {
		public:
			ContextManager();
			ContextManager(int scr_width, int scr_height, bool fulls, DispMode_t disp);

			// Initialization.
			void InitWindow(int width, int height, bool fulls, DispMode_t disp);
			void InitWindowLogical(int width, int height, int width_log, int height_log, bool fulls, DispMode_t disp);

			// Set window title
			void SetTitle(char* title);

			// Render related.
			void Clear();
			void Flush();
			void ClearOverlay();
			void Blit(void* inp, SDL_Rect* src, SDL_Rect* dst, SDL_Rect* glrect);
			void OverlayBlit(void* inp, SDL_Rect* src, SDL_Rect* dst, SDL_Rect* glrect);

			// Accessors.
			SDL_Surface* Surface();
			SDL_Renderer* Renderer();
			bool Accelerated();
			bool GLMode();
			int GetWidth();
			int GetHeight();

			// Run input callbacks and update states.
			bool Input();

			// Abstract remappable key functions.
			int RegisterInput(SDL_Keycode key, GameCallback func);
			void RegisterMouse(MouseCallback func);
			bool GetInput(int index);
			void RemapKey(int index, SDL_Keycode to);
			void InputMode(InMode mode);

			// Gets the AudioManager & TextManager for usage.
			AudioManager* Audio();
			TextManager* Text();
			VertexController* Vertex();

			// Exit methods.
			void SetQuit();
			bool GetQuit();

			// Logic sync limiter @ 60fps
			void StartSync();
			void EndSync();

			// Wrapper functions for tex load.
			void* LoadImage(char* fname);
			void* LoadImageMemory(void* mem, size_t len, char* type);
			void* AccelImage(void* in);
			void* GLTexImage(void* in);

			~ContextManager();
		private:
			// Lower init.
			void _InitWindowSW(int width, int height, bool fulls);
			void _InitWindowAC(int width, int height, bool fulls);
			void _InitWindowGL(int width, int height, bool fulls);

			// Lower init, logical.
			void _InitWindowLogicalSW(int width_win, int height_win, int width_log, int height_log, bool fulls);
			void _InitWindowLogicalAC(int width_win, int height_win, int width_log, int height_log, bool fulls);
			void _InitWindowLogicalGL(int width_win, int height_win, int width_log, int height_log, bool fulls);

			// Used to load default values in abscense of GNU exts
			void DefaultVariables();

			InMode inputmode;
			SDL_Window* window;
			SDL_Surface* surface, *surface_o;
			SDL_Texture* texture, *texture_o;
			SDL_Renderer* renderer;
			SDL_GLContext glctx;

			// use logical size
			bool logicalRender;
			bool accelerate;
			bool opengl_mode;

			GLuint fg, bg, fg_tex, bg_tex;

			bool StartQuit;

			AudioManager* audioMgr;
			TextManager* txtMgr;
			VertexController* vertCnt;

			SDL_Keycode* inputMappings; // This can actually be used instead of InputList now. Weird.
			bool* inputStates;
			GameCallback* inputCallbacks;
			MouseCallback mouseCallback;

			int inputRegistered;

			// Logic cap.
			int logicRate;
			int timeElapsed;
			int syncElapsed;

			// Width and height
			int WIN_width;
			int WIN_height;
			int LOG_width;
			int LOG_height;
	};

#endif
