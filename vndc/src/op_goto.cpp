#include "Zero.hpp"

#include "Funcs.hpp"

/* Implements vnds goto function
 * goto label
 * File pointer is provided so we can rewind.
 */

void op_goto(char* label) {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;
	FILE** infile = &(GetData()->accessScriptHandle);

	rewind(*infile);

	char *line = (char*)calloc(sizeof(char), 400);

	int line_to = 0;

	bool found = false;
	while(!found) {
		line_to++;
		fgets(line, 400, *infile);

	// Remove all '\n' from this string
		for(int i=0; i < 400; i++) {
			if (line[i] == '\n')
				line[i] = '\0';
		}

		int num = 1;
		char** tokens = (char**)realloc(NULL, sizeof(char*) * (1));
		tokens[num-1] = strtok(line, " ");

		do {
			//printf("Loop: num = %d;\n", num);
			++num;
			tokens = (char**)realloc(tokens, sizeof(char*) * (num));
			tokens[num-1] = strtok(NULL, " ");
		} while(tokens[num-1] != NULL);

		--num;

		if(!strcmp(tokens[0], "label"))
			if(!strcmp(tokens[1], label))
				found = true;
	}
	GetData()->currentLine = line_to;
}
