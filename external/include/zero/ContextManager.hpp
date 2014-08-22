#ifndef CONTEXTMANAGER_HPP
#define CONTEXTMANAGER_HPP

typedef void (*GameCallback)(bool);
typedef void (*MouseCallback)(int, int, bool, bool, bool, bool); // X, Y, up/down, isLeft, isMiddle, isRight

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
			ContextManager(int scr_width, int scr_height, bool fulls, bool accel);

			// Initialization.
			void InitWindow(int width, int height, bool fulls, bool accel);
			void InitWindowLogical(int width, int height, int width_log, int height_log, bool fulls, bool accel);

			// Set window title
			void SetTitle(char* title);

			// Display related.
			void Clear();
			void Flush();

			void ClearOverlay();

			SDL_Surface* Surface();
			SDL_Renderer* Renderer();
			bool Accelerated();
			void Blit(void* inp, SDL_Rect* src, SDL_Rect* dst);
			void OverlayBlit(void* inp, SDL_Rect* src, SDL_Rect* dst);

			// Support funcs.
			int GetWidth();
			int GetHeight();

			// Get Input State and Modify input state.
			bool Input();

			// Abstract remappable key functions.
			int RegisterInput(SDL_Keycode key, GameCallback func);
			void RegisterMouse(MouseCallback func);
			bool GetInput(int index);
			void RemapKey(int index, SDL_Keycode to);
			void InputMode(InMode mode);

			// Gets the AudioManager & TextManager
			AudioManager* Audio();
			TextManager* Text();

			// Exit methods.
			void SetQuit();
			bool GetQuit();

			// Logic sync limiter.
			void StartSync();
			void EndSync();

			~ContextManager();
		private:
			// Lower init.
			// DELETEME - Software rendering doesn't work now.
			void _InitWindowSW(int width, int height, bool fulls);
			void _InitWindowAC(int width, int height, bool fulls);
			// DELETEME - Software rendering doesn't work now.
			void _InitWindowLogicalSW(int width_win, int height_win, int width_log, int height_log, bool fulls);
			void _InitWindowLogicalAC(int width_win, int height_win, int width_log, int height_log, bool fulls);

			InMode inputmode = Medium;
			SDL_Window* window = NULL;
			SDL_Surface* surface = NULL, *surface_o = NULL;
			SDL_Texture* texture = NULL, *texture_o = NULL;
			SDL_Renderer* renderer = NULL;
			// use logical size
			bool logicalRender = false;
			bool accelerate = false;

			bool StartQuit = false;

			AudioManager* audioMgr = NULL;
			TextManager* txtMgr = NULL;

			SDL_Keycode* inputMappings = NULL; // This can actually be used instead of InputList now. Weird.
			bool* inputStates = NULL;
			GameCallback* inputCallbacks = NULL;
			MouseCallback mouseCallback = NULL;

			int inputRegistered = 0;

			// Logic cap.
			int logicRate = 60;
			int timeElapsed = 0;
			int syncElapsed = 0;

			// Width and height
			int WIN_width = 0;
			int WIN_height = 0;
			int LOG_width = 0;
			int LOG_height = 0;
	};

#endif
