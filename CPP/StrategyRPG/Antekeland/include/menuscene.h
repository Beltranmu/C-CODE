/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 

#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__ 1

#include "scene.h"
#include "label.h"

class Character;

class MenuScene : public Scene{
  
  public:
  
  MenuScene();
  virtual ~MenuScene();
  /**
   * @brief init the scene
  */ 
  void init() override;
  /**
   *@brief Contains the input of each scene
   *@param SDL_Event* eve. Event of SDL
  */
  void input(SDL_Event* eve) override;
  /**
   *@brief Contains the update fuctions of each scene
  */
  void update() override;
  /**
   *@brief use to draw extra informatio that is not child of entity (Imgui).
   *@param SDL_Rendeder* ren, SDL rendeder
  */
  void drawImgui(SDL_Renderer* ren) override;
  
  //Atributes

  Label menu_text_[3]; ///@var Label that contains the first 3 button
  
};

#endif  // __MENUSCENE_H__