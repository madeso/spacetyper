#include <SDL2/SDL.h>
#include <iostream>

#include "spacetyper/gl.h"

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0) {
    std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow(
        "Space Typer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (window == NULL) {
    std::cerr << "Failed to create window " << SDL_GetError() << "\n";
    return -1;
  }

  SDL_GL_CreateContext(window);
  std::cout << "Created OpenGL " << glGetString(GL_VERSION) << " context" << "\n";

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}