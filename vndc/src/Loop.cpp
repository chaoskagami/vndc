#include "Zero.hpp"

#include "Funcs.hpp"

void Wait() {
	// If wait wasn't specified, don't halt.
	// Don't even bother with extensions off.
	// If we're currently in a quote, leave it alone.
	// If the line is spoken, then halt previous spoken lines.
	// If the skip key is held, just gogogo

	// bool stop_voice = GetData()->wait_input && GetData()->vndc_enabled && GetData()->is_spoken_line;

	while((GetData()->wait_input && !GetData()->ctx->GetQuit())) {
		GetData()->ctx->Input();

		GetData()->ctx->Flush();

		if(GetData()->debug_to_shell)
			ParseShell();

		if(GetData()->ctx->GetInput(1)) break;
	}
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
	GetData()->ctx->InitWindowLogical(GetData()->physical_w, GetData()->physical_h, GetData()->screen_w, GetData()->screen_h, GetData()->fullscreen, (GetData()->sw_rendering ? Software : Accel2d ));

	GetData()->window_name = (char*)calloc(sizeof(char), 400);
	sprintf(GetData()->window_name, "%s", "VNDC Interpreter ");
	GetData()->ctx->SetTitle(GetData()->window_name);

	GetData()->ctx->InputMode(Burst);

	// Input
	GetData()->ctx->RegisterMouse(&InputAdvance);
	GetData()->ctx->RegisterInput(SDLK_ESCAPE, &QuitKey);
	GetData()->ctx->RegisterInput(SDLK_LCTRL, &NopKey);

	GetData()->s_flags = new std::map<std::string, int>();

	// Font
	GetData()->ctx->Text()->LoadFont((char*)"default.ttf", GetData()->text_size);
	GetData()->ctx->Text()->SetFontUsed(1);

	GetData()->ctx->Text()->Outline(1);
	GetData()->ctx->Text()->SetColor(255,255,255,255);

	Data_PostInit();

	op_cleartext();
}
