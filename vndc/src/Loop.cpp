#include "Zero.hpp"

#include "Funcs.hpp"

void Wait() {
	// If wait wasn't specified, don't halt.
	// Don't even bother with extensions off.
	// If we're currently in a quote, leave it alone.
	// If the line is spoken, then halt previous spoken lines.
	// If the skip key is held, just gogogo

	bool stop_voice = GetData()->wait_input && GetData()->vndc_enabled;

	while((GetData()->wait_input && !GetData()->ctx->GetQuit())) {
		GetData()->ctx->Input();

		GetData()->ctx->Flush();

		if(GetData()->debug_to_shell)
			ParseShell();

		if(GetData()->ctx->GetInput(1)) break;
	}

	if(stop_voice)
		op_sound((char*)"~", NULL);

}

void Loop() {
	if(GetData()->ctx->GetQuit()) return;
		Parse();

		// We don't clear. This system uses dirty blits. ;P
		GetData()->ctx->Flush();

	GetData()->ctx->StartSync();

		Wait();

	GetData()->ctx->EndSync();
}

void Setup() {
	// Init window
	GetData()->ctx->InitWindowLogical(GetData()->physical_w, GetData()->physical_h, GetData()->screen_w, GetData()->screen_h, false, true);

	GetData()->window_name = (char*)calloc(sizeof(char), 400);
	sprintf(GetData()->window_name, "%s", "VNDC Interpreter ");
	GetData()->ctx->SetTitle(GetData()->window_name);

	GetData()->ctx->InputMode(Burst);

	// Input
	GetData()->ctx->RegisterMouse(&InputAdvance);
	GetData()->ctx->RegisterInput(SDLK_ESCAPE, &QuitKey);
	GetData()->ctx->RegisterInput(SDLK_LCTRL, &NopKey);

	GetData()->s_flags = new std::map<std::string, int>();
	GetData()->g_flags = new std::map<std::string, int>();

	GetData()->ctx->Text()->Outline(2);
	GetData()->ctx->Text()->SetColor(255,255,255,255);

	op_cleartext();
}
