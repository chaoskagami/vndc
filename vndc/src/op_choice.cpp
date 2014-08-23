#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements choice vnds function.
 * choice choicest1|choicest2|... [>var]
 * [>var] is an extension of this dialect
 * which stores the result into a specific variable
 * The default will still be 'selected' if not specified
 */

void op_choice(char* line) {
	if (GetData()->if_fail != 0)
		return;
	// Strtok by the pipe '|' character

	//printf("%s\n", line);

	int num = 1;
	char** tokens = (char**)realloc(NULL, sizeof(char*) * (1));
	tokens[num-1] = strtok(line, "|");

	do {
		//printf("Loop: num = %d;\n", num);
		++num;
		tokens = (char**)realloc(tokens, sizeof(char*) * (num));
		tokens[num-1] = strtok(NULL, "|");
	} while(tokens[num-1] != NULL);

	--num;

	/*for(int i=0; i<num; i++) {
		printf("%s\n", tokens[i]);
	}*/

	// Scan thru the last token to see if a '>' is there.

	char varname[400];
	memset(varname, 0, 400);
	int defaultname = 0;

	if(GetData()->vndc_enabled) { // This is an extension.
		for( int i = strlen(tokens[num-1]); i >= 0; i-- ) {
			if(tokens[num-1][i] == '>') {
				// Yes there is.
				defaultname = i + 1;
				tokens[num-1][i] = '\0';
				break;
			}
		}
	}

	if(defaultname > 0) {
		strncpy(varname, &tokens[num-1][defaultname], 400);
	}
	else {
		strncpy(varname, "selected", 400);
	}

	// For each choice, we need to print text and make a click area. This will use the text function.
	int *choices = (int*)calloc(sizeof(int), num * 2);

	for(int i=0; i < num*2; i+=2) {
		choices[i] = GetData()->text_y;
		char* outarr = (char*)calloc(sizeof(char*), strlen(tokens[i/2])+3);
		outarr[0] = '0' + (i/2);
		outarr[1] = '.';
		outarr[2] = ' ';
		strncpy(&outarr[3], tokens[i/2], strlen(tokens[i/2]));
		op_text(outarr);
		free(outarr);
		choices[i+1] = GetData()->text_y;
	}

	GetData()->choice_coords = choices;
	GetData()->choices = num;

	while(GetData()->choices > 0) {
		if(GetData()->ctx->GetQuit()) goto killed;

		GetData()->ctx->Flush();

		GetData()->ctx->StartSync();
			GetData()->ctx->Input();
		GetData()->ctx->EndSync();
	}

	// Get the chosen option and load it to a variable (abs of course)

	GetData()->s_flags[0][std::string(varname)] = -(GetData()->choices);

	// Clear after choices. Period.
	op_cleartext();

killed:

	// Also reset the 'need to click' since we just did.
	GetData()->wait_input = false;
}
