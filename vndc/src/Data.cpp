#include "Zero.hpp"

#include "Funcs.hpp"

DataContainer* data = NULL;

const char* loadup = "script/main.scr";

void CreateDataContainer() {
	data = new DataContainer();
	GetData()->main_scr = (char**)calloc(sizeof(char*), 1);
	GetData()->main_scr[0] = (char*)calloc(sizeof(char), 400);
	strncpy(GetData()->main_scr[0], loadup, 400);
}

DataContainer* GetData() {
	return &data[0];
}

void DumpSave(char* fname) {
	// Dump variables.
	// Dump script file.

	// A save actually ends up as a mini-script.
	// So, to load a save you load this as the
	// main script
	// For example:
	// setvar data1 = 6
	// setvar data2 = 9
	// music mus
	// bgload bg
	// jump script.scr 60
	// The call command runs a scr, and returns here.

	FILE* save_to = fopen(fname, "w");

	std::map<std::string, int> data_vals = GetData()->s_flags[0];

	if(!data_vals.empty()) {
		std::map<std::string, int>::iterator item = data_vals.begin();
		while(item != data_vals.end()) {
			fprintf(save_to, "setvar %s = %d\n", item->first.c_str(), item->second);

			++item;
		}
	}

	fprintf(save_to, "music %s\n", &(GetData()->current_music[6]));
	fprintf(save_to, "bgload %s\n", &(GetData()->current_bg[11]));
	fprintf(save_to, "jump %s %d\n", &(GetData()->current_scr[7]), GetData()->currentLine - 1); // The text never completely displayed
																								// So restore back one to replay

	fclose(save_to);
}
