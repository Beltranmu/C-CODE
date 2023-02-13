/**
 * Antekeland 2020
 * Author: Ricardo Beltrán Muriel
 * Mail: beltranmu@esat-alumni.com
 * University Development @ESAT
 */

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "gamemanager.h"

GameManager* GameManager::instance_gm_ = nullptr;

/// @brief Instantiates only one object (Singletone)
GameManager& GameManager::Instantiate(){

  if(nullptr == instance_gm_){ instance_gm_ = new GameManager(); }

  return *instance_gm_;

}

GameManager::GameManager(){
  map_texture_ = nullptr;
  bg_texture_ = nullptr;
  ground_cave_ = nullptr;
  frozen_cave_ = nullptr;
  over_world_scene_ = 0;
  current_edit_ = 0;
  new_game_ = 0;
  board_seed_ = 0;
  first_time_overworld_ = 0;
}

GameManager::~GameManager(){  
}



void GameManager::init(SDL_Renderer* ren){
  
  first_time_overworld_ = 1;
  map_texture_ = Texture::CreateTexture("../data/resources/tileset.png", ren);
  bg_texture_ = Texture::CreateTexture("../data/resources/bgc.png", ren);
  ground_cave_ = Texture::CreateTexture("../data/resources/ground_cave.png", ren);
  frozen_cave_ = Texture::CreateTexture("../data/resources/frozen_cave.png", ren);
  
  for(int i = 0; i<44; ++i){
    textures_[i] = Texture::CreateEmptyTexture();
    /*textures_[i] = Texture::CreateTexture(
    "../data/skins/body/male/body1.png",ren);*/
  }
  
  enemy_textures_[0] = Texture::CreateTexture("../data/skins/body/male/body10.png", ren);   // Skeleton
  enemy_textures_[1] = Texture::CreateTexture("../data/skins/body/male/body11.png", ren);   // Orc
  enemy_textures_[2] = Texture::CreateTexture("../data/skins/body/male/body12.png", ren);   // Dark Ork
  enemy_textures_[3] = Texture::CreateTexture("../data/skins/body/female/body11.png", ren); // Orc
  enemy_textures_[4] = Texture::CreateTexture("../data/skins/body/female/body12.png", ren); // Dark Ork
  
   
}