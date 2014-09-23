#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "gitrev.hpp"

#include "Data.hpp"

void Parse();				// This reads commands from files.
void ParseShell();			// This reads commands from the shell.
void ParseCmd(char* line); 	// This does the heavy lifting.
void Loop();
void Wait();
void Setup();

void InputAdvance(int x, int y, bool down, bool left, bool middle, bool right);
void QuitKey(bool down);
void NopKey(bool down);

void op_bgload(char* file, int* fadetime);
void op_cleartext();
void op_delay(int* frames);
void op_fi();
void op_gsetvar(char* var, int *modifier, char *value);
void op_if(char* left, int* op, char* right);
void op_music(char* file);
void op_random(char* var, int* low, int* high);
void op_setimg(char* file, int* x, int* y);
void op_setvar(char* var, int *modifier, char *value);
void op_sound(char* file, int* times);
void op_text(char* string);
void op_jump(char* file, int* lineTo, bool isSave);
void op_goto(char* label);
void op_choice(char* choices);

// Extensions.
void op_save(char* file);

#endif
