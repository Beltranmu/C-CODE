/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 
#ifndef __ENTITY_H__
#define __ENTITY_H__ 1

#include <stdint.h>
#include "SDL.h"
#include "rbmmath.h"

class Entity{
  public:
  
  //Methods
  
  Entity();
  Entity(const Entity& e);
  
  virtual ~Entity();
  /** 
   *@brief init the entity
  */
  virtual void init();
  /** 
   *@brief draw the entity.Pure virtual
  */
  virtual void draw(SDL_Renderer* ren)=0;;
  /*
  void init(const int32_t t, 
            const int8_t e, 
            const RBM::Transform2* tr,
            const SDL_Rect r);*/
  
  
  void set_position(RBM::Vec2 v);
  RBM::Vec2 position();
  
  void set_rotation(float r);
  float rotation();
  
  void set_scale(RBM::Vec2 v);
  RBM::Vec2 scale();
   
  int32_t id();
  
  //Atributes

  static int32_t next_id_; ///@var the next id of a new entity
  int32_t tag_;            ///@var the tag of the entity
  uint8_t enabled_;        ///@var if it is enabled == 1 or not == 0 
  SDL_Rect dst_rect_;      ///@var where are you going to draw it
 
  protected:
  RBM::Transform2 transform_; ///@var its transform
  
  private:
  int32_t id_;                ///@var its unic id
};

#endif
