#ifndef ASDP_DEFINE_H
#define ASDP_DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL2/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "nuklear_main.h"
#include "nuklear_sdl_renderer.h"

// include menu headers
#include "ui/states.h"
#include "ui/main_menu.h"
#include "ui/window.h"
#include "ui/play_menu.h"
#include "ui/leaderboard_menu.h"

#endif // ASDP_DEFINE_H