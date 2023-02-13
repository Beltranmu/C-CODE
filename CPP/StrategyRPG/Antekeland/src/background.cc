#include "background.h"
#include "gamemanager.h"
#include <stdio.h>

Background::Background(){
  horizontal_scroll_ = 0;
  vertical_scroll_ = 0;
  velocity_ = {0.0f,0.0f};
  texture_ = nullptr;
}

Background::~Background(){}



void Background::init(const RBM::Transform2 tr, 
                      const uint8_t hs, 
                      const uint8_t vs,
                      Texture& tex,
                      const RBM::Vec2* vel){
              
  transform_ = tr;
  horizontal_scroll_ = hs;
  vertical_scroll_ = vs; 
  texture_ = &tex;
  velocity_ = *vel;
  dst_rect_.x = (int)transform_.position.x;
  dst_rect_.y = (int)transform_.position.y;
  dst_rect_.w = texture_->width();
  dst_rect_.h = texture_->height();
  
}


void Background::updateB(float dt){
  if(enabled_){
    
    if(horizontal_scroll_){
      transform_.position.x += velocity_.x * dt;
    }
    
    if(vertical_scroll_){
      transform_.position.y += velocity_.y * dt;
    }
    
    if(transform_.position.x < 0){
      transform_.position.x += dst_rect_.w;
    }
    else if(transform_.position.x /*+ dst_rect_.w*/
    > GameManager::kBoardWidth){
      transform_.position.x -= dst_rect_.w;
    }
    
    if(transform_.position.y < 0){
      transform_.position.y += dst_rect_.h;
    }
    else if(transform_.position.y /*+ dst_rect_.h */
    > GameManager::kBoardHeight){
      transform_.position.y -= dst_rect_.h;
    }
    
  }
  dst_rect_.x = (int)transform_.position.x;
  dst_rect_.y = (int)transform_.position.y;
  
  /*printf("%f,%f\n",transform_.position.x, transform_.position.y);
  printf("%d,%d\n",dst_rect_.x, dst_rect_.y);
  printf("%d,%d\n",dst_rect_.w, dst_rect_.h);*/
}

void Background::update(float dt){
  if(enabled_){
    
    if(horizontal_scroll_){
      transform_.position.x += velocity_.x * dt;
    }
    
    if(vertical_scroll_){
      transform_.position.y += velocity_.y * dt;
    }
    
    if(transform_.position.x < 0){
      transform_.position.x += dst_rect_.w;
    }
    else if(transform_.position.x /*+ dst_rect_.w*/
    > GameManager::kWindowWidth){
      transform_.position.x -= dst_rect_.w;
    }
    
    if(transform_.position.y < 0){
      transform_.position.y += dst_rect_.h;
    }
    else if(transform_.position.y /*+ dst_rect_.h */
    > GameManager::kWindowHeight){
      transform_.position.y -= dst_rect_.h;
    }
    
  }
  dst_rect_.x = (int)transform_.position.x;
  dst_rect_.y = (int)transform_.position.y;
  
  /*printf("%f,%f\n",transform_.position.x, transform_.position.y);
  printf("%d,%d\n",dst_rect_.x, dst_rect_.y);
  printf("%d,%d\n",dst_rect_.w, dst_rect_.h);*/
}


void Background::draw(SDL_Renderer* ren){
  
  int x_origin = dst_rect_.x , x_end = 0;
  int y_origin = dst_rect_.y , y_end = 0;
  int32_t width = dst_rect_.w;
  int32_t height = dst_rect_.h;
  SDL_Rect aux_rect = {0,0,width,height};
  x_end = x_origin;
  y_end = y_origin;
  
  bool end = false;
  
  while(!end){
    if(x_origin - width < 0){
      end = true;
    }
    x_origin -= width;
  }
  
  end = false;
  
  while(!end){
    if(x_end + width > GameManager::kWindowWidth){
      end = true;
    }
    x_end += width;
  }
  end = false;
  
  while(!end){
    if(y_origin - height < 0){
      end = true;
    }
    y_origin -= height;
  }
  end = false;
  
  while(!end){
    if(y_end + height > GameManager::kWindowHeight){
      end = true;
    }
    y_end += height;
  }
  
  x_end = x_end - x_origin;
  y_end = y_end - y_origin;
  
  x_end = x_end/width;
  y_end = y_end/height;
  

  
  for(int i=0; i< y_end; ++i){
    for(int j=0; j< x_end; ++j){
      aux_rect.x = x_origin + j*width;
      aux_rect.y = y_origin + i*height;

      SDL_RenderCopy(ren,texture_->texture_,NULL,&aux_rect);
    }
  }
  //SDL_RenderCopy(render,texture_->texture_,NULL,&dst_rect_);
  
}