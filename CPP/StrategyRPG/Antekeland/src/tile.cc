/**
 * Antekeland 2020
 * Author: Ricardo Beltrán Muriel
 * Mail: beltranmu@esat-alumni.com
 * University Development @ESAT
 */

#include "tile.h"
#include "gamemanager.h"

Tile::Tile(){

  state_ = 0;
  type_ = 0;
  dst_rect_.x = 0;
  dst_rect_.y = 0;
  dst_rect_.w = 1;
  dst_rect_.h = 1;
  tag_ = 1;
  enabled_ = 1;

}

Tile::~Tile(){
}

void Tile::init(const SDL_Rect* r, const uint8_t t, const uint8_t s, const int8_t e){

  dst_rect_ = *r;  
  state_ = s;
  type_ = t;
  enabled_ = e;
  texture_ = GameManager::Instantiate().map_texture_;

}

void Tile::draw(SDL_Renderer* ren){
  Sprite::draw(ren);
  
  if(GameManager::Instantiate().over_world_scene_){
    int diagonal = 0;
    SDL_Rect aux_rect;
    SDL_Texture* map_t = GameManager::Instantiate().map_texture_->texture_;
    
    if(Board::x_origin_ > 0){
      aux_rect.x = dst_rect_.x - dst_rect_.w*Board::kBoardSize;
      aux_rect.y = dst_rect_.y ;
                   
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
     
      SDL_RenderCopy(ren, map_t, 
                         &snip_rect_, &aux_rect);
        
      
      diagonal+=1;
    }  
    
   // if(Board::x_origin_ < -map_[0][0].dst_rect_.w){
    if(Board::x_origin_ < dst_rect_.w){
     
      aux_rect.x = dst_rect_.x + dst_rect_.w * Board::kBoardSize;
      aux_rect.y = dst_rect_.y ;
      
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
     
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);
    
      diagonal+=10;
    }  
    
    if(Board::y_origin_ > 0){
      
      aux_rect.x = dst_rect_.x ;
      aux_rect.y = dst_rect_.y - dst_rect_.h * Board::kBoardSize;
          
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
         
      SDL_RenderCopy(ren, map_t, 
                      &snip_rect_, &aux_rect);
     
      diagonal+=100;
    }  
    
    //if(Board::y_origin_ < -map_[0][0].dst_rect_.h){
    if(Board::y_origin_ < dst_rect_.h){
      
      aux_rect.x = dst_rect_.x ;
      aux_rect.y = dst_rect_.y + dst_rect_.h * Board::kBoardSize;
      
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
     
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);
      
      diagonal+=1000;
    }
    
    //DIAGONALS
    
    // upper and left
    if(diagonal == 101){
     
      aux_rect.x = dst_rect_.x - dst_rect_.w * Board::kBoardSize;
      aux_rect.y = dst_rect_.y - dst_rect_.h * Board::kBoardSize;
                   
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
     
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);
     
    }
    
    // botton and left
    else if(diagonal == 1001){
      
     
      aux_rect.x = dst_rect_.x - dst_rect_.w * Board::kBoardSize;
      aux_rect.y = dst_rect_.y + dst_rect_.h * Board::kBoardSize;
      
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
   
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);
     
    }
    
    // upper right
    else if(diagonal == 110){
      
      
      aux_rect.x = dst_rect_.x + dst_rect_.w * Board::kBoardSize;
      aux_rect.y = dst_rect_.y - dst_rect_.h * Board::kBoardSize;
      
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
      
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);

    }
   
   // bottom right
    else if(diagonal == 1010){
      
      aux_rect.x = dst_rect_.x + dst_rect_.w * Board::kBoardSize;
      aux_rect.y = dst_rect_.y + dst_rect_.h * Board::kBoardSize;
      
      aux_rect.w = dst_rect_.w;
      aux_rect.h = dst_rect_.h;
      
      SDL_RenderCopy(ren, map_t, 
                     &snip_rect_, &aux_rect);
     
    }
  }
}

void Tile::initSubSprite(){

  snip_rect_.x = type_ * 64;
  snip_rect_.y = state_ * 64;
  snip_rect_.w = 64;
  snip_rect_.h = 64;

}