/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#ifndef __TILE_H__  
#define __TILE_H__ 1

#include "SDL.h"

class Tile{
  
  public:
  
  //Atributes
  
  unsigned char state_; // Cell State
  unsigned char type_;  // Cell Type
  SDL_Rect info_;        // (x,y) and width and heigh
  
};


#endif