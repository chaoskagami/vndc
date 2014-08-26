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

			void DefaultVariables();

			InMode inputmode;
			SDL_Window* window;
			SDL_Surface* surface, *surface_o;
			SDL_Texture* texture, *texture_o;
			SDL_Renderer* renderer;
			// use logical size
			bool logicalRender;
			bool accelerate;

			bool StartQuit;

			AudioManager* audioMgr;
			TextManager* txtMgr;

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
