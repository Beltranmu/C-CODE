#include "character.h"
#include "gamemanager.h"
#include "rbmmath.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

Character::Character(){
  base_.hp = 100;
  base_.mana = 100;
  base_.physical_att = 10;
  base_.magic_att = 10;
  base_.armor = 10;
  base_.magic_resist = 10;
  base_.movement = 7; 
  
  name_[0]='\0';
  name2_[0]='\0';
  
  current_ = base_;
  profession_ = kEnumProfession_Warrior;
  xp_ = 0;
  level_ = 1;
  char_id_ = 0;
  
  end_tile_mov_.x = 0;
  end_tile_mov_.y = 0;
  cont_mov_ = 0;
  generate_mov_ = 1;
  
}

Character::~Character(){
  
}

void Character::cpyCharacter(Character& c){
  
  profession_ = c.profession_;
  base_ = c.base_;
  current_ = c.current_;
  xp_ = c.xp_;
  level_ = c.level_;
  char_id_ = c.char_id_;
  
  for(int i = 0; i<7; ++i){
    skin_[i] = c.skin_[i];
  }
  
  for(int i = 0; i<10; ++i){
    outfit_[i] = c.outfit_[i];
  }
  
}

void Character::init(unsigned char id){
  
  GameManager& gM = GameManager::Instantiate(); 
  profession_ = 0;
  
  
  switch(profession_){
    
    case 0:
      char_attacks_[0] = *gM.data_base_.attacks_[5];
      char_attacks_[1] = *gM.data_base_.attacks_[0];
      char_attacks_[2] = *gM.data_base_.attacks_[7];
    break;
    
    case 1:
      char_attacks_[0] = *gM.data_base_.attacks_[14];
      char_attacks_[1] = *gM.data_base_.attacks_[0];
      char_attacks_[2] = *gM.data_base_.attacks_[15];
    break;
    
    case 2:
      char_attacks_[0] = *gM.data_base_.attacks_[8];
      char_attacks_[1] = *gM.data_base_.attacks_[3];
      char_attacks_[2] = *gM.data_base_.attacks_[12];
    break;
    
    case 3:
      char_attacks_[0] = *gM.data_base_.attacks_[11];
      char_attacks_[1] = *gM.data_base_.attacks_[4];
      char_attacks_[2] = *gM.data_base_.attacks_[5];
    break;
    
    case 4:
      char_attacks_[0] = *gM.data_base_.attacks_[11];
      char_attacks_[1] = *gM.data_base_.attacks_[13];
      char_attacks_[2] = *gM.data_base_.attacks_[4];
    break;
    
    case 5:
      char_attacks_[0] = *gM.data_base_.attacks_[13];
      char_attacks_[1] = *gM.data_base_.attacks_[11];
      char_attacks_[2] = *gM.data_base_.attacks_[7];
    break;
    
    case 6:
      char_attacks_[0] = *gM.data_base_.attacks_[8];
      char_attacks_[1] = *gM.data_base_.attacks_[9];
      char_attacks_[2] = *gM.data_base_.attacks_[10];
    break;
    
    case 7:
      char_attacks_[0]= *gM.data_base_.attacks_[6];
      char_attacks_[1]= *gM.data_base_.attacks_[0];
      char_attacks_[2]= *gM.data_base_.attacks_[7];
    break;
    
    case 8:
      char_attacks_[0]= *gM.data_base_.attacks_[13];
      char_attacks_[1]= *gM.data_base_.attacks_[1];
      char_attacks_[2]= *gM.data_base_.attacks_[5];
    break;
    
    case 9:
      char_attacks_[0]= *gM.data_base_.attacks_[1];
      char_attacks_[1]= *gM.data_base_.attacks_[2];
      char_attacks_[2]= *gM.data_base_.attacks_[3];
    break;
    /*
    case 10:
      char_attacks_[0]= gM.data_base_.attacks_[1];
      char_attacks_[1]= gM.data_base_.attacks_[4];
      char_attacks_[2]= gM.data_base_.attacks_[11];
    break;*/
  
  }
 
  base_ = *(gM.data_base_.p_[profession_]);
  current_ = base_;

  xp_ = 0;
  level_ = 1;
  
 
  int32_t* skin_ptr = (int32_t*)&skin_id_;
  
  for(int i = 2; i < 23; ++i){

    *(skin_ptr+i) = 0;

  }
  
  skin_id_.gender = 1;
  skin_id_.skin = 1;
  skin_id_.hair_color = 1;
  for(int i=0; i<11; ++i){
    previous_movs_[i]=-1;
  }
  
  index_mov_=0;
  char_id_ = id;
  turn_completed_ = 0;
  
  end_tile_mov_.x = 0;
  end_tile_mov_.y = 0;

  player_attacking_ = 0;
  attack_chosen_ = 0;
}

void Character::init(int prof, unsigned char id){
  
  GameManager& gM = GameManager::Instantiate(); 
  profession_ = prof;
  
  
  switch(profession_){
    
    case 0:
      char_attacks_[0] = *gM.data_base_.attacks_[5];
      char_attacks_[1] = *gM.data_base_.attacks_[0];
      char_attacks_[2] = *gM.data_base_.attacks_[7];
    break;
    
    case 1:
      char_attacks_[0] = *gM.data_base_.attacks_[14];
      char_attacks_[1] = *gM.data_base_.attacks_[0];
      char_attacks_[2] = *gM.data_base_.attacks_[15];
    break;
    
    case 2:
      char_attacks_[0] = *gM.data_base_.attacks_[8];
      char_attacks_[1] = *gM.data_base_.attacks_[3];
      char_attacks_[2] = *gM.data_base_.attacks_[12];
    break;
    
    case 3:
      char_attacks_[0] = *gM.data_base_.attacks_[11];
      char_attacks_[1] = *gM.data_base_.attacks_[4];
      char_attacks_[2] = *gM.data_base_.attacks_[5];
    break;
    
    case 4:
      char_attacks_[0] = *gM.data_base_.attacks_[11];
      char_attacks_[1] = *gM.data_base_.attacks_[13];
      char_attacks_[2] = *gM.data_base_.attacks_[4];
    break;
    
    case 5:
      char_attacks_[0] = *gM.data_base_.attacks_[13];
      char_attacks_[1] = *gM.data_base_.attacks_[11];
      char_attacks_[2] = *gM.data_base_.attacks_[7];
    break;
    
    case 6:
      char_attacks_[0] = *gM.data_base_.attacks_[8];
      char_attacks_[1] = *gM.data_base_.attacks_[9];
      char_attacks_[2] = *gM.data_base_.attacks_[10];
    break;
    
    case 7:
      char_attacks_[0]= *gM.data_base_.attacks_[6];
      char_attacks_[1]= *gM.data_base_.attacks_[0];
      char_attacks_[2]= *gM.data_base_.attacks_[7];
    break;
    
    case 8:
      char_attacks_[0]= *gM.data_base_.attacks_[13];
      char_attacks_[1]= *gM.data_base_.attacks_[1];
      char_attacks_[2]= *gM.data_base_.attacks_[5];
    break;
    
    case 9:
      char_attacks_[0]= *gM.data_base_.attacks_[1];
      char_attacks_[1]= *gM.data_base_.attacks_[2];
      char_attacks_[2]= *gM.data_base_.attacks_[3];
    break;
    /*
    case 10:
      char_attacks_[0]= gM.data_base_.attacks_[1];
      char_attacks_[1]= gM.data_base_.attacks_[4];
      char_attacks_[2]= gM.data_base_.attacks_[11];
    break;*/
  
  }
 
  base_ = *(gM.data_base_.p_[profession_]);
  current_ = base_;

  xp_ = 0;
  level_ = 1;
  
  /*
  int32_t* skin_ptr = (int32_t*)&skin_id_;
  
  for(int i = 2; i < 23; ++i){

    *(skin_ptr+i) = 0;

  }
  
  skin_id_.gender = 1;
  skin_id_.skin = 1;
  skin_id_.hair_color = 1;*/
  for(int i=0; i<11; ++i){
    previous_movs_[i]=-1;
  }
  
  index_mov_=0;
  char_id_ = id;
  turn_completed_ = 0;
  
  end_tile_mov_.x = 0;
  end_tile_mov_.y = 0;

  player_attacking_ = 0;
  attack_chosen_ = 0;
}

void Character::initEnemy(const int lvl, const int id){

  GameManager& gM = GameManager::Instantiate();
  SDL_Rect tmp_rect = {0, 128, 64, 64};

  profession_ = kEnumProfession_Monster;
  level_ = lvl;
 
  base_ = *(gM.data_base_.p_[profession_]);
 // current_ = base_ * level_;

  char_id_ = (unsigned char)id;

  index_mov_ = 0;

  turn_completed_ = 0;

  skin_id_.gender = 1 + (rand()%2);
  if(skin_id_.gender == 1){
    skin_id_.skin = 10 + (rand()%3);
  }else{
    skin_id_.skin = 11 + (rand()%2);
  }

  // Body Path
  std::string tmp_dir = "../data/skins/body/male/body$.png";

  // Set Gender
  if(skin_id_.gender == 2){

    tmp_dir.replace(tmp_dir.find("male"), 4, "female");

  }

  if(skin_id_.gender == 1){

    switch(skin_id_.skin){

      case 10: skin_[0].initSprite(*gM.enemy_textures_[0], &tmp_rect, &tmp_rect); break;
      case 11: skin_[1].initSprite(*gM.enemy_textures_[1], &tmp_rect, &tmp_rect); break;
      case 12: skin_[2].initSprite(*gM.enemy_textures_[2], &tmp_rect, &tmp_rect); break;      

    }

  }else{

    switch(skin_id_.skin){

      case 11: skin_[3].initSprite(*gM.enemy_textures_[3], &tmp_rect, &tmp_rect); break;
      case 12: skin_[4].initSprite(*gM.enemy_textures_[4], &tmp_rect, &tmp_rect); break;

    }

  }
  char_attacks_[0]= *gM.data_base_.attacks_[1];
  char_attacks_[1]= *gM.data_base_.attacks_[4];
  char_attacks_[2]= *gM.data_base_.attacks_[11];
  
  end_tile_mov_.x = 0;
  end_tile_mov_.y = 0;
  cont_mov_ = 0;
  generate_mov_ = 1;
  current_.movement = 5;
  player_attacking_ = 0;
  attack_chosen_ = 0;

  xp_ = level_ * 1+rand()%20;
}




void Character::updateSpriteC(){ 
  SDL_Rect  snip_r = {0,0,64,64};//UP
  switch(direction_){
    case 1:         
     snip_r.y = 128; //DOWN
    break;
    
    case 2:
      snip_r.y = 64; //LEFT
    break;
    
    case 3:
      snip_r.y = 192; //RIGHT
    break;
  }

  for (int i = 0; i<7; ++i){             
    
    skin_[i].snip_rect_ = snip_r; 
  }
  for (int i = 0; i<10; ++i){ 

    outfit_[i].snip_rect_ = snip_r; 
  } 
}
int32_t Character::mhDistance(const SDL_Rect* tr_rect){
  
  int x = dst_rect_.x - tr_rect->x;
  int y = dst_rect_.y - tr_rect->y;
  
  if(x<0){ x *=-1;}
  if(y<0){ y *=-1;}
  
  return x+y;
  
}

int32_t Character::mhDistanceV2(const SDL_Rect* tr_rect){
  
  int x = dst_rect_.x - tr_rect->x;
  int y = dst_rect_.y - tr_rect->y;
  
  if(x<0){ x *=-1;}
  if(y<0){ y *=-1;}
  
  if(x>=y){
    return x;
  }
  return y;
}






void Character::endTile(){
  GameManager& gM = GameManager::Instantiate();
  int end_x = 0, end_y = 0;
  int aux_x = 0, aux_y = 0;
  uint8_t find = 0;
  SDL_Rect aux_rect = gM.player_[0].dst_rect_;
  int d = this->mhDistance(&aux_rect), aux_d=0;
  printf("\n\n\nD1:%d\n",d);
 
  aux_rect = dst_rect_;
  printf("OP:(%d,%d)", aux_rect.x,aux_rect.y);
  for(int r = 0; r<=base_.movement && find == 0 ; ++r){
    for(int i = 0; i<r && find == 0; ++i){
      int j = r-i;
      printf("\ni:%d,j:%d",i,j);
      //Down-Right
      aux_x = dst_rect_.x+i;
      aux_y = dst_rect_.y+j;
      
      if(0 <= aux_x && aux_x < 16 &&
         0 <= aux_y && aux_y < 16){
           
        if(gM.logic_board_[aux_y][aux_x].enabled_ == 1){
          aux_rect.x = aux_x;
          aux_rect.y = aux_y;
          printf("\nDR:(%d,%d)\n", aux_x,aux_y);       
          for(int c = 0; c<4 && find == 0; ++c){
            aux_d = gM.player_[c].mhDistance(&aux_rect);
            printf("distancia personaje %d: %d\n",c+1,aux_d);
            if(d>gM.player_[c].mhDistance(&aux_rect)){
              d=aux_d;
              end_x = aux_x;
              end_y = aux_y;
              if(d==0){
                find=1;
              }
            }
          }
        }
      } 
      
      //Down_left
      aux_x = dst_rect_.x-i;
      aux_y = dst_rect_.y+j;
      
      if(0 <= aux_x && aux_x < 16 &&
         0 <= aux_y && aux_y < 16){
           
        if(gM.logic_board_[aux_y][aux_x].enabled_ == 1){
          aux_rect.x = aux_x;
          aux_rect.y = aux_y;
          printf("\nDL:(%d,%d)\n", aux_x,aux_y);
          for(int c = 0; c<4 && find == 0; ++c){
            aux_d = gM.player_[c].mhDistance(&aux_rect);
            printf("distancia personaje %d: %d\n",c+1,aux_d);
            if(d>gM.player_[c].mhDistance(&aux_rect)){
              d=aux_d;
              end_x = aux_x;
              end_y = aux_y;
              if(d==0){
                find=1;
              }
            }
          }
        }
      }
      //Up-Right
      aux_x = dst_rect_.x+i;
      aux_y = dst_rect_.y-j;
      
      if(0 <= aux_x && aux_x < 16 &&
         0 <= aux_y && aux_y < 16){
           
        if(gM.logic_board_[aux_y][aux_x].enabled_ == 1){
          aux_rect.x = aux_x;
          aux_rect.y = aux_y;
          printf("\nUR:(%d,%d)\n", aux_x,aux_y);
          for(int c = 0; c<4 && find == 0; ++c){
            if(d>gM.player_[c].mhDistance(&aux_rect)){
              aux_d = gM.player_[c].mhDistance(&aux_rect);
              printf("distancia personaje %d: %d\n",c+1,aux_d);
              d=aux_d;
              end_x = aux_x;
              end_y = aux_y;
              if(d==0){
                find=1;
              }
            }
          }
        }
      }
      //Up-Left
      aux_x = dst_rect_.x-i;
      aux_y = dst_rect_.y-j;
      
      if(0 <= aux_x && aux_x < 16 &&
         0 <= aux_y && aux_y < 16){
           
        if(gM.logic_board_[aux_y][aux_x].enabled_ == 1){
          aux_rect.x = aux_x;
          aux_rect.y = aux_y;
          printf("\nUL:(%d,%d)\n", aux_x,aux_y);
          for(int c = 0; c<4 && find == 0; ++c){
            aux_d = gM.player_[c].mhDistance(&aux_rect);
            printf("distancia personaje %d: %d\n",c+1,aux_d);
            if(d>gM.player_[c].mhDistance(&aux_rect)){
              d=aux_d;
              end_x = aux_x;
              end_y = aux_y;
              if(d==0){
                find=1;
              }
            }
          }
        }
      }      
    }
  }
  printf("%d,%d", end_x, end_y);
  
  end_tile_mov_.x = end_x - dst_rect_.x;
  //if(end_tile_mov_.x<0){end_tile_mov_.x*=-1;}
  end_tile_mov_.y = end_y - dst_rect_.y;
  //if(end_tile_mov_.y<0){end_tile_mov_.y*=-1;}
  printf("ME quedan: x:%d, y:%d", end_tile_mov_.x, end_tile_mov_.y);
  
}

void Character::iaMov(){
  
  unsigned char next_up = RBM::GetMatrixPosition((unsigned char)dst_rect_.y,-1);
  unsigned char next_down = RBM::GetMatrixPosition((unsigned char)dst_rect_.y,1);
  unsigned char next_right = RBM::GetMatrixPosition((unsigned char)dst_rect_.x,1);
  unsigned char next_left = RBM::GetMatrixPosition((unsigned char)dst_rect_.x,-1);
  GameManager& gM = GameManager::Instantiate();
  uint8_t mov = 0;
  
  if(cont_mov_>=60){
    
     if(end_tile_mov_.y < 0 && mov == 0 &&
        CheckBeforeMove(dst_rect_.x, next_up)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.y = next_up;
      //previous_movs_[index_mov_] = next_up * 16 + dst_rect_.x;
      direction_ = 0;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      printf("ARIBBA\n");
      mov = 1;
    }
    
    if(end_tile_mov_.y > 0 && mov == 0 &&
       CheckBeforeMove(dst_rect_.x, next_down)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.y = next_down;
      //previous_movs_[index_mov_] = next_down * 16 + dst_rect_.x; 
      direction_ = 1;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      printf("ABAJO\n");
      mov = 1;
      // printf("%d\n",previous_movs_[index_mov_]);
    }
    
    if(end_tile_mov_.x >0 && mov == 0 &&
       CheckBeforeMove(next_right, dst_rect_.y)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.x = next_right;
     // previous_movs_[index_mov_] = dst_rect_.y * 16 + next_right; 
      direction_ = 3;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      printf("DERECHA\n");
      mov = 1;
      // printf("%d\n",previous_movs_[index_mov_]);
    }
    
    if(end_tile_mov_.x < 0 && mov == 0 &&
    CheckBeforeMove(next_left, dst_rect_.y)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.x = next_left;
     // previous_movs_[index_mov_] = dst_rect_.y * 16 + next_left;
      direction_ = 2;    
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;  
      printf("IZQUIERDA\n");
      mov = 1;    
      // printf("%d\n",previous_movs_[index_mov_]);
    } 
    
    cont_mov_= 0;
    printf("ME quedan: x:%d, y:%d", end_tile_mov_.x, end_tile_mov_.y);
  }
  
  ++cont_mov_;
  
  if(end_tile_mov_.x == 0 && end_tile_mov_.y == 0){
    cont_mov_ = 0;
    end_tile_mov_.x = 0 ;
    end_tile_mov_.y = 0;
    turn_completed_ = 1;
  }
}

void Character::iaBehaviour(){
  
  if(enabled_){
    if(generate_mov_ == 1){
      endTile();
      generate_mov_ = 0;
    }
    else{
      iaMov();
    }
  }
  else{
    cont_mov_ = 0;
    end_tile_mov_.x = 0 ;
    end_tile_mov_.y = 0;
    turn_completed_ = 1;
  }
}

void Character::reset(){
  current_.movement = base_.movement;
  current_.mana += base_.mana_regen;
  
  if(current_.mana > base_.mana){
      current_.mana = base_.mana;
  }
  
  generate_mov_ = 1;
  turn_completed_ = 0;
  previous_movs_[0] = dst_rect_.x +  dst_rect_.y * 16;
  index_mov_ = 0;
}


void Character::levelUp(){
  
  xp_ %=100;
  ++level_;
  
  base_.hp +=10;
  base_.mana +=10;
  base_.physical_att +=5;
  base_.magic_att +=5;
  base_.armor +=4;
  base_.magic_resist +=4;
  base_.mana_regen +=5;
  base_.crit_chance +=1;
  
  current_ = base_;
}


bool Character::CheckPreviousMovs(const int next_pos_x, 
                                  const int next_pos_y){
  
  int i = index_mov_-1;
  bool end = false;
  int aux_mov_cost = 0;
 
  while(i>=0 && !end){
    //aux_mov_cost += board[*(chara->previous_movs + i)].movement_cost;
    aux_mov_cost += 1;
    if((previous_movs_[i]) == next_pos_y * 16+ next_pos_x){
      current_.movement += aux_mov_cost;
      index_mov_ = i;
      end = true;
    }
    --i;
  }
  return end;
}

/**
  * @brief Tells if you can move to a position
  * @param next_pos_x x coordinate of the next tile 
  * @param next_pos_y y coordinate of the next tile 
  * @return true if you can move, otherwise false.
  */

bool Character::CheckBeforeMove(const int next_pos_x, 
                                const int next_pos_y){
  
  GameManager& gM = GameManager::Instantiate();
  int m_cost = 1;
  
  if(end_tile_mov_.y>0){ --end_tile_mov_.y;}
  if(end_tile_mov_.y<0){ ++end_tile_mov_.y;}
  if(end_tile_mov_.x>0){ --end_tile_mov_.x;}
  if(end_tile_mov_.x<0){ ++end_tile_mov_.x;}
  
  //const int m_cost = board[next_pos].movement_cost;
  if(gM.logic_board_[next_pos_y][next_pos_x].enabled_ == 1 &&
     gM.units_board_[next_pos_y][next_pos_x].enabled_ == 255){
    m_cost = 1;
  }else{
    m_cost = 1234567890;
  }
  
  //printf("coste : %d\n", m_cost);
  //printf("movs_res : %d\n", chara->n_movement);
  if(CheckPreviousMovs(next_pos_x, next_pos_y)){
    return true;
  }
  
  
  if(m_cost <= current_.movement){
    index_mov_++;
    current_.movement -=m_cost;
    return true;
  }
  return false;

}

void Character::takeDamage(Character* c, const uint8_t range){

  float dmg_multiplier = 0;

  // Melé
  if(current_.hp > 0){
    if(c->char_attacks_[attack_chosen_].type == 1){
      
      current_.hp -= (c->current_.physical_att/2 + c->char_attacks_[attack_chosen_].dmg/2);
      printf("Mele: %d\n", current_.hp);
      printf("Mana: %d\n", c->current_.mana);
      c->current_.mana -= c->char_attacks_[attack_chosen_].mana_cost;
      printf("Mana: %d\n", c->current_.mana);

    }else{  // Spell

      current_.hp -= (c->current_.physical_att/2 + c->char_attacks_[attack_chosen_].dmg/2);
      printf("Spell: %d\n", current_.hp);
      printf("Mana: %d\n", c->current_.mana);
      c->current_.mana -= c->char_attacks_[attack_chosen_].mana_cost;
      printf("Mana: %d\n", c->current_.mana);
    }
  }
  
  if(current_.hp <= 0){
    enabled_ = 0;
    GameManager::Instantiate().units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
    // Liberar Unit Board
    c->xp_ += xp_;
    printf("XP: %d\n", c->xp_);
  }

}

void Character::movCharacterCombat(SDL_Event* e){
  
  int next_up = RBM::GetMatrixPosition(dst_rect_.y,-1);
  int next_down = RBM::GetMatrixPosition(dst_rect_.y,1);
  int next_right = RBM::GetMatrixPosition(dst_rect_.x,1);
  int next_left = RBM::GetMatrixPosition(dst_rect_.x,-1);
  GameManager& gM = GameManager::Instantiate();
  
  if(e->type == SDL_KEYDOWN){
   

    // printf("pulso: %d,%d\n",dst_rect_.x, dst_rect_.y);
    // printf("pulso: %d,%d,%d,%d\n\n",next_up,next_down,next_right,next_right);
    if(e->key.keysym.sym == SDLK_UP &&
    CheckBeforeMove(dst_rect_.x, next_up)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.y = next_up;
      previous_movs_[index_mov_] = next_up * 16 + dst_rect_.x;
      direction_ = 0;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      // printf("%d\n",previous_movs_[index_mov_]);
    }
    
    if(e->key.keysym.sym == SDLK_DOWN &&
    CheckBeforeMove(dst_rect_.x, next_down)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.y = next_down;
      previous_movs_[index_mov_] = next_down * 16 + dst_rect_.x; 
      direction_ = 1;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      // printf("%d\n",previous_movs_[index_mov_]);
    }
    
    if(e->key.keysym.sym == SDLK_RIGHT &&
    CheckBeforeMove(next_right, dst_rect_.y)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.x = next_right;
      previous_movs_[index_mov_] = dst_rect_.y * 16 + next_right; 
      direction_ = 3;
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;
      // printf("%d\n",previous_movs_[index_mov_]);
    }
    
    if(e->key.keysym.sym == SDLK_LEFT &&
    CheckBeforeMove(next_left, dst_rect_.y)){
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = 255;
      dst_rect_.x = next_left;
      previous_movs_[index_mov_] = dst_rect_.y * 16 + next_left;
      direction_ = 2;    
      gM.units_board_[dst_rect_.y][dst_rect_.x].enabled_ = char_id_;      
      // printf("%d\n",previous_movs_[index_mov_]);
    } 
  }
}


void Character::movCharacter(SDL_Event* e){
  //Input i compruebo si me puedo mover
  uint8_t can_move = 0;
  int8_t vertical_mov = 0;
  int8_t horizontal_mov = 0;
  
  // siempre me voy a mover de celda si puedo, y si me salgo del rango,
  // tambien movere el tablero.
  GameManager& gM = GameManager::Instantiate();
  //sustituir por el ancho del personaje
  int w_tile =  gM.layer1_.map_[0][0].dst_rect_.w;
  int h_tile =  gM.layer1_.map_[0][0].dst_rect_.h;
  
  
  
  if(e->type == SDL_KEYDOWN){
    if(e->key.keysym.sym == SDLK_UP &&
    gM.board_[RBM::GetMatrixPosition(dst_rect_.y,-1)][dst_rect_.x].enabled_ == 1){
      vertical_mov = 1;
      dst_rect_.y = RBM::GetMatrixPosition(dst_rect_.y,-1);
      can_move = 1;
      direction_ = 0;
    }else if(e->key.keysym.sym == SDLK_UP &&
             gM.board_[RBM::GetMatrixPosition(dst_rect_.y,-1)][dst_rect_.x].enter_ == 1){
               direction_ = 0;
               gM.enter_cave_ = 1;

    }else if(e->key.keysym.sym == SDLK_DOWN &&
    gM.board_[RBM::GetMatrixPosition(dst_rect_.y,1)][dst_rect_.x].enabled_ == 1){
      vertical_mov = -1;
      dst_rect_.y = RBM::GetMatrixPosition(dst_rect_.y,1);
      can_move = 1;
      direction_ = 1;
    }else if(e->key.keysym.sym == SDLK_DOWN &&
             gM.board_[RBM::GetMatrixPosition(dst_rect_.y,1)][dst_rect_.x].enter_ == 1){
      
      direction_ = 1;
      gM.enter_cave_ = 1;

    }else if(e->key.keysym.sym == SDLK_LEFT &&
             gM.board_[dst_rect_.y][RBM::GetMatrixPosition(dst_rect_.x,-1)].enabled_ == 1){
      horizontal_mov = 1;
      dst_rect_.x = RBM::GetMatrixPosition(dst_rect_.x,-1);
      can_move = 1;
      direction_ = 2;
    }else if(e->key.keysym.sym == SDLK_LEFT &&
             gM.board_[dst_rect_.y][RBM::GetMatrixPosition(dst_rect_.x,-1)].enter_ == 1){
      
      direction_ = 2;
      gM.enter_cave_ = 1;

    }
    else if(e->key.keysym.sym == SDLK_RIGHT &&
    gM.board_[dst_rect_.y][RBM::GetMatrixPosition(dst_rect_.x,1)].enabled_ == 1){
      horizontal_mov = -1;
      dst_rect_.x = RBM::GetMatrixPosition(dst_rect_.x,1);
      can_move = 1;
      direction_ = 3;
    }
    else if(e->key.keysym.sym == SDLK_RIGHT &&
             gM.board_[dst_rect_.y][RBM::GetMatrixPosition(dst_rect_.x,1)].enter_ == 1){
      
      direction_ = 3;
      gM.enter_cave_ = 1;

    }

    
   // printf("Casilla character x:%d y:%d\n",dst_rect_.x, dst_rect_.y );

    if(1 == can_move){
      
      Board::x_origin_ += w_tile * horizontal_mov;
      Board::y_origin_ += h_tile * vertical_mov;
     
    }
    //printf(" x:%d y:%d\n",Board::x_origin_, Board::y_origin_ );
  }
  
}

void Character::draw(SDL_Renderer* ren){
  GameManager& gM = GameManager::Instantiate();
  //SDL_SetRenderDrawColor(ren,0,255,0,255);
  if(enabled_){
    SDL_Rect aux_rect;
    aux_rect = dst_rect_;
    if(gM.over_world_scene_==1){
    aux_rect.x = gM.layer1_.map_[0][0].dst_rect_.w *
                 gM.kViewSize/2;
    aux_rect.y = gM.layer1_.map_[0][0].dst_rect_.h *
                 gM.kViewSize/2; 
    }
    else{
      
      aux_rect.x = aux_rect.w *dst_rect_.x;             
      aux_rect.y = aux_rect.h *dst_rect_.y; 
      
      SDL_Rect aux_rect2;
      SDL_SetRenderDrawColor(ren,255,0,0,1);

      for(int i=0; i<= index_mov_; ++i){
        aux_rect2.w = 40; 
        aux_rect2.h = 40;
        aux_rect2.x = 40 * (previous_movs_[i]%16);
        aux_rect2.y = 40 * (previous_movs_[i]/16); 
        SDL_RenderDrawRect(ren,&aux_rect2); 
      }
    }     
    for (int i = 0; i<7; ++i){
        skin_[i].dst_rect_ = aux_rect;
        skin_[i].draw(ren);           
      }
    for (int i = 0; i<10; ++i){ 
        outfit_[i].dst_rect_ = aux_rect;
        outfit_[i].draw(ren);
      }  
      
  }
  //SDL_SetRenderDrawColor(ren,0,255,0,255);
 // SDL_RenderDrawRect(ren,&aux_rect);
}