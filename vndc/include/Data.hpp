#ifndef DATAGUARD_HPP
#define DATAGUARD_HPP

#include <map>
#include <string>

class DataContainer {
	public:
		DataContainer();

		ContextManager* ctx;
		int if_fail;
		bool wait_input;
		char current_scr[400], current_music[400], current_bg[400];
		int screen_w;
		int screen_h;
		int physical_w;
		int physical_h;
		int render_x1;
		int render_y1;
		int render_x2;
		int render_y2;
		int text_x;
		int text_y;
		std::map<std::string, int> *s_flags;
		bool jumped;
		int* choice_coords;
		int choices;
		FILE* accessScriptHandle;
		char** main_scr; // Default value.
		bool vndc_enabled;
		bool quote_incomplete;
		bool debug_mode;
		bool debug_to_shell;
		bool verbose;
		int currentLine;
		bool skip_key_on;
		bool sw_rendering;
		bool eof;
		char* window_name;
		char* next_line; // Used for voice-detect.
					// It's impossible to parse without lookahead.
};

DataContainer* GetData();
void CreateDataContainer();
void DumpSave(char* fname);

#endif
