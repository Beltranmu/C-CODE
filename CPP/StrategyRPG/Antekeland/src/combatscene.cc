#include "stdlib.h"

#include "combatscene.h"
#include "gamemanager.h"
#include <stdlib.h>


CombatScene::CombatScene(){
  
  num_turns_ = 0;
  current_character_ = 0;
  total_turns_ = 0;
}

CombatScene::~CombatScene(){

}

void CombatScene::init(){

  num_range_rect_ = 0;
  attacking_ = 0;
  attack_range_ = 0;

  GameManager& gM = GameManager::Instantiate();
  gM.over_world_scene_=0;
  SDL_Rect r1 = {0, 0, 640, 640};
  
  cave_[0].initSprite(*gM.ground_cave_, &r1, &r1);
  cave_[1].initSprite(*gM.frozen_cave_, &r1, &r1);

  // Init Logic Cave map
  for(int i = 0; i < 16; ++i){
    for(int j = 0; j < 16; ++j){
      if(i == 0 || i == 15 || j == 0 || j == 15
        ||i == 1 || j == 14|| j == 1){ 
        gM.logic_board_[i][j].enabled_ = 0;
        gM.logic_board_[i][j].enter_ = 0;
        gM.units_board_[i][j].enabled_ = 255;        
      }else{
        gM.logic_board_[i][j].init();
        gM.units_board_[i][j].enabled_ = 255;
      }
      
    }
  }
  
  if(rand()%10 < 5){
    gM.logic_board_[13][12].enabled_ = 1;
    gM.logic_board_[13][12].enter_ = 1;
    ent_list.push_back(&cave_[0]);
  }else{
    gM.logic_board_[10][3].enabled_ = 1;
    gM.logic_board_[10][3].enter_ = 1;
    ent_list.push_back(&cave_[1]);
  }

  //Init  Enemies
  
  int n_e = rand()%5+2,x = 0, y = 0;
  
  for(int i= 0; i < n_e; ++i){
    gM.NPC_[i].initEnemy((rand()%5)+1, i+4);
    //Give initial pos
    do{
      x = rand()%16, y = rand()%3+2;
    }while(gM.logic_board_[y][x].enabled_ == 0
        || gM.units_board_[y][x].enabled_ != 255);
    gM.NPC_[i].dst_rect_.x = x;
    gM.NPC_[i].dst_rect_.y = y;
    gM.NPC_[i].dst_rect_.w = 40;
    gM.NPC_[i].dst_rect_.h = 40;
    gM.NPC_[i].generate_mov_ = 1;
    gM.units_board_[y][x].enabled_= i+4;
    
    ent_list.push_back(&gM.NPC_[i]);

  }
  
  for(int i= 0; i < 4; ++i){
    //Give initial pos
    
    do{
      x = rand()%16, y = rand()%3+11;
    }while(gM.logic_board_[y][x].enabled_ == 0
        || gM.units_board_[y][x].enabled_ != 255);
        
    gM.units_board_[y][x].enabled_= i;
    gM.player_[i].dst_rect_.x = x;
    gM.player_[i].dst_rect_.y = y;
    gM.player_[i].dst_rect_.w = 40;
    gM.player_[i].dst_rect_.h = 40;
    gM.player_[i].index_mov_ = 0;
    gM.player_[i].previous_movs_[gM.player_[i].index_mov_]= gM.player_[i].dst_rect_.y*16+ gM.player_[i].dst_rect_.x;
    gM.player_[i].current_.movement = gM.player_[i].base_.movement;
    ent_list.push_back(&gM.player_[i]);

  }
  num_turns_ = 0;
  total_turns_ = 4 + n_e;

  SDL_Color black = {0,0,0,255};
  
  gM.combat_.initCombat(gM.player_[0]);
  gM.combat_.current_char_ = &gM.player_[0];

  gM.ui_rects_[0].dst_rect_.x = gM.kBoardWidth;
  gM.ui_rects_[0].dst_rect_.y = 0;
  gM.ui_rects_[0].dst_rect_.w = gM.kWindowWidth - gM.kBoardWidth;
  gM.ui_rects_[0].dst_rect_.h = gM.kWindowHeight;
  
  gM.ui_rects_[1].dst_rect_.x = 0;
  gM.ui_rects_[1].dst_rect_.y = gM.kBoardHeight;
  gM.ui_rects_[1].dst_rect_.w = gM.kWindowWidth;
  gM.ui_rects_[1].dst_rect_.h = gM.kWindowHeight - gM.kBoardHeight;
  
  gM.ui_rects_[0].fill_color_ = black;
  gM.ui_rects_[1].fill_color_ = black;

  ent_list.push_back (&gM.ui_rects_[0]);
  ent_list.push_back (&gM.ui_rects_[1]);
  for(int i= 0; i<9; ++i){
    ent_list.push_back(&gM.combat_.stats_text_[i]);
  }
  for(int i = 0; i<6; ++i){
    ent_list.push_back(&gM.combat_.stats_rect_[i]);
  }
  for(int i = 0; i < 2; ++i){
    ent_list.push_back(&gM.combat_.stats_title_[i]);
  }

  // Combat Buttons
  SDL_Color button_color = {121, 121, 121, 60};
  SDL_Color text_color = {0, 0, 0, 255};
  char* font_path = "../data/fonts/BreathFire.ttf";
  int font_size = 25;
  
    // Attack
  combat_button_[0].dst_rect_ = {760, 495, 150, 50};
  combat_button_[0].fill_color_ = button_color;
  
    // End Turn
  combat_button_[1].dst_rect_ = {760, 565, 150, 50};
  combat_button_[1].fill_color_ = button_color;

  for(int i = 0; i < 3; ++i){
    ent_list.push_back(&combat_button_[i]);
  }

  button_color = {255, 255, 255, 60};
  SDL_Rect action_dst = {785, 500, 100, 40};
  actions_text_[0].init(font_path, 25, button_color,"Attack", action_dst);

  action_dst = {785, 570, 100, 40};
  actions_text_[1].init(font_path, 25, button_color,"End Turn", action_dst);

  for(int i = 0; i < 2; ++i){
    ent_list.push_back(&actions_text_[i]);
  }

}
/*
void CombatScene::quit(){
  
  ent_list.clear();
}*/


void CombatScene::input(SDL_Event* eve){

  GameManager& gM = GameManager::Instantiate();
  char aux_text[50];

  SDL_Point mouse_position = {eve->button.x, eve->button.y};
  SDL_Rect button_dst = {760, 495, 150, 50};
  SDL_Color text_color_red = {255,0,0,255};
  SDL_Color text_color_blue = {0,0,255,255};
  SDL_Color text_color_grey = {255,255,255,60};
  SDL_Color button_color = {121, 121, 121, 60};
  
  // Choose enemy to attack
  if(gM.player_[num_turns_].player_attacking_ == 2){

    if(eve->type == SDL_MOUSEBUTTONDOWN && eve->button.button == SDL_BUTTON_LEFT){

      SDL_Rect aux_tile;
      aux_tile.x = eve->button.x/40;
      aux_tile.y = eve->button.y/40;

      if(gM.units_board_[aux_tile.y][aux_tile.x].enabled_ != 255){
        if(gM.units_board_[aux_tile.y][aux_tile.x].enabled_ > 3){
          
          if(gM.NPC_[gM.units_board_[aux_tile.y][aux_tile.x].enabled_-4].mhDistanceV2(&gM.player_[num_turns_].dst_rect_)
             <= attack_range_){
               
              if(gM.player_[num_turns_].current_.mana >= gM.player_[num_turns_].char_attacks_[gM.player_[num_turns_].attack_chosen_].mana_cost){
              
                gM.NPC_[gM.units_board_[aux_tile.y][aux_tile.x].enabled_-4].takeDamage(&gM.player_[num_turns_], gM.player_[num_turns_].char_attacks_[gM.player_[num_turns_].attack_chosen_].type);
                
                gM.player_[num_turns_].turn_completed_ = 1;
                ++num_turns_;

                gM.combat_.initCombat(gM.player_[num_turns_]);
                gM.combat_.current_char_ = &gM.player_[num_turns_];
                if(num_turns_>=4)
                gM.combat_.current_char_ = &gM.NPC_[num_turns_-4];

                combat_button_[0].dst_rect_ = {760, 495, 150, 50};
                combat_button_[1].dst_rect_ = {760, 565, 150, 50};

                combat_button_[2].dst_rect_ = {7600, 495, 150, 50};
                combat_button_[3].dst_rect_ = {7600, 565, 150, 50};

                SDL_Rect tmp_dst = {10000, 10000, 0,0};
                actions_text_[2].setPosition(&tmp_dst);
                actions_text_[3].setPosition(&tmp_dst);

                tmp_dst = {785, 500, 100, 40};
                actions_text_[0].changeText("Attack");
                actions_text_[0].setPosition(&tmp_dst);

                tmp_dst = {785, 570, 100, 40};
                actions_text_[1].changeText("End Turn");
                actions_text_[1].setPosition(&tmp_dst);
                gM.player_[num_turns_].player_attacking_ = 0;

                attack_range_ = 0;

              }else{

                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Attack Missed", "Not enough mana", NULL);

              }
              
          }else{
            
            // printf("Enemy: %d %d %d %d %d\n", aux_tile.x, aux_tile.y, gM.NPC_[gM.units_board_[aux_tile.y][aux_tile.x].enabled_-4].mhDistance(&gM.player_[num_turns_].dst_rect_), gM.player_[num_turns_].char_attacks_[gM.player_[num_turns_].attack_chosen_].range, gM.player_[num_turns_].char_attacks_[gM.player_[num_turns_].attack_chosen_].type);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Attack Missed", "Enemy out of range", NULL);

          } 

        }else{

          // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Attack Missed", "Ally out of range", NULL);

        }
        
      }else{
        
        if(mouse_position.x <= 640){
          
          SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Attack Missed", "Wrong target", NULL);
        }

      }

    }

  }
  
  // Starter Menu (Attack - End Turn)
  if(gM.player_[num_turns_].player_attacking_ == 0){
    
    // Attack Button
    if(SDL_PointInRect(&mouse_position, &button_dst)){
      char* att_msg[16];
      att_msg[0] = "Kick\0";
      att_msg[1] = "Fuuton\0";
      att_msg[2] = "Katon\0";
      att_msg[3] = "Blizzard\0";
      att_msg[4] = "High Kick\0";
      att_msg[5] = "Close Combat\0";
      att_msg[6] = "Punch\0";
      att_msg[7] = "Mega Punch\0";
      att_msg[8] = "Drain\0";
      att_msg[9] = "Giga Drain\0";
      att_msg[10] = "New Life\0";
      att_msg[11] = "Ragnarok\0";
      att_msg[12] = "Aery\0";
      att_msg[13] = "Raikiri\0";
      att_msg[14] = "Hawkshot\0";
      att_msg[15] = "Crystal Arrow\0";
      actions_text_[0].changeColor(text_color_red);
      // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Prueba", "Hola", NULL);
      
      if(eve->type == SDL_MOUSEBUTTONDOWN &&
         eve->button.button == SDL_BUTTON_LEFT &&
         gM.player_[num_turns_].player_attacking_ == 0){
        
          char* font_path = "../data/fonts/BreathFire.ttf";
          gM.player_[num_turns_].player_attacking_ = 1;
          actions_text_[0].changeColor(text_color_grey);
          
          // Buttons
          combat_button_[0].dst_rect_ = {700, 495, 125, 50};
          combat_button_[1].dst_rect_ = {700, 565, 125, 50};
          combat_button_[2].dst_rect_ = {845, 495, 125, 50};
          combat_button_[3].dst_rect_ = {845, 565, 125, 50};

          // Text
          SDL_Rect tmp_rect = {705, 500, 120, 40};
          actions_text_[0].changeColor(text_color_grey);
        
        if(num_turns_<4){
          /*sprintf (aux_text, "Attack %d", 
          gM.player_[num_turns_].char_attacks_[0].id);*/   
          actions_text_[0].changeText(att_msg[gM.player_[num_turns_].char_attacks_[0].id]);
        }
        actions_text_[0].setPosition(&tmp_rect);

        tmp_rect = {705, 570, 120, 40};
        
        if(num_turns_<4){
          /*sprintf (aux_text, "Attack %d", 
          gM.player_[num_turns_].char_attacks_[1].id);   */
          actions_text_[1].changeText(att_msg[gM.player_[num_turns_].char_attacks_[1].id]);
        }
        
        actions_text_[1].setPosition(&tmp_rect);

        if(num_turns_<4){  
          /*    
          sprintf (aux_text, "Attack %d", 
          gM.player_[num_turns_].char_attacks_[2].id); */  
          
       
          actions_text_[2].init(font_path, 25, text_color_grey, 
                              att_msg[gM.player_[num_turns_].char_attacks_[2].id],
                              {850, 500, 120, 40});
        }                     
        actions_text_[3].init(font_path, 25, text_color_grey, "Return",
                              {860, 570, 100, 40});

        combat_button_[2].fill_color_ = button_color;
        combat_button_[3].fill_color_ = button_color;
        ent_list.push_back(&combat_button_[2]);
        ent_list.push_back(&combat_button_[3]);
        ent_list.push_back(&actions_text_[2]);
        ent_list.push_back(&actions_text_[3]);

      }
        
    }else{
      actions_text_[0].changeColor(text_color_grey);
    }

    // End Turn
    button_dst = {785, 570, 100, 40};
    if(SDL_PointInRect(&mouse_position, &button_dst)){

      actions_text_[1].changeColor(text_color_red);
      
      if(eve->type == SDL_MOUSEBUTTONDOWN &&
        eve->button.button == SDL_BUTTON_LEFT){

        gM.player_[num_turns_].turn_completed_ = 1;
        ++num_turns_;
        if(num_turns_<4){
          gM.combat_.initCombat(gM.player_[num_turns_]);
          gM.combat_.current_char_ = &gM.player_[num_turns_];
        }
        else{
           gM.combat_.initCombat(gM.NPC_[num_turns_-4]);
          gM.combat_.current_char_ = &gM.NPC_[num_turns_-4];
          printf("Current NPC: %d\n", gM.combat_.current_char_->char_id_);
        }

        gM.player_[num_turns_].player_attacking_ = 0;
        attack_range_ = 0;

      }
        
    }else{
      actions_text_[1].changeColor(text_color_grey);
    }

  }else if(gM.player_[num_turns_].player_attacking_ == 1){
    
    // Attack Buttons
    SDL_Rect tmp_button[3];
    tmp_button[0] = {700, 495, 125, 50};
    tmp_button[1] = {700, 565, 125, 50};
    tmp_button[2] = {845, 495, 125, 50};
 
    for(int i = 0; i < 3; ++i){
      if(SDL_PointInRect(&mouse_position, &tmp_button[i])){
        
        if(gM.player_[num_turns_].char_attacks_[i].type == 1){
          actions_text_[i].changeColor(text_color_red);
        }else{
          actions_text_[i].changeColor(text_color_blue);
        }

        // Attack
        if(eve->type == SDL_MOUSEBUTTONDOWN &&
           eve->button.button == SDL_BUTTON_LEFT &&
           gM.player_[num_turns_].current_.mana >= gM.player_[num_turns_].char_attacks_[i].mana_cost){
            
            gM.player_[num_turns_].player_attacking_ = 2;
            
            combat_button_[0].dst_rect_ = {700, 495, 270, 50};
            combat_button_[1].dst_rect_ = {760, 565, 150, 50};

            combat_button_[2].dst_rect_ = {7600, 495, 150, 50};
            combat_button_[3].dst_rect_ = {7600, 565, 150, 50};

            SDL_Rect tmp_dst = {10000, 10000, 0,0};
            actions_text_[1].setPosition(&tmp_dst);
            actions_text_[2].setPosition(&tmp_dst);
            actions_text_[3].setPosition(&tmp_dst);

            tmp_dst = {735, 500, 200, 40};
            actions_text_[0].changeText("Choose target...");
            actions_text_[0].setPosition(&tmp_dst);

            tmp_dst = {785, 570, 100, 40};
            actions_text_[1].changeText("Return");
            actions_text_[1].setPosition(&tmp_dst);
                        
            // gM.player_[num_turns_].player_attacking_ = 1;
            gM.player_[num_turns_].attack_chosen_ = gM.player_[num_turns_].char_attacks_[i].id;
            printf("Attack: %d\n", gM.player_[num_turns_].attack_chosen_);
            // printf("Ataque: %d %d\n", gM.player_[num_turns_].attack_chosen_, gM.player_[num_turns_].char_attacks_[i].id);

            actions_text_[i].changeColor(text_color_grey);
        }

      }else{
        
        actions_text_[i].changeColor(text_color_grey);

      }

    }

    if(SDL_PointInRect(&mouse_position, &tmp_button[0])){

      attack_range_ = gM.player_[num_turns_].char_attacks_[0].range;

    }else if(SDL_PointInRect(&mouse_position, &tmp_button[1])){

      attack_range_ = gM.player_[num_turns_].char_attacks_[1].range;

    }else if(SDL_PointInRect(&mouse_position, &tmp_button[2])){

      attack_range_ = gM.player_[num_turns_].char_attacks_[2].range;

    }else{
      attack_range_ = 0;
    }

    // Return
    button_dst = {845, 565, 125, 50};
    if(SDL_PointInRect(&mouse_position, &button_dst)){
    
      actions_text_[3].changeColor(text_color_red);

      if(eve->type == SDL_MOUSEBUTTONDOWN &&
         eve->button.button == SDL_BUTTON_LEFT){

          combat_button_[0].dst_rect_ = {760, 495, 150, 50};
          combat_button_[1].dst_rect_ = {760, 565, 150, 50};

          combat_button_[2].dst_rect_ = {7600, 495, 150, 50};
          combat_button_[3].dst_rect_ = {7600, 565, 150, 50};

          SDL_Rect tmp_dst = {10000, 10000, 0,0};
          actions_text_[2].setPosition(&tmp_dst);
          actions_text_[3].setPosition(&tmp_dst);

          tmp_dst = {785, 500, 100, 40};
          actions_text_[0].changeText("Attack");
          actions_text_[0].setPosition(&tmp_dst);

          tmp_dst = {785, 570, 100, 40};
          actions_text_[1].changeText("End Turn");
          actions_text_[1].setPosition(&tmp_dst);
          gM.player_[num_turns_].player_attacking_ = 0;
         
      }

    }else{

      actions_text_[3].changeColor(text_color_grey);

    }

  }else if(gM.player_[num_turns_].player_attacking_ == 2){
    
    // Return
    button_dst = {760, 565, 150, 50};
    if(SDL_PointInRect(&mouse_position, &button_dst)){
    
      actions_text_[1].changeColor(text_color_red);

      if(eve->type == SDL_MOUSEBUTTONDOWN &&
         eve->button.button == SDL_BUTTON_LEFT){

          combat_button_[0].dst_rect_ = {760, 495, 150, 50};
          combat_button_[1].dst_rect_ = {760, 565, 150, 50};

          combat_button_[2].dst_rect_ = {7600, 495, 150, 50};
          combat_button_[3].dst_rect_ = {7600, 565, 150, 50};

          SDL_Rect tmp_dst = {10000, 10000, 0,0};
          actions_text_[2].setPosition(&tmp_dst);
          actions_text_[3].setPosition(&tmp_dst);

          tmp_dst = {785, 500, 100, 40};
          actions_text_[0].changeText("Attack");
          actions_text_[0].setPosition(&tmp_dst);

          tmp_dst = {785, 570, 100, 40};
          actions_text_[1].changeText("End Turn");
          actions_text_[1].setPosition(&tmp_dst);
          gM.player_[num_turns_].player_attacking_ = 0;

          attack_range_ = 0;
         
      }

    }else{

      actions_text_[1].changeColor(text_color_grey);

    }

  }
  
  if(eve->type == SDL_KEYDOWN){ 
   
  } 
  if(eve->key.keysym.sym == SDLK_p){
    ++num_turns_;
    gM.combat_.initCombat(gM.player_[num_turns_]);
    gM.combat_.current_char_ = &gM.player_[num_turns_];
    if(num_turns_>=4){
      gM.combat_.current_char_ = &gM.NPC_[num_turns_-4];
    }
  }
  if(eve->key.keysym.sym == SDLK_l){
    
    gM.NPC_[0].endTile();
    gM.NPC_[0].current_.movement = 5 ;
    
  }
  if(num_turns_<4){
    gM.player_[num_turns_].movCharacterCombat(eve);
  }

  

  for(int i = 0; i < total_turns_-4; ++i){
    // printf("Vida NPC[%d]: %d\n", i, gM.NPC_[i].current_.hp);
  }
  
}


void CombatScene::update(){
  GameManager& gM = GameManager::Instantiate();
  gM.combat_.updateStats();
  
  if(num_turns_<4){ 
    gM.player_[num_turns_].updateSpriteC();
    if(gM.player_[num_turns_].current_.hp <= 0){ ++num_turns_; }
  }
  else{
    int tmp_attack = 0;
    if(gM.NPC_[num_turns_-4].current_.hp > 0){
      gM.NPC_[num_turns_-4].iaBehaviour();
      gM.NPC_[num_turns_-4].updateSpriteC();

      do{
       tmp_attack = rand()%3;
      }while(gM.NPC_[num_turns_-4].char_attacks_[tmp_attack].mana_cost > gM.NPC_[num_turns_-4].current_.mana);
      
      for(int i = 0; i < num_turns_; ++i){
        if(gM.NPC_[num_turns_-4].mhDistanceV2(&gM.player_[i].dst_rect_) <= gM.NPC_[num_turns_-4].char_attacks_[tmp_attack].range){
          
          gM.player_[i].takeDamage(&gM.NPC_[num_turns_-4], gM.NPC_[num_turns_-4].char_attacks_[tmp_attack].type);
          printf("%d %d\n", gM.player_[i].char_id_, gM.player_[i].current_.hp);
          gM.NPC_[num_turns_-4].turn_completed_ = 1;
        }
      }

      if(gM.NPC_[num_turns_-4].turn_completed_){
        ++num_turns_;
      }
    }else{
      ++num_turns_;
    }
  }
  
  if(num_turns_<4 && gM.player_[num_turns_].xp_>100){
    gM.player_[num_turns_].levelUp();
  }

  if(num_turns_ >= total_turns_){
    num_turns_ = 0;
    gM.player_[0].reset();
    gM.player_[1].reset();
    gM.player_[2].reset();
    gM.player_[3].reset();
    for(int i= 0; i< total_turns_-4; ++i) {
      gM.NPC_[i].reset();
    }      
  }
  
}

void CombatScene::drawImgui(SDL_Renderer* ren){
  
  // Character Stats Frame
  SDL_Rect stats_frame = {685, 60, 300, 370};

  SDL_SetRenderDrawColor(ren, 255,255,0,255);
  SDL_RenderDrawRect(ren, &stats_frame);

  // Actions Frame
  SDL_Rect actions_frame = {685, 480, 300, 150};

  SDL_SetRenderDrawColor(ren, 255,255,0,255);
  SDL_RenderDrawRect(ren, &actions_frame);

  GameManager& gM = GameManager::Instantiate();
  SDL_Rect mark_rect;
  
  if(num_turns_ < 4){
    mark_rect.x = gM.player_[num_turns_].dst_rect_.x * 40;
    mark_rect.y = gM.player_[num_turns_].dst_rect_.y * 40;
    mark_rect.w = 40;
    mark_rect.h = 40;
  }else{
    mark_rect.x = gM.NPC_[num_turns_-4].dst_rect_.x * 40;
    mark_rect.y = gM.NPC_[num_turns_-4].dst_rect_.y * 40;
    mark_rect.w = 40;
    mark_rect.h = 40;
  }
  
  SDL_SetRenderDrawColor(ren, 0,255,0,255);
  SDL_RenderDrawRect(ren, &mark_rect);

  SDL_Rect range_rect[50];
  if(attack_range_ > 0 && num_turns_ < 4){

    for(int i = -attack_range_; i <= attack_range_; ++i){
      for(int j = -attack_range_; j <= attack_range_; ++j){
        
        if(i != 0 || j != 0){
          range_rect[num_range_rect_].x = (gM.player_[num_turns_].dst_rect_.x * 40) + (40 * j);
          range_rect[num_range_rect_].y = (gM.player_[num_turns_].dst_rect_.y * 40) + (40 * i);
          range_rect[num_range_rect_].w = 40;
          range_rect[num_range_rect_].h = 40;

          if(range_rect[num_range_rect_].x >= 0 && range_rect[num_range_rect_].x <= 984 &&
             range_rect[num_range_rect_].y >= 0 && range_rect[num_range_rect_].y <= 600){
          
            SDL_SetRenderDrawColor(ren, 0,255,0,255);
            SDL_RenderDrawRect(ren, &range_rect[num_range_rect_]);

            ++num_range_rect_;

            if(num_range_rect_ >= attack_range_*8){ num_range_rect_ = 0; }

          }
        }

      }
    }

    num_range_rect_ = 0;

  }

}