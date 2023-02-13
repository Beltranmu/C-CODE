/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>

#include "label.h"

/** @brief Process mouse events for the ImGuiSDL binding.
 *
 * @param Pointer to an SDL_Event captured this frame.
 */

void Input(int* quit, SDL_Window* window) {
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    
      if (event.type == SDL_QUIT) {
          *quit = true;
      }
      if (event.type == SDL_WINDOWEVENT && 
          event.window.event == SDL_WINDOWEVENT_CLOSE && 
          event.window.windowID == SDL_GetWindowID(window)) {
          *quit = true;
      }
      // Add the rest of input events here: keyboard, gamepad, etc...
    }
}

void LabelManagement(Label& l){

  char* path = "../data/fonts/AntiqueQuestSt.ttf";
  char* msg = "This is a Label test";
  SDL_Color col = {255,255,255,255};
  SDL_Rect pos = {200, 200, 500, 40};

  l.init(path, 40, msg, col, pos);

}

int main(int argc, char* argv[]){

  srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

  SDL_Window *win = SDL_CreateWindow("Ejercicio 10", 700, 100, 1280, 720, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    //std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                         SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    //std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  TTF_Init();

  Label l0;
  int quit = 0;

  LabelManagement(l0);

  while (!quit) {
    Input(&quit, win);
      
    //First clear the renderer
    SDL_SetRenderDrawColor(ren, 0,0,0,0);
    SDL_RenderClear(ren);

    l0.drawText(ren);
    
    //Update the screen
    SDL_RenderPresent(ren);
  }

  // Clean up our objects and quit
  TTF_Quit();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

	return 0;
}
