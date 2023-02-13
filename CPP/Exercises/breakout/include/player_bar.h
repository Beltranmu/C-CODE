#ifndef __PLAYER_BAR_H__
#define __PLAYER_BAR_H__ 1

#include "structs.h"

void InitPlayer(BrickStruct *player);
void DrawBlock(SDL_Renderer* render, BrickStruct *brick);
void ControlPlayer(SDL_Event event, const int width, BrickStruct *player);
void MovePlayer(BrickStruct *player, int width);

#endif // __PLAYER_BAR_H__