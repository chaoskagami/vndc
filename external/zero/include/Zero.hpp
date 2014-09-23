#ifndef _ZERO_LIBRARY_HPP
#define _ZERO_LIBRARY_HPP

#ifdef __WIN32
#include <windows.h>
#endif

#include <gitrev.hpp>

#ifdef USE_ANDROID
#define printf SDL_Log
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RED_MASK 	0xff000000
    #define GREEN_MASK 	0x00ff0000
    #define BLUE_MASK 	0x0000ff00
    #define ALPHA_MASK 	0x000000ff
#else
    #define RED_MASK 	0x000000ff
    #define GREEN_MASK 	0x0000ff00
    #define BLUE_MASK 	0x00ff0000
    #define ALPHA_MASK 	0xff000000
#endif

#include "AudioManager.hpp"
#include "ContextManager.hpp"
#include "TextManager.hpp"
#include "UDisplayable.hpp"
#include "VertexController.hpp"

#endif
