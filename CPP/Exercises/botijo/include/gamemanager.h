/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/


#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__ 1

#include <SDL.h>
#include "sprite.h"
#include "board.h"

class GameManager{
  public:
  
  //constant
  static const int kWindowWidth = 640;
  static const int kWindowHeight = 640;
  
  //Methods
  
  void game();
  void input();
  // void update();
  void draw();
  
  
  int init();
  void quit();
  
  void ImGuiSDLProcessEvent1(SDL_Event* e);
  
  //this method maybe will be in other .h
  SDL_Texture loadTexture(const char* imagePath);

  
  // Atributes
  SDL_Window *win_;
  SDL_Renderer *ren_;
  int quit_;
  Sprite sprite_;
  Board board_;
  

};


#endif