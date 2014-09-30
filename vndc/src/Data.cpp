#include "Zero.hpp"

#include "Funcs.hpp"

DataContainer* data = NULL;

const char* loadup = "script/main.scr";

DataContainer::DataContainer() {
	if_fail = 0;
	wait_input = false;
	screen_w = 800;
	screen_h = 600;
	physical_w = 800;
	physical_h = 600;
	render_x1 = screen_w / 15;
	render_y1 = screen_h / 15;
	render_x2 = screen_w / 15 * 14;
	render_y2 = screen_h / 15 * 13;
	text_x = render_x1;
	text_y = render_y1;
	text_gap = 35;
	text_size = 24;
	jumped = false;
	choice_coords = NULL;
	choices = 0;
	accessScriptHandle = NULL;
	vndc_enabled = true;
	quote_incomplete = false;
	debug_mode = false;
	debug_to_shell = false;
	verbose = false;
	currentLine = 0;
	skip_key_on = false;
	#ifdef USE_ANDROID
	rendering_mode = 0;
	fullscreen = true;
	#else
	rendering_mode = 1;
	fullscreen = false;
	#endif
	eof = false;
	next_line = NULL; // Used for voice-detect.
				// It's impossible to parse without lookahead.
}

void Data_PreInit() {
	data = new DataContainer();

	/* Storage of values for saves */
	GetData()->main_scr = (char**)calloc(sizeof(char*), 1);
	GetData()->main_scr[0] = (char*)calloc(sizeof(char), 400);
	strncpy(GetData()->main_scr[0], loadup, 400);
}

void Data_PostInit() {
	/* Generate the surface for use with cleartext. */
	int width_dr = (GetData()->render_x2 - GetData()->render_x1 + 20);
	int height_dr = (GetData()->render_y2 - GetData()->render_y1 + 20 + 30);

	SDL_Surface* pass_sfc = SDL_CreateRGBSurface(0, width_dr, height_dr, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
	SDL_FillRect(pass_sfc, NULL, SDL_MapRGBA(pass_sfc->format, 0, 0, 0, 100));

	GetData()->text_box_base = new UDisplayable(GetData()->ctx, Normal, pass_sfc);
	GetData()->text_box_base->SetOverlay(true);
	GetData()->text_box_base->SetXY(GetData()->render_x1 - 10, GetData()->render_y1 - 10);
}

DataContainer* GetData() {
	return &data[0];
}

void DumpSave(char* fname) {
	// Dump variables.
	// Dump script file.

	// A save actually ends up as a mini-script.
	// So, to load a save you load something like this as the
	// main script:

	// setvar data1 = 6
	// setvar data2 = 9
	// music mus
	// bgload bg
	// jump script.scr 60

	FILE* save_to = fopen(fname, "w");

	std::map<std::string, int> data_vals = GetData()->s_flags[0];

	if(!data_vals.empty()) {
		std::map<std::string, int>::iterator item = data_vals.begin();
		while(item != data_vals.end()) {
			if(strcmp(item->first.c_str(), "selected"))
				fprintf(save_to, "setvar %s = %d\n", item->first.c_str(), item->second);

			++item;
		}
	}

	fprintf(save_to, "music %s\n", &(GetData()->current_music[6]));
	fprintf(save_to, "bgload %s\n", &(GetData()->current_bg[11]));
	fprintf(save_to, "jump %s %d\n", &(GetData()->current_scr[7]), GetData()->currentLine - 1); // The text ack'd with leftclick
												    // So restore back one to replay
	fclose(save_to);
}
