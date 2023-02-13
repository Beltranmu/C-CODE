/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */

#ifndef __RECT_H__
#define __RECT_H__ 1

#include "entity.h"

class Rect : public Entity{
  
  public:
  
  //Methods
  
  Rect();
  virtual~Rect();
  /**
   *@brief init the rect
  */
  void init()override;
 
  //void init(RBM::Transform2* tr);
/*
  void init(SDL_Rect* r, int32_t t, int8_t e, uint8_t c,
            RBM::Transform2* tr,
            SDL_Color* fc, SDL_Color* bc,
            uint8_t s, int32_t speed, float speedr);
  */   

  /**
   *@brief draw the rect
   *@param SDL_Renderer* renderer. Renderer of SDL
  */
  void draw(SDL_Renderer* renderer)override;
 /* void drawT(SDL_Renderer* renderer);
  

  void horizontalMovement(int w);
  
  void changeColor();
  
  void rotate();*/
  
  //Atributes
  SDL_Color border_color_;  ///@var color of the border of the rect
  SDL_Color fill_color_;    ///@var color of the inside of the rect
  uint8_t solid_;           ///@var if it is solid (1) or not(0)
  uint8_t centered_;        ///@var if you want to rotate it arround the center(1) or not(0)
  int32_t speed_;           ///@var displacement speed
  float speed_rot_;         ///@var rotation speed
  
};

#endif 