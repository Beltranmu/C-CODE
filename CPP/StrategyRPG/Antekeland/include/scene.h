/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */


#ifndef __SCENE_H__
#define __SCENE_H__ 1

#include "SDL.h"
#include <iostream> 
#include <list> 


class Entity;

class Scene{
  
  public:
  
  // Methods
  Scene();
  virtual ~Scene();
  /**
   * @brief init the scene. Pure virtual
  */
  virtual void init()=0;
  /**
   *@brief Contains the input of each scene. Pure virtual
   *@param SDL_Event* eve. Event of SDL
  */
  virtual void input(SDL_Event* eve)=0;
  /**
   *@brief Contains the update fuctions of each scene. Pure virtual
  */
  virtual void update()=0;
  /**
   *@brief use to draw extra informatio that is not child of entity (Imgui).
   *@param SDL_Rendeder* ren, SDL rendeder
  */
  virtual void drawImgui(SDL_Renderer* ren)=0;
  
  /**
   *@brief Contains the functions that has to be done before leave that scene
  */
  virtual void quit();
  
  /**
   *@brief Draw all the enity list
  */ 
  void draw(SDL_Renderer* ren);
  
  //Atributes
  
  //Sustituir por el vector de IA
  std::list <Entity*> ent_list; ///@var list of all entities that are in the scene and must be drawn
  
  
};

#endif