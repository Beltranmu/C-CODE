#include "combat.h"
#include "gamemanager.h"
#include <stdio.h>


Combat::Combat(){
  turn_ = 0;
  stage_ = 0;
  n_entities_ = 0;
  current_char_ = nullptr;
}

Combat::~Combat(){}
/*
void Combat::drawMark(SDL_Renderer* ren){
  
  GameManager& gM = GameManager::Instantiate();
  SDL_Rect mark_rect[8];
  int aux = gM.layer1_.map_[0][0].dst_rect_.w *gM.kViewSize/2;
  
  int w = 10, h = 10;
  
  mark_rect[0].x = aux - w;
  mark_rect[0].y = aux - h;
  mark_rect[0].w = gM.player_[0].dst_rect_.w + w;
  mark_rect[0].h = h;
  
  mark_rect[1].x = aux + gM.player_[0].dst_rect_.w;
  mark_rect[1].y = aux - h;
  mark_rect[1].w = w;
  mark_rect[1].h = gM.player_[0].dst_rect_.h + h;
 
  mark_rect[2].x = aux;
  mark_rect[2].y = aux + gM.player_[0].dst_rect_.h;
  mark_rect[2].w = gM.player_[0].dst_rect_.w + w;
  mark_rect[2].h = h;
  
  mark_rect[3].x = aux - w;
  mark_rect[3].y = aux;
  mark_rect[3].w = w;
  mark_rect[3].h = gM.player_[0].dst_rect_.h + h;
  
  SDL_SetRenderDrawColor(ren, 255,0,0,255);
  SDL_RenderFillRects(ren,mark_rect,4);
  
}
*/

void Combat::initCombat(Character& current_char){
  
  GameManager& gM = GameManager::Instantiate();
  int n=0; //Lenght of aux_text;
  int spacing = 40;
  int font_size = 25;
  char* font = "../data/fonts/combat.otf";
  char aux_text[50];
  stats_scale_ = 0.9;
  ui_stats_rect_.x = gM.kBoardWidth + 35;
  ui_stats_rect_.y = -100;
  ui_stats_rect_.w = (int)(stats_scale_ * 
                  (gM.kWindowWidth - gM.kBoardWidth));
  ui_stats_rect_.h = (int)(stats_scale_ * 
                  (gM.kWindowHeight - gM.kBoardHeight));
  
  SDL_Color white = {255,255,255,255};
  SDL_Rect aux_rect;
  aux_rect.x = 0;
  aux_rect.y = gM.kBoardHeight;
  aux_rect.w = 200;
  aux_rect.h = 50;
  /*
  att_text_[0].init("../data/fonts/combat.otf",font_size,white,
                    current_char.char_attacks_[0].name,aux_rect);
  aux_rect.x = 200;
  aux_rect.y = gM.kBoardHeight;
  
  att_text_[1].init("../data/fonts/combat.otf",font_size,white,
                    current_char.char_attacks_[1].name,aux_rect);               

  aux_rect.x = 0;
  aux_rect.y = gM.kBoardHeight+50;
  
  att_text_[2].init("../data/fonts/combat.otf",font_size,white,
                    current_char.char_attacks_[2].name,aux_rect);

  aux_rect.x = 200;
  aux_rect.y = gM.kBoardHeight+50;
  
  att_text_[3].init("../data/fonts/combat.otf",font_size,white,
                    "el zumo de naranja lleva mucha azucar xdddddddddddddddd",aux_rect); 
*/  
  SDL_Rect title_rect = {685, 10, 300, 50};
  SDL_Color title_color = {255,255,0,255};
  stats_title_[0].init(font, (uint16_t)(stats_scale_ *font_size), title_color,"CHARACTER STATS", title_rect);
  
  SDL_Rect actions_rect = {760, 430, 150, 50};
  stats_title_[1].init(font, (uint16_t)(stats_scale_ *font_size), title_color,"ACTIONS", actions_rect);

  aux_rect.x = (int)(ui_stats_rect_.x + stats_scale_ * spacing);
  aux_rect.y = (int)(ui_stats_rect_.y + stats_scale_ * 200);
  aux_rect.w = (int)(stats_scale_ * 15*3);
  aux_rect.h = (int)(stats_scale_ * font_size);

  SDL_Rect char_rect;
  char_rect.x = aux_rect.x;
  char_rect.y = aux_rect.y-10;
  char_rect.w = 100;
  char_rect.h = aux_rect.h + 10;
  if (current_char.char_id_<4){
    n = sprintf (aux_text, "Character: %d", current_char.char_id_);
  }
  else{
   n = sprintf (aux_text, "Enemy: %d", current_char.char_id_-4);
  }

  stats_text_[8].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, char_rect);
                      
  aux_rect.y += (int)(stats_scale_ * spacing);
  stats_text_[0].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white,"HP:", aux_rect);
                      
  aux_rect.y += (int)(stats_scale_ * spacing); 
  aux_rect.w = (int)(stats_scale_ * 15*5);
  stats_text_[1].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white,"Mana:", aux_rect);
                      
                                           
  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "AD: %d", 
            current_char.current_.physical_att);
            
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[2].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white,aux_text, aux_rect);


  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "AP: %d", 
            current_char.current_.magic_att);
            
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[3].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, aux_rect);
                      
                      
  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "Armor: %d", 
            current_char.current_.armor);          
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[4].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, aux_rect);
     
     
  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "Magic Res.: %d", 
            current_char.current_.magic_resist);          
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[5].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, aux_rect);
                      
                      
                      
  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "Movements: %d", 
            current_char.current_.movement);          
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[6].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, aux_rect);
                      
                      
  aux_rect.y += (int)(stats_scale_ * spacing);  
  n = sprintf (aux_text, "Lvl: %d, Exp: %d/%d", 
            current_char.level_,current_char.xp_,100);          
  aux_rect.w = (int)(stats_scale_ * 15*n);
  stats_text_[7].init(font,
                     (uint16_t)(stats_scale_ *font_size),
                      white, aux_text, aux_rect);  
  SDL_Rect grey_rect;
  SDL_Color grey = {170,170,170,170};
  //HP Grey Rect
  grey_rect.x =(int)(stats_text_[0].dst_rect_.x + stats_scale_*60);
  grey_rect.y =(int)(stats_text_[0].dst_rect_.y+7 * stats_scale_);
  grey_rect.w =(int)(200 * stats_scale_);
  grey_rect.h =(int)(15 * stats_scale_);
  
  stats_rect_[0].dst_rect_ = grey_rect;
  stats_rect_[3].dst_rect_ = grey_rect;
  stats_rect_[0].fill_color_ = grey;
 
  
  //Mana Grey Rect
  grey_rect.y = (int)(stats_text_[1].dst_rect_.y+7 * stats_scale_);
  grey_rect.w =(int)(150 * stats_scale_);
  grey_rect.x += (int)(20 * stats_scale_);
  
  stats_rect_[1].dst_rect_ = grey_rect;
  stats_rect_[4].dst_rect_ = grey_rect;
  stats_rect_[1].fill_color_ = grey;
  stats_rect_[4].fill_color_ = {0,105,255,255};
  
  
  //Exp Grey Rect
  grey_rect.y = (int)(stats_text_[7].dst_rect_.y+40 * stats_scale_);
  grey_rect.x = (int)(stats_text_[7].dst_rect_.x+20 * stats_scale_);
  grey_rect.h = (int)(8 * stats_scale_);
  
  stats_rect_[2].dst_rect_ = grey_rect;
  stats_rect_[5].dst_rect_ = grey_rect;
  stats_rect_[2].fill_color_ = grey;
  stats_rect_[5].fill_color_ = {255,255,0,255};
  // stats_rect_[5].fill_color_ = {209,151,219,200};
  



                      

}
/*
void Combat::drawAttacks(SDL_Renderer* ren){
  
  att_text_[0].draw(ren);
  att_text_[1].draw(ren);
  att_text_[2].draw(ren);
  att_text_[3].draw(ren);
  
  
}
*/
void Combat::updateStats(){
  //HP
  
  float bar_percentage = 1;
  char aux_text[50];
  //SDL_Rect grey_rect, hp_rect;
 // GameManager& gM = GameManager::Instantiate();
  
  //grey_rect.x =(int)(stats_text_[0].dst_rect_.x + stats_scale_*60);
  //grey_rect.y =(int)(stats_text_[0].dst_rect_.y+7 * stats_scale_);
  //grey_rect.w =(int)(150 * stats_scale_);
  //grey_rect.h =(int)(15 * stats_scale_);
  
  //hp_rect = grey_rect;
  bar_percentage = ((float)(current_char_->current_.hp) / 
                   (float)(current_char_->base_.hp));  
  //stats_text_[0].draw(ren);                   
  //SDL_SetRenderDrawColor(ren, 170,170,170,170);
  //SDL_RenderFillRect(ren,&grey_rect);
  stats_rect_[3].dst_rect_.w = (int)(bar_percentage * stats_rect_[0].dst_rect_.w);
  SDL_Color hp_color;
    hp_color.b = 0;
    hp_color.a = 255;
    
  if(bar_percentage >= 0.85){
    hp_color.r = 0;
    hp_color.g = 255;
  }
  else if(0.85 > bar_percentage && bar_percentage > 0.6){
    hp_color.r = 123;
    hp_color.g = 255;
  }
  else if(0.6 >= bar_percentage && bar_percentage >= 0.4){
    hp_color.r = 255;
    hp_color.g = 255;
  }
  else if(0.4 > bar_percentage && bar_percentage > 0.15){
    hp_color.r = 255;
    hp_color.g = 123;
  }
  else{
    hp_color.r = 255;
    hp_color.g = 0;
  }
  stats_rect_[3].fill_color_ = hp_color;

 // SDL_SetRenderDrawColor(ren, hp_color.r, hp_color.g, 
                         //hp_color.b, hp_color.a);
  //SDL_RenderFillRect(ren,&hp_rect);
  
  
  //MANA
  
  //grey_rect.y = (int)(stats_text_[1].dst_rect_.y+7 * stats_scale_);
 // grey_rect.x += (int)(20 * stats_scale_);
 
  //hp_rect = grey_rect;
  
  bar_percentage = ((float)current_char_->current_.mana / 
                   (float)current_char_->base_.mana);  
  //stats_text_[1].draw(ren);   
  //SDL_SetRenderDrawColor(ren, 170,170,170,170);
 // SDL_RenderFillRect(ren,&grey_rect);
  stats_rect_[4].dst_rect_.w = (int)(bar_percentage *stats_rect_[1].dst_rect_.w);

  //SDL_SetRenderDrawColor(ren,0,105,255,hp_color.a);
  //SDL_RenderFillRect(ren,&hp_rect);
  

  //stats_text_[2].draw(ren); 
  //stats_text_[3].draw(ren); 
  //stats_text_[4].draw(ren); 
  //stats_text_[5].draw(ren); 
  //stats_text_[6].draw(ren); 
  
  //stats_text_[7].draw(ren);
  
  //grey_rect.y = (int)(stats_text_[7].dst_rect_.y+40 * stats_scale_);
  //grey_rect.x = (int)(stats_text_[7].dst_rect_.x+20 * stats_scale_);
  //grey_rect.h = (int)(8 * stats_scale_);
 
  //hp_rect = grey_rect;
  
  // bar_percentage = ((float)aux_char.current_.xp_ / 
                   // (float)aux_char.base_.xp_); 
  bar_percentage = ((float)current_char_->xp_ / 100.0f);
  //stats_text_[1].draw(ren);   
  //SDL_SetRenderDrawColor(ren, 170,170,170,170);
  //SDL_RenderFillRect(ren,&grey_rect);
  stats_rect_[5].dst_rect_.w = (int)(bar_percentage * stats_rect_[2].dst_rect_.w);

  //SDL_SetRenderDrawColor(ren,209,151,219,200);
  //SDL_RenderFillRect(ren,&hp_rect);

 //MOVS 
 
 sprintf(aux_text, "Movements: %d", 
          current_char_->current_.movement);   
 stats_text_[6].changeText(aux_text);
 
 sprintf(aux_text, "Lvl: %d, Exp: %d/%d", 
          current_char_->level_,current_char_->xp_,100);  
 stats_text_[7].changeText(aux_text);
 
}








