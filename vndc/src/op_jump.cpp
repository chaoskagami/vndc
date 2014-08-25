#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements jump vnds function.
 * jump file
 */

void op_jump(char* file, int* lineTo, bool isSave) {
	if (GetData()->if_fail != 0)
		return;

	memset(GetData()->current_scr, 0, 400);
	if(!isSave)
		snprintf(GetData()->current_scr, 400, "script/%s", file);
	else
		snprintf(GetData()->current_scr, 400, "%s", file);

	if(GetData()->debug_mode) {
		memset(&GetData()->window_name[16], 0, sizeof(char)*400);
		snprintf(&GetData()->window_name[16], 400-16-3, " (%s)", file);
		GetData()->ctx->SetTitle(GetData()->window_name);
	}

	if(GetData()->accessScriptHandle != NULL)
		fclose(GetData()->accessScriptHandle);

	GetData()->accessScriptHandle = fopen(GetData()->current_scr, "r");
	if(GetData()->accessScriptHandle == NULL) {
		printf("[error] Failed to open script file %s.\n", GetData()->current_scr);
		exit(-1);
	}


	if(lineTo != NULL) {
		GetData()->currentLine = lineTo[0];
		char *line = (char*)calloc(sizeof(char), 400);

		while(--lineTo[0] > 0) {
			fgets(line, 400, GetData()->accessScriptHandle);
		}
	}
	else {
		GetData()->currentLine = 0;
	}

	// Read the first line into next_line.
	GetData()->next_line = (char*)calloc(sizeof(char), 400);
	// Load the next line.
	fgets(GetData()->next_line, 400, GetData()->accessScriptHandle);

}
