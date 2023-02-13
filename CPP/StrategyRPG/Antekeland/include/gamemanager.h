/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */




#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__ 1

#include "SDL.h"
#include "database.h"
#include "texture.h"
#include "background.h"
#include "board.h"
#include "logic.h"
#include "character.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "combat.h"
#include "rect.h"

class GameManager{
  public:  
    //Constant
    static const int kWindowWidth = 1024; ///@var width of the windows
    static const int kWindowHeight = 768; ///@var height of the windows
    static const int kBoardWidth = 640;   ///@var width of the board
    static const int kBoardHeight = 640;  ///@var height of the board     
    static const unsigned char kViewSize = 16; ///@var how amny tile you see in the overwolrd scene
    static const int kImGuiWidth = 500;   ///@var width of the ImGui window
    static const int kImGuiHeight = 500;  ///@var height of the ImGui window
    ImGuiWindowFlags window_flags = 0;    ///@var Imguoi flags
   
    
    //Methods 
    /**
     *@brief init of variables of gamemanager
     */
    void init(SDL_Renderer* ren);
    ~GameManager();
    //It has to be change of .h
   

    //Factory
     /**
     *@brief Factory of Gamemanager
     */
    static GameManager& Instantiate();
    
    // Atributes
    Board layer1_; ///@var first graphical map
    Board layer2_;     ///@var second graphical map
    Logic board_[Board::kBoardSize][Board::kBoardSize]; ///@var first logical map
    Logic units_[Board::kBoardSize][Board::kBoardSize]; ///@var second logical map
    Logic logic_board_[16][16]; ///@var Logical Combat map
    Logic units_board_[16][16]; ///@var Logical units Combat map
    Character player_[5];       ///@var All the character the last is a copy of the first
    Character NPC_[10];         ///@var All the enemies
    Texture* map_texture_;      ///@var Texture of the map
    Texture* bg_texture_;       ///@var Texture of the background 
    Texture* ground_cave_;      ///@var Texture of the first cave 
    Texture* frozen_cave_;      ///@var Texture of the second cave 
    Texture* textures_[44];     ///@var Texture of the characters
    Texture* enemy_textures_[5];    ///@var Texture of the enemies
    Background bg_custo_;       ///@var the background with parallax
    Combat combat_;             ///@var combat UI
    DataBase data_base_;        ///@var database variable
    unsigned char current_edit_;    ///@var Current character in edition
    unsigned char enter_cave_;      ///@var 1 = Enter cave
    unsigned char exit_cave_;       ///@var 1 = Exit cave

      // Menu Scene
    Rect menu_scene_[3];          ///@var rects of the menu scene
    unsigned char new_game_;      ///@var if it is a new game 1 or not 0, loaded game 2

    int32_t over_world_scene_;     ///@var if you are in the overworld scene 1 or not 0
    time_t board_seed_;            ///@var the board of the random map
    uint8_t first_time_overworld_; ///@var the board of the random map
    Rect ui_rects_[2];             ///@ver rect for the background of the UI
    
  private:
    //Methods
    GameManager();
    static GameManager* instance_gm_; ///@var instance of the singletone     

};

#endif  // __GAMEMANAGER_H__