/**
 * Antekeland 2020
 * Author: Javier Benito Abolafio
 * Mail: benitoab@esat-alumni.com
 * University Development @ESAT
 */
 
#include "board.h"
#include "gamemanager.h"
#include <stdio.h>
#include <stdlib.h>

int32_t Board::x_origin_ = 0;
int32_t Board::y_origin_ = 0;

// Initializes the Layer 1
void Board::initLayer1(){

  GameManager& gM = GameManager::Instantiate();

  SDL_Rect aux_rect;  // Storages the info to init every tile of the board
  int x = gM.kBoardWidth/kBoardSize * (kBoardSize/gM.kViewSize);
  int y = gM.kBoardHeight/kBoardSize * (kBoardSize/gM.kViewSize);
  
  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){

      aux_rect = {c*x, r*y, x, y};
      
      if(rand()%100 <= kCellconcentration * 100){
        
        map_[r][c].init(&aux_rect, 0, rand()%kNState+1, 1);

      }else{

        map_[r][c].init(&aux_rect, 0, 0, 1);

      }
    }
  }


}

// Initializes the Layer 2
void Board::initLayer2(){

  GameManager& gM = GameManager::Instantiate();

  SDL_Rect aux_rect;  // Storages the info to init every tile of the board
  int x = gM.kBoardWidth/kBoardSize * (kBoardSize/gM.kViewSize);
  int y = gM.kBoardHeight/kBoardSize * (kBoardSize/gM.kViewSize);
  
  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){

      aux_rect = {c*x, r*y, x, y};
        
      map_[r][c].init(&aux_rect, 50, 0, 1);
      
    }
  }

}

void Board::reset0Position(){
  GameManager& gM = GameManager::Instantiate();  
  int total_w = (map_[0][0].dst_rect_.w*(kBoardSize));
  int total_h = (map_[0][0].dst_rect_.h*(kBoardSize));
  //printf("x: %d, y: %d\n", x_origin_, y_origin_);
  if(x_origin_ > gM.kBoardWidth){
    x_origin_ %= total_w;
  }
  
  else if(y_origin_ > gM.kBoardHeight){
    y_origin_ %= total_h;
  }
  
  else if(x_origin_ < -total_w){
    x_origin_ %= total_w;
  }
  
  else if(y_origin_ < -total_h){
    y_origin_ %= total_h;
  }  
}

// Update Position of the map
void Board::update0Position(){
  
  GameManager& gM = GameManager::Instantiate();
  
  int x = gM.kBoardWidth/kBoardSize * (kBoardSize/gM.kViewSize);
  int y = gM.kBoardHeight/kBoardSize * (kBoardSize/gM.kViewSize);

  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){
      map_[r][c].dst_rect_.x = c * x + x_origin_;
      map_[r][c].dst_rect_.y = r * y + y_origin_;
    }
  }
}

/**
 *  @brief Draws the map. It draw other maps
 *  @param renderer It's the renderer to be drawn
 */
void Board::drawMap(SDL_Renderer* renderer){
    
  int diagonal = 0;
  SDL_Rect aux_rect;
  SDL_Texture* map_t = GameManager::Instantiate().map_texture_->texture_;
/*
  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){

      map_[r][c].draw(renderer);

    }
  }  */
  
  if(x_origin_ > 0){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x - map_[r][c].dst_rect_.w*kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y ;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
       
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
    diagonal+=1;
  }  
  
  if(x_origin_ < -map_[0][0].dst_rect_.w){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x + map_[r][c].dst_rect_.w * kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y ;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
       
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
    diagonal+=10;
  }  
  
  if(y_origin_ > 0){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x ;
        aux_rect.y = map_[r][c].dst_rect_.y - map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
       
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
    diagonal+=100;
  }  
  
  if(y_origin_ < -map_[0][0].dst_rect_.h){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x ;
        aux_rect.y = map_[r][c].dst_rect_.y + map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
       
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
    diagonal+=1000;
  }
  
  //DIAGONALS
  
  // upper and left
  if(diagonal == 101){
   
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x - map_[r][c].dst_rect_.w*kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y - map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
       
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
  }
  
  // botton and left
  else if(diagonal == 1001){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x - map_[r][c].dst_rect_.w*kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y + map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
     
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
  }
  
  // upper right
  else if(diagonal == 110){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x + map_[r][c].dst_rect_.w*kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y - map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
        
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
  }
 
 // bottom right
  else if(diagonal == 1010){
    
    for(int r = 0; r < kBoardSize; ++r){
      for(int c = 0; c < kBoardSize; ++c){
        aux_rect.x = map_[r][c].dst_rect_.x + map_[r][c].dst_rect_.w*kBoardSize;
        aux_rect.y = map_[r][c].dst_rect_.y + map_[r][c].dst_rect_.h * kBoardSize;
        
        aux_rect.w = map_[r][c].dst_rect_.w;
        aux_rect.h = map_[r][c].dst_rect_.h;
        
        SDL_RenderCopy(renderer, map_t, 
                       &map_[r][c].snip_rect_, &aux_rect);
      }
    }
  }
}