#ifndef UDISPLAYABLE_HPP
#define UDISPLAYABLE_HPP

typedef enum {
	Normal=1,
	Hitbox=2,
	Docked=3,
	HitboxDocked=4,
	Anim=5, // nyi
	AnimHitbox=6, // nyi
	AnimDocked=7, // nyi
	AnimHitboxDocked=8 // nyi
} UDisplayableMode;

	class UDisplayable {
		public:
			UDisplayable(ContextManager* ctx, char* fname); // Sets up in normal mode.
			UDisplayable(ContextManager* ctx, UDisplayableMode mode, char* fname); // Sets up in specified mode; params not set
			UDisplayable(ContextManager* ctx, UDisplayableMode mode, void* memory, int mSize); // Sets up from memory block in mode
			UDisplayable(ContextManager* cx, UDisplayableMode mode, SDL_Surface* bitmap_tmp);  // Loads from SDL_surface

			// All modes can use the following.
			void SetXY(double x, double y);
			void ModXY(double xd, double yd);

			double GetX();
			double GetY();

			int GetXI();
			int GetYI();

			int GetW();
			int GetH();

			void Blit();

			int* GetHitbox();

			~UDisplayable();

			// Only specific modes can use these. Otherwise, they nop.
			void SetHitbox(int x, int y, int w, int h);
			void SetDock(int x, int y, int w, int h);
			void SetOverlay(bool state);

			// Only animated Displayables can use these.
			void SetFrameWidth(int frameW);
			void NextFrame();
			void NextFrame(int f);
			void ResetFrame();
			void SetFrame(int f);


		protected:
			void DefaultVars();

			double x, y;
			void* bitmap; // Will contain either a SDL_Surface or SDL_Texture
			SDL_Rect loc, clip;
			ContextManager* ctx;
			bool over;

			int bmp_w, bmp_h;

			// Used in docked mode
			int* frame;
			// Used in hitbox mode
			int* hitbox;

			// The current mode.
			UDisplayableMode dispMode;

			// Used by animated mode.
			int frameIndex;
			int frameWidth;

			bool Error;
	};
#endif
