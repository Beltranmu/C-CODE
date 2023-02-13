#ifndef __BRICKS_H__
#define __BRICKS_H__ 1

#include "structs.h"
#include <stdlib.h>
#include "player_bar.h"
#include "ball.h"

void CreateBrick(BrickStruct **brick, int num_brick, int row_order);
void CreateRow(RowStruct **main_row, int order);
void CreateAllRows(RowStruct **row_list);
void DrawBrick(SDL_Renderer* render, BrickStruct *brick);
void CheckList(RowStruct *main_row, SDL_Renderer* render);
void BallToBar(BrickStruct *ball, BrickStruct player_bar);
void BallToWall(BrickStruct *ball, int width, int height);

#endif // __BRICKS_H__