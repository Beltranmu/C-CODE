/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */


#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__ 1

#include <stdint.h>
#include "entity.h"

class Texture;

class Background : public Entity{
  
  public:
 
  Background();
  virtual ~Background();
  /**
   *@brief init the background
   *@param const RBM::Transform2 tr the initial transform of the background
   *@param const uint8_t hs 0 = horizontal mov desactivated otherwise activated
   *@param const uint8_t vs 0 = horizontal mov desactivated otherwise activated
   *@param Texture& tex the texture of the background
   *@param const RBM::Vec2* vel speed of the background
  */
  void init(const RBM::Transform2 tr, 
            const uint8_t hs, 
            const uint8_t vs,
            Texture& tex,
            const RBM::Vec2* vel);
  /**
   *@brief Update the position of the background using board width and height
   *@param float dt delta time
  */               
  void updateB(float dt); 
  /**
   *@brief Update the position of the background using window width and height
   *@param float dt delta time
  */    
  void update(float dt);
  /**
   *@brief draw the background and tile it if is necesary
   *@param SDL_Renderer* ren. Renderer
  */  
  void draw(SDL_Renderer* ren) override;

  
  uint8_t horizontal_scroll_; /** @var 0 =  no horizontal mov otherwise, yes */
  uint8_t vertical_scroll_;   /** @var 0 =  no vertical mov otherwise, yes */
  RBM::Vec2 velocity_;        /** @var vector velocity of the bacground*/
  Texture* texture_;          /** @var texture of the background*/
  
};;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


#endif


