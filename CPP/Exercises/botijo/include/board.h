/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/


#ifndef __BOARD_H__
#define __BOARD_H__ 1

#include "tile.h"
#include "sprite.h"

  const unsigned char kBoardSize = 64;
  const unsigned char kViewSize = 64;
  
  //Constant of procedural generation
  const float kCellconcentration = 0.55f;
  const int kChanceToMove = 721;
  const unsigned char kSearchRange = (kBoardSize/4)*3;
  const unsigned char kNState = 7;
  
class Board{
  
  public:
  
  //Constant
  
  
  //Methods
  void initMap(const int window_w, const int window_h);
   
  void drawMap(SDL_Renderer* renderer);
  
  
  //Atributes
  Tile map_[kBoardSize][kBoardSize];
  Sprite map_sprite_ ;
  
};


#endif