/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 

#ifndef __CHARACTER_H__
#define __CHARACTER_H__ 1 

#include "entity.h"
#include "sprite.h"
#include "database.h"



enum kEnumProfession{

  kEnumProfession_Warrior = 0,
  kEnumProfession_Archer ,
  kEnumProfession_Sorceress ,
  kEnumProfession_Berserker ,
  kEnumProfession_Valkyrie ,
  kEnumProfession_Darkknight ,
  kEnumProfession_Healer ,
  kEnumProfession_Guardian ,
  kEnumProfession_Hashashin ,
  kEnumProfession_Mage ,
  kEnumProfession_Monster ,
};

class Character : public Entity{

  public :
  Character();
  ~Character();
  /**
   * @brief init a character
   * @param unsigned char id. id of the new character.
  */
  void init(unsigned char id);
   /**
   * @brief init a character with professiona and a id
   * @param int prof, the id of the profession
   * @param unsigned char id. the id of the character
  */
  void init(int prof, unsigned char id) ; 
  /**
   * @brief init a enemy with professiona and a id
   * @param int prof, the id of the profession
   * @param unsigned char id. the id of the character
  */
  void initEnemy(const int lvl, const int id);
  
  
  /**
   * @brief Copy the visual information of a character
   * @param Character& c. The character that you want to copy
  */
  void cpyCharacter(Character& c);
  
  /**
   * @brief Update the sprite of the character to face it at the direction of your movement
  */
  void updateSpriteC(); 
  /**
   * @brief Calculate the best movement for the enemies
  */
  void endTile(); 
  /**
   * @brief Mov the ia
  */ 
  void iaMov();
  /**
   * @brief Cotains all the IA behaviour
  */ 
  void iaBehaviour();
  /**
   * @brief reset the variables of the character once a round is comlpeted
   */
  void reset();
  
  /**
   * @brief Upgrade the stats of a character
   */
  void levelUp();
  
  
  /**
  * @brief Calculate the manhantan distance between you and a target
  * @param const SDL_Rect* tr_rect the target.
  */
  int32_t mhDistance(const SDL_Rect* tr_rect);
  /**
  * @brief Calculate the radius distance between you and a target
  * @param const SDL_Rect* tr_rect the target.
  */
  int32_t mhDistanceV2(const SDL_Rect* tr_rect);
  
  /**
  * @brief Check if I can mov to this position
  * @param const int next_pos_x the x coord of the position
  * @param const int next_pos_y the y coord of the position
  * @return true if you can move otherwise you can't
  */
  bool CheckBeforeMove(const int next_pos_x, 
                       const int next_pos_y);
 /**
  * @brief Check if your next position is once that you have already visited
  * @param const int next_pos_x the x coord of the position
  * @param const int next_pos_y the y coord of the position
  * @return true if you can move otherwise you can't
  */
  bool CheckPreviousMovs(const int next_pos_x, 
                         const int next_pos_y);  
 /**
  * @brief Move the character
  * @param SDL_Event* e. SDL event to get the input.
  */                         
  void movCharacter(SDL_Event* e);
 /**
  * @brief Move the character while you are in combat
  * @param SDL_Event* e. SDL event to get the input.
  */   
  void movCharacterCombat(SDL_Event* e);
  /**
  * @brief Reciebe damage from other 
  * @param Character c. The one that deal you dmg.
  * @param const uint8_t range. the range of it attack
  */  
  void takeDamage(Character* c, const uint8_t range);
  
  /**
  * @brief draw the character
  * @param SDL_Renderer* ren. SDL renderer
  */    
  void draw(SDL_Renderer* ren) override;

  char name_[32] = "";  ///@var name of the character
  char name2_[32] = ""; ///@var name2 of the character
  
  AttacksData char_attacks_[4]; ///@var the info of the character attacks. 0 means no attack
  uint8_t current_att_;         ///@var the current att that you have
  
    ///@var base_ Base stats of the characters.
    Character_Stats base_;
    ///@var current_ Current stats that change over the combat.
    Character_Stats current_;

    unsigned char char_id_;  ///@var the character id
    
    int32_t profession_;         ///@var the profession of the character
    int32_t xp_;                 ///@var the xp of the character
    int32_t level_;              ///@var the level fo the character
    int32_t direction_;          ///@var the direction of the character
    
    int32_t previous_movs_[11];  ///@var contains the previous visited tile in your movement
    int32_t index_mov_;          ///@var the index of the previous_movs_
    
    SDL_Rect end_tile_mov_;        ///@var have the information of the movemento that the AI has to do
    int32_t cont_mov_;             ///@var used in AI mov that it mov once per second.
    
    SkinsCustomization skin_id_;      /** @var Storages all the skins used for every character */

    Sprite skin_[7];                  /** @var Manages the skin customization */
    Sprite outfit_[10];               /** @var Manages the outfit customization */
    unsigned char generate_mov_;       /** @var If the ia have to generate a mov o not*/
    unsigned char turn_completed_;    /** @var turn_completed_ Identifies whether the turn has been completed by the character. 0 = uncompleted, 1 = completed */
    unsigned char player_attacking_;
    unsigned char attack_chosen_;

};

#endif  // __CHARACTER_H__