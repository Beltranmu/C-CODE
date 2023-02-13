/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com>
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */
 
#ifndef __DATABASE_H__
#define __DATABASE_H__ 1

#include <stdint.h>
#include "sqlite3.h"
extern "C" {
#include "../ADT/adt_memory_node.h"
#include "../ADT/adt_vector.h"
#include "abgs_platform_types.h"
#include "abgs_memory_manager.h"
}

const int32_t kNumSavedGames = 4;  ///@var max num of saved games that we read
const int32_t kNumProfession = 11; ///@var num of saved proffesion in DB
const int32_t kNumAttacks = 16;    ///@var num of saved attacks in the DB
const int32_t kNumCharacter = 4;   ///@var max num of saved characters that we read
const int32_t kNumTiles = 64*64;   ///@var max num of saved tiles that we read

/** @struct Stores the information of the customization of the character */
struct SkinsCustomization{

  int32_t gender;         /** @var Defines whether it's male (1) or female (2) */
  int32_t skin;           /** @var ID of the skin */
  int32_t hair;           /** @var ID of the hair */
  int32_t hair_color;     /** @var ID of the hair color */
  int32_t eyes;           /** @var ID of the eyes */
  int32_t ears;           /** @var ID of the ears */
  int32_t nose;           /** @var ID of the nose */  
  int32_t beard;          /** @var ID of the beard */
  int32_t mustache;       /** @var ID of the mustache */
  int32_t mustache_color; /** @var ID of the mustache */
  int32_t torso;          /** @var ID of the torso */
  int32_t cape;           /** @var ID of the cape */
  int32_t legs;          /** @var ID of the legs */

  int32_t head;           /** @var ID of the helmet/hat */
  int32_t neck;           /** @var ID of the necklace */
  int32_t belt;           /** @var ID of the belt */
  int32_t armor;          /** @var ID of the armor/shirt/dress */
  int32_t back;           /** @var ID of the back */
  int32_t bracelets;      /** @var ID of the bracelets */
  int32_t bracers;        /** @var ID of the bracers */
  int32_t gloves;         /** @var ID of the gloves */
  int32_t pants;          /** @var ID of the pants */
  int32_t feet;           /** @var ID of the boots/shoes */
};

/** @struct Stores the stats of the characters */
struct Character_Stats{

  int32_t hp;
  int32_t mana;
  int32_t physical_att; 
  int32_t magic_att;
  int32_t armor;
  int32_t magic_resist;
  int32_t movement; 
  int32_t mana_regen; 
  int32_t crit_chance; 
};

/** @struct Stores the stats of attacks */
struct AttacksData{

  int32_t id;        ///@var att id
  int32_t dmg;       ///@var damage of the attack
  int32_t mana_cost; ///@var how many mana it costs
  uint8_t range;     ///@var the manhatan distance you need to be to use it.
  uint8_t type;      ///@var the type of the damage magic(2) or physical(1)
  char* name;        ///@var the name of the attack

};


/** @struct Stores information of a board */
struct SaveLoadBoard{
  int32_t id_world;       ///@var id of the game that it belongs
  int32_t logic_enabled;  ///@var save the value of enabled in the logic board
  int32_t logic_enter;    ///@var save the value of enter in the logic board
  int32_t units_enabled;  ///@var save the value of enabled in the uits board
  int32_t layer1_state;   ///@var save the value of state in the layer1
  int32_t layer1_type;    ///@var save the value of type in the layer1
  int32_t layer2_state;   ///@var save the value of state in the layer2
  int32_t layer2_type;    ///@var save the value of type in the layer2
};

/** @struct Stores information of a game*/
struct GameData{
  int32_t board_seed; ///@var seed of the board 
  int32_t id_char_1;  ///@var id of the character 1
  int32_t id_char_2;  ///@var id of the character 2
  int32_t id_char_3;  ///@var id of the character 3
  int32_t id_char_4;  ///@var id of the character 4
};
/** @struct Store the information necesary to save/read a character*/
struct CharacterData{
  int32_t id_character;          ///@var id of the character
  int32_t xp;                    ///@var current xp of the character
  int32_t lvl;                   ///@var current lvl of the character
  int32_t profession;            ///@var profession of the character
  SkinsCustomization custo_data;  ///@var customization data of the character
  
};
/** 
 *@brief Callback to read stats of each class
 */
int callbackProfesion(void *profdata, int argc,
                       char **argv, char **azcolname);
      
/** 
 *@brief Callback to read stats of each attack
 */      
int callbackAttacks(void *attdata, int argc,
                    char **argv, char **azcolname);

/** 
 *@brief Callback to read the information of saved games
 */
int callbackGame(void *gamedata, int argc,
                 char **argv, char **azcolname);
   
/** 
 *@brief Callback to read the information of the board
 */
int callbackBoard(void *boardinfo, int argc,
                  char **argv, char **azcolname);      
     
class DataBase{
  
  public:
  
  DataBase();
  ~DataBase();
  
  /** 
   *@brief open the data base
   *@param const char* path, the parth where is the DB
   *@return return 0 if all goes ok 1 if not.
  */
  int openDB(const char* path);
  /** 
   *@brief close the data base
  */
  void closeDB();
  /** 
   *@brief init all variables to work with DB
  */
  void init();
  
  
  /** 
   *@brief read the information of each class and save it
  */
  void readProfessionData();
  /** 
   *@brief read the information of each game and save it
  */
  void readGameData();
  /** 
   *@brief read the information of each character and save it
  */
  void readCharacterData();
  /** 
   *@brief read the information of each attack and save it
  */
  void readAttacksData();
  /** 
   *@brief read the information of a board and save it
  */
  void readBoardData();
  /** 
   *@brief read the information of the last game saved and save it
  */
  void readLastGame();
  /** 
   *@brief save the information of the current board
  */
  void saveBoardData();
  /** 
   *@brief save all 4 character information in the game
  */
  void saveCharacter();
  
  /** 
   *@brief Save the data of a game characters id and board_seed
  */  
  void saveGameData();
  
  /** 
   *@brief load the information of attacks and professions
  */
  void loadData();
  /** 
   *@brief load the information of the character
  */
  void loadCharacter();
  /** 
   *@brief load the information  of the game character, board,..
  */
  void loadGameData();

  void printProfession();
  
  
  //Atributes
  Vector* prof_vector_;  ///@var vector that is use to save professions info
  Vector* games_vector_; ///@var vector that is use to save games info
  Vector* char_vector_;  ///@var vector that is use to save characters info
  Vector* att_vector_;   ///@var vector that is use to save attacks info
  Vector* board_vector_; ///@var vector that is use to save board info
  

  AttacksData* attacks_[kNumAttacks];    ///@var used to saved the att information 
  Character_Stats* p_[kNumProfession];   ///@var used to saved the profession information 
  GameData* games_;                      ///@var used to saved the game information 
  CharacterData char_data_[kNumCharacter]; ///@var used to saved the character information 
  
  sqlite3* db_;   ///@var pointer to the database
  
  
};

 


//void ReadProffesion(const char* directory);                   

#endif