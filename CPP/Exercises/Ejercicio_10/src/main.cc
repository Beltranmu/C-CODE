#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>

#include "rect.h"

/** @brief Process mouse events for the ImGuiSDL binding.
 *
 * @param Pointer to an SDL_Event captured this frame.
 */

void Input(int* quit, SDL_Window* window, unsigned char* s, unsigned char* array_pointer, Rect r[]) {
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      
      if(event.key.keysym.sym == SDLK_1){
        *s = 1;
      }

      if(event.key.keysym.sym == SDLK_2){
        *s = 2;
      }

      if(event.key.keysym.sym == SDLK_3){
        *s = 3;
      }

      if(event.button.type == SDL_MOUSEBUTTONDOWN && *s == 3 && *array_pointer < 12){

        r[*array_pointer].transform_.position.x = event.button.x;
        r[*array_pointer].transform_.position.y = event.button.y;

        printf("%d %d\n", event.button.x, event.button.y);
        
      }

      if(event.button.type == SDL_MOUSEBUTTONUP && *s == 3 && *array_pointer < 12){

        r[*array_pointer].info_.w = event.button.x - r[*array_pointer].transform_.position.x;
        r[*array_pointer].info_.h = event.button.y - r[*array_pointer].transform_.position.y;

        printf("%d %d %d %d\n", r[*array_pointer].transform_.position.x, r[*array_pointer].transform_.position.y, r[*array_pointer].info_.w,  r[*array_pointer].info_.h);

        ++*array_pointer;
        
      }

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

// Init Rectangles
void InitRects(Rect r[], const unsigned char num){  

  for(int i = 0; i < 12; ++i){

    int rnd = rand()%650;

    r[i].init(50, 50);

    if(num == 1 || num == 2){
      r[i].setTransform({1280.0f, (float)rnd}, 2.0f, {2.0f, 2.0f});
    }

    r[i].setBorderColor(rand()%255, rand()%255, rand()%255, 255);
    
    if(num == 1){
      r[i].setSolid(1);
    }

    if(i < 4){

      r[i].setFillColor(rand()%85, rand()%85, rand()%85, 255);
      r[i].setSpeed(1+rand()%5);

    }else if(4 <= i && i < 8){

      r[i].setFillColor(85+rand()%85, 85+rand()%85, 85+rand()%85, 255);
      r[i].setSpeed(5+rand()%5);

    }else{

      r[i].setFillColor(170+rand()%85, 170+rand()%85, 170+rand()%85, 255);
      r[i].setSpeed(10+rand()%5);

    }

    if(num == 3){

      r[i].info_.x = 0;
      r[i].info_.y = 0;
      r[i].info_.w = 0;
      r[i].info_.h = 0;

    }

  }

}

// Move Rectangles
void MoveRects(Rect r[]){

  for(int i = 0; i < 12; ++i){

    r[i].transform_.position.x -= r[i].speed_;
    r[i].transform_.rotation += 0.05f;

    if(r[i].transform_.position.x + r[i].info_.w <= 0){

      r[i].transform_.position.x = 1280;

    }
    
  }
}

// Draw Rectangles
void DrawRects(Rect r[], SDL_Renderer* rend){

  for(int i = 0; i < 12; ++i){

    r[i].draw(rend);

  }

}

void MainMovement(const unsigned char state, Rect r1[], Rect r2[], Rect r3[]){

  switch(state){

    // Solid Rects
    case 1:

      MoveRects(r1);

    break;

    // Rotating Rects
    case 2:
      
      MoveRects(r2);

    break;

  }

}

void MainDraw(const unsigned char state, Rect r1[], Rect r2[], Rect r3[], SDL_Renderer* ren){

  switch(state){

    // Solid Rects
    case 1:

      DrawRects(r1, ren);

    break;

    // Rotating Rects
    case 2:

      DrawRects(r2, ren);

    break;

    // Draw Rects yourself
    case 3:

      DrawRects(r3, ren);

    break;

  }

}

int main(int argc, char* argv[]){

  srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

  SDL_Window *win = SDL_CreateWindow("Ejercicio 10", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    //std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    //std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  Rect r1[12], r2[12], r3[12];
  
  InitRects(r1, 1);
  InitRects(r2, 2);
  InitRects(r3, 3);

  unsigned char array_ptr = 0;
  unsigned char state = 0;
  int quit = 0;
  while (!quit) {
    Input(&quit, win, &state, &array_ptr, r3);

    // Move Rects
    MainMovement(state, r1, r2, r3);
      
    //First clear the renderer
    SDL_SetRenderDrawColor(ren, 0,0,0,0);
    SDL_RenderClear(ren);

    MainDraw(state, r1, r2, r3, ren);

    //Update the screen
    SDL_RenderPresent(ren);
  }

  // Clean up our objects and quit
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

	return 0;
}