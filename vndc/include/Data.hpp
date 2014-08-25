#ifndef DATAGUARD_HPP
#define DATAGUARD_HPP

#include <map>
#include <string>

class DataContainer {
	public:
		ContextManager* ctx;
		int if_fail = 0;
		bool wait_input = false;
		char current_scr[400], current_music[400], current_bg[400];
		int screen_w = 800;
		int screen_h = 600;
		int physical_w = 800;
		int physical_h = 600;
		int render_x1 = screen_w / 15;
		int render_y1 = screen_h / 15;
		int render_x2 = screen_w / 15 * 14;
		int render_y2 = screen_h / 15 * 13;
		int text_x = render_x1;
		int text_y = render_y1;
		std::map<std::string, int> *g_flags = NULL;
		std::map<std::string, int> *s_flags = NULL;
		bool jumped = false;
		int* choice_coords = NULL;
		int choices = 0;
		FILE* accessScriptHandle = NULL;
		char** main_scr; // Default value.
		bool vndc_enabled = true;
		bool is_spoken_line = false;
		bool debug_mode = false;
		bool debug_to_shell = false;
		bool verbose = false;
		int currentLine = 0;
		bool skip_key_on = false;
		char* window_name;
		char* next_line = NULL; // Used for voice-detect.
					// It's impossible to parse without lookahead.
};

DataContainer* GetData();
void CreateDataContainer();
void DumpSave(char* fname);

#endif
