/*
 * Clone Breakout
 * Author: Javier Benito Abolafio
 * University Development 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bricks.h"

// Utility macros
#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

const int kWindowHeight = 1000;
const int kWindowWidth = 700;

BrickStruct g_player_bar;
BrickStruct g_ball;
RowStruct *g_main_row = NULL;

bool ExitWindow(SDL_Event key_event){	// Checks if Escape has been pressed or not
	
	if(key_event.key.keysym.sym == SDLK_ESCAPE){
		
		return true;
		
	}else{
		
		return false;
		
	}
	
}

int main(int argc, char** argv){

  srand(time(NULL));

	// Init Window
  CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	
  SDL_Window* window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), kWindowWidth, kWindowHeight, SDL_WINDOW_OPENGL);
	CHECK_ERROR(window == NULL, SDL_GetError());
	
	// Create Renderer to be drawn
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);	
	CHECK_ERROR(renderer == NULL, SDL_GetError());
	
	// Init Funcs
	CreateAllRows(&g_main_row);
	InitPlayer(&g_player_bar);
	InitBall(&g_ball, (rand()% 121) + 30);
	
	const unsigned char fps = 60;
	double current_time, last_time;
	
	bool running = true;
  SDL_Event event;

	while(running) {
		
		last_time = SDL_GetTicks();
		
		// Input Events
		while(SDL_PollEvent(&event)) {
			
			running = !/*((event.type == SDL_QUIT) || */ExitWindow(event);//);
			ControlPlayer(event, kWindowWidth, &g_player_bar);
				
		}
		
    // Update Loop
		MoveBall(&g_ball);
		MovePlayer(&g_player_bar, kWindowWidth);
    BallToBar(&g_ball, g_player_bar);
    BallToWall(&g_ball, kWindowWidth, kWindowHeight);
		
		// Clear screen
		SDL_SetRenderDrawColor(renderer,0,0,0, 255);
		SDL_RenderClear(renderer);
		
		// Draw Events		
		DrawBlock(renderer, &g_player_bar);
		CheckList(g_main_row, renderer);	// Checks the list of bricks and draws them
		DrawBall(renderer, &g_ball);
		
		// Show what was drawn
		SDL_RenderPresent(renderer);
		
		do{
			
			current_time = SDL_GetTicks();
		
		}while((current_time - last_time) <= 1000/fps);		
		
	}

	// Release resources
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
  return 0;
}