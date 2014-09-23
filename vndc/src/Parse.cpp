#include "Zero.hpp"

#include "Data.hpp"

#include "Funcs.hpp"

void ParseCmd(char* line) {

	char *passthru_line = (char*)calloc(sizeof(char), 400);

	char** tokens = NULL;

	strncpy(passthru_line, line, sizeof(char)*400);

	if(GetData()->verbose) printf("[scr] %s\n", line);

	int num = 1;
	tokens = (char**)realloc(NULL, sizeof(char*) * (1));
	tokens[num-1] = strtok(line, " ");

	do {
		#ifdef DEBUG_OVERKILL
		printf("Loop: num = %d;\n", num);
		#endif
		++num;
		tokens = (char**)realloc(tokens, sizeof(char*) * (num));
		tokens[num-1] = strtok(NULL, " ");
	} while(tokens[num-1] != NULL);

	--num;

	//printf("Made it out of the loop.\n");
	//for (int i = 0; i < num; i++) {
		//printf("[param] %d = '%s'\n", i, tokens[i]);
	//}

	int value_1, value_2;

	if(!strcmp(tokens[0], "bgload")) {
		//printf("[OP::bgload] exec (params:%d)", num-1);
		if (num > 2) {
			sscanf(tokens[2], "%d", &value_1);
			op_bgload(tokens[1], &value_1);
		}
		else
			op_bgload(tokens[1], NULL);
	}
	else if(!strcmp(tokens[0], "setimg")) {
		sscanf(tokens[2], "%d", &value_1);
		sscanf(tokens[3], "%d", &value_2);
		op_setimg(tokens[1], &value_1, &value_2);
	}
	else if(!strcmp(tokens[0], "sound")) {
		// Another ridiculous thing. Second parameter if not
		// specified is implicity one
		if(num > 2) {
			sscanf(tokens[2], "%d", &value_1);
			op_sound(tokens[1], &value_1);
		}
		else
			op_sound(tokens[1], NULL);
	}
	else if(!strcmp(tokens[0], "music"))
		op_music(tokens[1]);
	else if(!strcmp(tokens[0], "text")) {
		// Because of *reasons* (lack of quotes) we use the copy of line,
		// passthru_line and adjust the pointer to use that.
		// strtok destroys the structure
		// And there's the possibility of a zero-length string as well.
		//printf("[op_text] Reconstructed string = '%s'\n", &passthru_line[5]);
		op_text(&passthru_line[5]);
	}
	else if(!strcmp(tokens[0], "choice")) {
		op_choice(&passthru_line[7]);
	}
	else if(!strcmp(tokens[0], "setvar")) {
		if(num > 3) {
			if(!strcmp(tokens[2], "="))
				value_1 = 0;
			else if(!strcmp(tokens[2], "+"))
				value_1 = 1;
			else if(!strcmp(tokens[2], "-"))
				value_1 = -1;
			else if(!strcmp(tokens[2], "~")) // Apparently resets. UGH NOT DOC'D
				value_1 = -2;


			sscanf(tokens[3], "%d", &value_2);
			op_setvar(tokens[1], &value_1, tokens[3]);
		}
		else {
			value_1 = -2;
			op_setvar(tokens[1], &value_1, NULL);
		}
	}
	else if(!strcmp(tokens[0], "gsetvar")) {
		if(!strcmp(tokens[2], "="))
			value_1 = 0;
		else if(!strcmp(tokens[2], "+"))
			value_1 = 1;
		else if(!strcmp(tokens[2], "-"))
			value_1 = -1;

		op_gsetvar(tokens[1], &value_1, tokens[3]);
	}
	else if(!strcmp(tokens[0], "if")) {
		if(!strcmp(tokens[2], "<="))
			value_1 = 0;
		else if(!strcmp(tokens[2], "<"))
			value_1 = 1;
		else if(!strcmp(tokens[2], "=="))
			value_1 = 2;
		else if(!strcmp(tokens[2], "!="))
			value_1 = 3;
		else if(!strcmp(tokens[2], ">"))
			value_1 = 4;
		else if(!strcmp(tokens[2], ">="))
			value_1 = 5;

		op_if(tokens[1], &value_1, tokens[3]);
	}
	else if(!strcmp(tokens[0], "fi"))
		op_fi();
	else if(!strcmp(tokens[0], "jump")) {
		if(num > 2) {
			sscanf(tokens[2], "%d", &value_1);
			op_jump(tokens[1], &value_1, false);
		}
		else
			op_jump(tokens[1], NULL, false);
	}
	else if(!strcmp(tokens[0], "delay")) {
		sscanf(tokens[1], "%d", &value_1);
		op_delay(&value_1);
	}
	else if(!strcmp(tokens[0], "random")) {
		sscanf(tokens[2], "%d", &value_1);
		sscanf(tokens[3], "%d", &value_2);
		op_random(tokens[1], &value_1, &value_2);
	}
	else if(!strcmp(tokens[0], "goto"))
		op_goto(tokens[1]);
	else if(!strcmp(tokens[0], "cleartext"))
		op_cleartext();
	else if(!strcmp(tokens[0], "save") && num == 2)
		op_save(tokens[1]);

	free(tokens);
	free(passthru_line);
}

void ParseShell() {
	bool DebugContinue = true;
	char buffer[400];
	while(DebugContinue) {
		memset(buffer, 0, sizeof(char)*400);

		printf("[scr command] $ ");
		fgets(buffer, 400, stdin);

		// Remove all '\n' from this string
		for(int i=0; i < 400; i++) {
			if (buffer[i] == '\n')
				buffer[i] = '\0';
		}

		if(!strcmp(buffer, "help") || strlen(buffer) < 1) {
			printf("%s\n", "Commands available:");
			printf("\t%s\t\t\t%s\n", "(debug) resume", "Stops debug mode");
			printf("\t%s\t\t\t%s\n", "(debug) quit", "Quits game");
			printf("\t%s\t\t%s\n", "(debug) save [file]", "Saves immediately to file");
			printf("\t%s\t%s\n", "setvar [var] [mod] [val]", "Set save flag");
			printf("\t%s\t%s\n", "gsetvar [var] [mod] [val]", "Set system flag");
			printf("\t%s\t\t\t%s\n", "text [text] ...", "Display text");
			printf("\t%s\t\t%s\n", "sound [file] (num)", "Play sound num times");
			printf("\t%s\t\t\t%s\n", "music [file]", "Play music");
			printf("\t%s\t\t%s\n", "setimg [file] [x] [y]", "Display image at x,y");
			printf("\t%s\t%s\n", "random [var] [low] [high]", "Store random number in var");
			printf("\t%s\t\t%s\n", "jump [file.scr] {N}", "Switch to file.src (goto line N)");
			printf("\t%s\t\t\t%s\n", "goto [label]", "Goto label in current file");
			printf("\t%s\t\t%s\n", "if [var] [op] [val]", "Test condition, and execute till\n\t\t\t\t\tfi if true");
			printf("\t%s\t\t\t\t%s\n", "fi", "Stop computing if");
			printf("\t%s\t\t\t%s\n", "delay [frames]", "Delay frames (at 60fps)");
			printf("\t%s\t\t\t%s\n", "cleartext", "Clear text from screen");
			printf("\t%s\t%s\n", "choice [ch|ch|...] {<var}", "Get input for choice. Store to selected\n\t\t\t\t\t(or var if extensions are on)");
			printf("\t%s\t%s\n", "bgload [file] [fadeframes]", "Load background");
		}
		else if (!strcmp(buffer, "quit")) {
			printf("[debug] Setting quit status & stopping debug.\n");
			GetData()->ctx->SetQuit();
			DebugContinue = false;
		}
		else if (!strcmp(buffer, "resume")) {
			printf("[debug] Exiting debug shell and resuming.\n");
			DebugContinue = false;
			GetData()->wait_input = true;
		}
		else if (!strncmp(buffer, "debugsave", 4)) {
			char* savefile = &buffer[5];
			printf("[debug] Saving to file '%s' NOW.\n", savefile);
			DumpSave(savefile);
		}
		else {
			ParseCmd(buffer);
			GetData()->ctx->Flush();
			GetData()->wait_input = false;
		}
	}
	GetData()->debug_to_shell = false;
}

void Parse() {
	if(GetData()->ctx->GetQuit()) return;

	++(GetData()->currentLine);

	if (!GetData()->accessScriptHandle) {
		fprintf(stderr, "Could not open script data\n");
		exit(-8);
	}

	// Load the next line to this one.
	char* line = GetData()->next_line;

	char* line_copy = line;

	while(line_copy[0] == ' ' || line_copy[0] == '\t') {
		line_copy[0] = '\0';
		line_copy = &line_copy[1];
	}

	// Remove all '\n' from the buffer line
	for(int i=0; i < (int)strlen(line_copy); i++) {
		if (line_copy[i] == '\n')
			line_copy[i] = '\0';
	}

	// The next line is null, because the file is finished.
	if(feof(GetData()->accessScriptHandle) && strlen(GetData()->next_line) == 0) {
		// We've reached EOF. Jump back and return.
		op_jump(GetData()->main_scr[0], NULL, true);
	}

	GetData()->next_line = (char*)calloc(sizeof(char), 400);
	fgets(GetData()->next_line, 400, GetData()->accessScriptHandle);

	// Execute the current line.
	if(strlen(line_copy) != 0) {
		ParseCmd(line_copy);
	}

	free(line);

	// Load the next line.

	return;
}

void InputAdvance(int x, int y, bool down, bool left, bool middle, bool right) {
	if(GetData()->choices > 0) {
		// Choose an option.
		for(int i=0; i<GetData()->choices; i++) {
			if(!down && y > GetData()->choice_coords[i*2] && y < GetData()->choice_coords[i*2+1])
			{
				GetData()->choices = -(i+1); // Negative indicates complete. This is just
				                             // for storage savings.
			}
		}
	}
	else if(left && !down) {
		GetData()->wait_input = false;
	}
}

void QuitKey(bool down) {
	// Flush data to save.scr
	DumpSave((char*)"save.scr");

	GetData()->ctx->SetQuit();
}

void NopKey(bool down) {}
