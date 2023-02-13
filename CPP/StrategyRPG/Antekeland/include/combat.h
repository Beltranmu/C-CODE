/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 
#ifndef __COMBAT_H__  
#define __COMBAT_H__ 1
#define _CRT_SECURE_NO_WARNINGS

#include "SDL.h"
#include "label.h"
#include "rect.h"

class Character;


class Combat {

  public:
    Combat();
    ~Combat();
    /**
     * @brief init the combat with a character
     * @param the current character that is playing
    */ 
    void initCombat(Character& current_char);

    /**
     * @brief init the combat with a character
     * @param the current character that is playing
    */     
    //void drawMark(SDL_Renderer* ren);
   //void drawAttacks(SDL_Renderer* ren);
    /**
     * @brief update the stats of the current character
    */ 
    void updateStats();
  
    //Atributes
    Character* current_char_; ///@var the current char that is playing
    
    Label stats_title_[2];    ///@var the text of the title of the UI
    Label stats_text_[10];    ///@var the text of statsof the UI
    Rect stats_rect_[6];      ///@var rects used to do the UI
    
    SDL_Rect ui_stats_rect_; ///@var big rect that containt the UI
    float stats_scale_;    ///@var scale of the box where the stats are shown
    
    /// @var It represent the character's turn. 0 the first one, 2 the thrid,...
    int32_t turn_;
    ///@var it means in which satge of the turn the character is (moving, attacking,etc)
    uint8_t stage_; 
    /// @var how many units are in the combat. Allais + ennemies.
    uint8_t n_entities_;
    
};

#endif  // __COMBAT_H__
