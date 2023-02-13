/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__ 1

#include <SDL.h>

class Sprite{
  public: 
  
  //Methods
  int loadTexture(const char* img_path, SDL_Renderer* r);

  //Atributes
  SDL_Texture* texture_;

};

#endif