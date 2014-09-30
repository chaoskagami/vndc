#include "Zero.hpp"

#include "Funcs.hpp"

/*
 * Implements text vnds function.
 * text string
 */

void op_text(char* string) {
	if (GetData()->if_fail != 0 || GetData()->ctx->GetQuit())
		return;

	// Linebreak on zero-length, and render size exceed.
	if (GetData()->text_y > (GetData()->render_y2) || strlen(string) < 1)
		op_cleartext();

	if(strlen(string) < 1)
		return;

	// Improvised linebreak. Also, an extension.
	if(GetData()->vndc_enabled && string[1] == '-' && string[2] == '-' && string[3] == '-') {
		op_cleartext();
	}
	// Improvised Tag display. Extension.
	if(GetData()->vndc_enabled && string[0] == '<' && string[strlen(string)-1] == '>') {
		string[strlen(string)-1] = '\0';
		string = &string[1];
		
		GetData()->ctx->ClearOverlay();

		TextManager* txt = GetData()->ctx->Text();

		int xloc = ( GetData()->screen_w - (txt->TestLen(string) + GetData()->render_x1) );
		
		txt->Render(string, xloc, GetData()->render_y1);

		GetData()->wait_input = true;
		Wait();

		op_cleartext();

		return;
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

		int lines = 0;
		char** ptrs = NULL;

		txt->SplitStringByWidth(string, GetData()->render_x2 - GetData()->render_x1, &lines, &ptrs);

		if( ( lines * GetData()->text_gap + GetData()->text_y ) > GetData()->render_y2 )
			op_cleartext();

		for(int i=0; i < lines; i++) {
			//printf("[br] %s\n", ptrs[i]);
			txt->Render(ptrs[i], GetData()->text_x, GetData()->text_y);
			GetData()->text_y += GetData()->text_gap;
		}

		free(ptrs);

		if(!noclick) {
			GetData()->wait_input = true;
		}
	}
}
