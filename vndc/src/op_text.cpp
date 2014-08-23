#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements text vnds function.
 * text string
 */

void op_text(char* string) {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;

	// Search thru for vars and rebuild string.

	// Linebreak on zero-length, and render size exceed.
	if (GetData()->text_y > (GetData()->render_y2) || strlen(string) < 1)
		op_cleartext();

	if(strlen(string) < 1)
		return;

	// Improvised linebreak. Also, an extension.
	if(GetData()->vndc_enabled && string[1] == '-' && string[2] == '-' && string[3] == '-') {
			op_cleartext();
	}
	// Improvised voice detect. Extension.
	// This is based on toggling.
	// If the quote is incomplete, it will be stopped on end.
	// If not, it will continue voice until the next quoted line.
	if(GetData()->vndc_enabled) {
		GetData()->is_spoken_line = false;

		int quotes = 0;
		for(int i=0; i < (int)strlen(string); i++) {
			if(string[i] == '"') quotes++;
		}
		if(quotes > 0) GetData()->is_spoken_line = true;
	}
	// Wait for input, then blank
	if(!strcmp(string, "!")) {
		GetData()->wait_input = true;
		Wait();
		op_cleartext();
	}
	// Blank screen
	else if(!strcmp(string, "~")) {
		op_cleartext();
	}
	// Output
	else {
		bool noclick = false;
		// No click required
		if(string[0] == '@') {
			noclick = true;
			string = &string[1];
		}


		TextManager* txt = GetData()->ctx->Text();

		if(txt->TestLen(string) > (GetData()->render_x2 - GetData()->render_x1)) {

			/* new algo */
			char** ptrs = NULL;
			int lines = 0;

			int len = strlen(string);

			int counted = 0;

			while(counted < len) {
				char* pt_start = &string[counted];
				char* pt_end = &pt_start[strlen(pt_start)];

				while(pt_end > pt_start && txt->TestLen(pt_start) > (GetData()->render_x2 - GetData()->render_x1)) {
					*pt_end = ' ';
					--pt_end;

					while (*pt_end != ' ' && pt_end > pt_start) --pt_end;

					*pt_end = '\0';
				}

				#ifdef DEBUG_OVERKILL
				printf("Reduced line %d: %s\n", lines, pt_start);
				#endif

				ptrs = (char**)realloc(ptrs, sizeof(char*)*(lines+1));

				ptrs[lines] = pt_start;

				counted += strlen(pt_start) + 1;

				++lines;
			}

			if( ( lines * 35 + GetData()->text_y ) > GetData()->render_y2 )
				op_cleartext();

			for(int i=0; i < lines; i++) {
				//printf("[br] %s\n", ptrs[i]);
				txt->Render(ptrs[i], GetData()->text_x, GetData()->text_y);
				GetData()->text_y += 35;
			}

			free(ptrs);

		}
		else {
			txt->Render(string, GetData()->text_x, GetData()->text_y);
			GetData()->text_y += 35;
		}

		if(!noclick) {
			GetData()->wait_input = true;
		}
	}
}
