#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_ttf.h"
#include <unistd.h>

typedef struct {
  SDL_Rect rect;   // Rectangle for position and size
  SDL_Color color; // Color of the object
} GameObject;

typedef struct {
  int x, y;
} Vec2;
