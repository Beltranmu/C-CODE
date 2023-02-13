/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 


#ifndef __COMBATSCENE_H__
#define __COMBATSCENE_H__ 1

#include "scene.h"
#include "sprite.h"
#include "logic.h"
#include "rect.h"
#include "label.h"

class CombatScene : public Scene{
  
  public:
  
    CombatScene();
    virtual ~CombatScene();
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
    //void quit()override;
  
  //Atributes

    Rect combat_button_[4];     ///@var the rect of the four buttons for the ui
    Label actions_text_[4];     ///@var the text of the four buttons for the ui
    unsigned char attacking_;   ///@var 0 = not attacking; 1 = attacking.

    Sprite cave_[2];            ///@var the sprites of the two combat boards
    int16_t num_turns_;         ///@var [0, 3] = Characters, >3 = IA
    unsigned char current_character_; ///@var is the id of the character that is moving
    unsigned char total_turns_;     ///@var how many turn has on round number of character + number of enemies
    unsigned char attack_range_;    ///@var identifies the range of the attack that the mouse hovers
    uint8_t alpha_;                 ///@var tue value of the alpha channel for a specific texture
    uint8_t inc_alpha_;             ///@var the increment/decrement of the alpha variable
    uint8_t num_range_rect_;        ///@var number of rectangles to show an attack's range
  
};


#endif