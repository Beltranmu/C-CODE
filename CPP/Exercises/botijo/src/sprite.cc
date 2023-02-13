/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#include "sprite.h"
#include "SDL_image.h"



int Sprite::loadTexture(const char* img_path, SDL_Renderer* r){

 // SDL_Surface *img = SDL_LoadBMP(img_path);
 SDL_Surface *img = IMG_Load(img_path);
  if (img == nullptr){
    // SDL_DestroyRenderer(r);
    // SDL_DestroyWindow(w);
    // std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    // SDL_Quit();
    return 1;
  }

  texture_ = SDL_CreateTextureFromSurface(r, img);
  SDL_FreeSurface(img);
  if (texture_ == nullptr){
    // SDL_DestroyRenderer(r);
    // SDL_DestroyWindow(w);
    // std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    // SDL_Quit();
    return 1;
  }
  return 0;
}
  