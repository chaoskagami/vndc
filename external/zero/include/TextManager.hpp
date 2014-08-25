
class TextManager {
	public:
		// Ctors & Dtors
		TextManager(ContextManager* ct);
		~TextManager();

		// Base font functions.
		int LoadFont(char* fname, int size);
		void Render(char* text);
		void Render(char* text, int x, int y);

		int TestLen(char* text);
		void Outline(int pixels);

		// Property functions.
		// void SetStyle(int style);
		void SetFontUsed(int font);
		void SetColor(int r, int g, int b, int a);

		// Complex functions.
		// Like printf but with formatting codes. Also not varargs.
		// Should support this subset of standards:
		// %d %u %x %c %s %f %%
		// It also should have the following:
		// %t - Font Index. Switches to font index.
		// %p - Style. Sets style to S.
		// %m - Expects an SDL_Color. Sets the color.
		// void fnoutf(char* text, void** data);

		// Uses control codes, unline fnoutf. Mainly intended for scripts.
		// These ops should be implemented:
		// ^b (bold) ^i (italic) ^r (normal / reset) ^fI (font -> i)
		// ^cNNN (color -> #NNN) ^n (next line) ^c (clear)
		// void scoutf(char* text);

	private:
		TTF_Font** fonts = NULL;
		int current_font = 0;
		int fonts_loaded = 0;
		SDL_Color color;
		ContextManager* ctx = NULL;
		bool outline = false;
		int outline_px = 0;
};
