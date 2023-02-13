#ifndef __STRUCTS_H__
#define __STRUCTS_H__ 1

#include "SDL.h"
#include <stdio.h>
#include "JBAmath.h"

struct BrickStruct{	// Both element bricks and player_bar
	
	int id;
	int hp;
	int speed;
	JBA::Vector2 vec_dir, position;
	SDL_Rect body_info; // body_info.x, body_info.y, body_info.w, body_info.h
	SDL_Color color;
	BrickStruct *next_brick;
	BrickStruct *previous_brick;
  int last_vec_dir;
	
};

struct RowStruct{
	
	int id;
	RowStruct *next_row;
	RowStruct *previous_row;
	BrickStruct *brick_list;
	SDL_Color row_color;
	
};

#endif // __STRUCTS_H__