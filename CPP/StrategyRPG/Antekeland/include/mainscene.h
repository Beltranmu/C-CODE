/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */

#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__ 1

#include "scene.h"
#include "label.h"
#include "rect.h"

class MainScene : public Scene{
  
  public:
  
  MainScene();
  virtual ~MainScene();
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
  /**
   *@brief Contains the functions that has to be done before leave that scene
  */
  void quit()override;

  Rect mainscene_button_[3]; ///@var the 3 rects that are use to do the background of the buttons 
  Label mainscene_text_[3]; ///@var the text of the 3 buttons
};


#endif