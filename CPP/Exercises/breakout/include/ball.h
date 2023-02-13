#ifndef __BALL_H__
#define __BALL_H__ 1

#include "structs.h"

void InitBall(BrickStruct *ball, float A);
void DrawBall(SDL_Renderer* render, BrickStruct *ball);
void MoveBall(BrickStruct *ball);

#endif	// __BALL_H__