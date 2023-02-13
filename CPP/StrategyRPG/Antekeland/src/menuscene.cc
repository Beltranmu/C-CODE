#include "menuscene.h"
#include "gamemanager.h"
#include "game.h"
#include "grid.h"
#include <time.h>

MenuScene::MenuScene(){


}

MenuScene::~MenuScene(){
  
}

void MenuScene::init(){

  GameManager& gM = GameManager::Instantiate();
  SDL_Color color = {236,193,241,95};
  SDL_Color text_color = {255,255,255,255};
  char* font_path = "../data/fonts/BreathFire.ttf";
  int font_size = 25;  
  
  //BG 
  RBM::Transform2 aux_tr = {{0.0f,0.0f},0.0f,{0.0f,0.0f}};
  RBM::Vec2 aux_v2 = {0.5f,0.0f};
  gM.bg_custo_.init(aux_tr,1,0,*gM.bg_texture_, &aux_v2);
  ent_list.push_back(&gM.bg_custo_);

  // Buttons
  gM.menu_scene_[0].dst_rect_.x = 330;
  gM.menu_scene_[0].dst_rect_.y = 180;
  gM.menu_scene_[0].dst_rect_.w = 300;
  gM.menu_scene_[0].dst_rect_.h = 70;
  gM.menu_scene_[0].fill_color_ = color;

  gM.menu_scene_[1].dst_rect_.x = 330;
  gM.menu_scene_[1].dst_rect_.y = 300;
  gM.menu_scene_[1].dst_rect_.w = 300;
  gM.menu_scene_[1].dst_rect_.h = 70;
  gM.menu_scene_[1].fill_color_ = color;

  gM.menu_scene_[2].dst_rect_.x = 330;
  gM.menu_scene_[2].dst_rect_.y = 420;
  gM.menu_scene_[2].dst_rect_.w = 300;
  gM.menu_scene_[2].dst_rect_.h = 70;
  gM.menu_scene_[2].fill_color_ = color;

  ent_list.push_back(&gM.menu_scene_[0]);
  ent_list.push_back(&gM.menu_scene_[1]);
  ent_list.push_back(&gM.menu_scene_[2]);

  // Dest Rect
  SDL_Rect text_dst = {350, 185, 260, 60};
  menu_text_[0].init(font_path, (uint16_t)60, text_color, "New Game", text_dst);

  text_dst = {350, 305, 260, 60};
  menu_text_[1].init(font_path, (uint16_t)60, text_color, "Load Game", text_dst);

  text_dst = {350, 425, 260, 60};
  menu_text_[2].init(font_path, (uint16_t)60, text_color, "Quit Game", text_dst);

  ent_list.push_back(&menu_text_[0]);
  ent_list.push_back(&menu_text_[1]);
  ent_list.push_back(&menu_text_[2]);
  
}

void MenuScene::input(SDL_Event* eve){

  GameManager& gM = GameManager::Instantiate();

  SDL_Point mouse_position = {eve->button.x, eve->button.y};
  SDL_Rect text_dst = {350, 185, 260, 60};
  SDL_Color text_color_white = {255,255,255,255};
  SDL_Color text_color_black = {0,0,0,255};

  // New Game
  if(SDL_PointInRect(&mouse_position, &text_dst)){

    menu_text_[0].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){

      gM.new_game_ = 1;
        
      gM.board_seed_ = time(NULL);
      srand((unsigned int)gM.board_seed_);
      printf("t: %d", gM.board_seed_);
      CreateBoard();
        
      InitLogic();
      CreateMap();
      for(int i = 0; i < Board::kBoardSize; ++i){
        for(int j = 0; j < Board::kBoardSize; ++j){
        gM.layer1_.map_[i][j].initSubSprite();
        gM.layer2_.map_[i][j].initSubSprite();
        }
      }
    }
      
  }else{
    menu_text_[0].changeColor(text_color_white);
  }

  text_dst = {350, 305, 260, 60};
  // Load Game
  if(SDL_PointInRect(&mouse_position, &text_dst)){

    menu_text_[1].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){

      gM.new_game_ = 2;
      gM.data_base_.readGameData();
      gM.data_base_.readCharacterData();
      gM.data_base_.loadCharacter();
      gM.data_base_.loadGameData();
      if(gM.data_base_.games_!= nullptr){
        gM.board_seed_ = gM.data_base_.games_->board_seed;
      }
      else{
         gM.board_seed_ = time(NULL);
      }
      
      srand((unsigned int)gM.board_seed_);
      printf("t: %d", gM.board_seed_);
      CreateBoard();
      InitLogic();
      CreateMap();
      for(int i = 0; i < Board::kBoardSize; ++i){
        for(int j = 0; j < Board::kBoardSize; ++j){
          gM.layer1_.map_[i][j].initSubSprite();
          gM.layer2_.map_[i][j].initSubSprite();
        }
      }
      
    }
      
  }else{
    menu_text_[1].changeColor(text_color_white);
  }

  text_dst = {350, 425, 260, 60};
  // Quit Game
  if(SDL_PointInRect(&mouse_position, &text_dst)){

    menu_text_[2].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){

      /* Do stuff */

    }
      
  }else{
    menu_text_[2].changeColor(text_color_white);
  }

}

void MenuScene::update(){

  GameManager& gM = GameManager::Instantiate();
  gM.bg_custo_.update(1);
    
}

void MenuScene::drawImgui(SDL_Renderer* ren){
}