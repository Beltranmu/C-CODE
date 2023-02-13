/**
 * Antekeland 2020
 * Author: Ricardo Beltrán Muriel
 * Mail: beltranmu@esat-alumni.com
 * University Development @ESAT
 
 * Scene in the combat when you see the "animation of hiting"
 */


#ifndef __FIGHTCENE_H__
#define __FIGHTCENE_H__ 1

#include "scene.h"


class FightScene : public Scene{
  
  public:
  
  void init() override;
  void input(SDL_Event* eve) override;
  void update() override;
  
};


#endif