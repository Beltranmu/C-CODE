#include "rect.h"
#include <stdio.h>
#include "stdlib.h"

Rect::Rect(){
  dst_rect_ = {0,0,1,1};
  enabled_ = 0;
  border_color_ = {255,120,0,255};
  fill_color_ = {255,120,0,255};
  solid_ =0;
  centered_ = 1;
  speed_ =1;
  centered_ = 1;
  speed_rot_ =0.05f;
  
}

Rect::~Rect(){
  
}

void Rect::init(){
  dst_rect_ = {0,0,1,1};

  border_color_ = {255,120,0,255};
  fill_color_ = {255,120,0,255};
  solid_ = 1;
  centered_ = 1;
  speed_ = 0;
  speed_rot_ = 0.0f;
  
}

/*
void Rect::init(RBM::Transform2* tr){
  //dst_rect_ = {0,0,1,1};
  Entity::init(2,1,tr,{0,0,1,1});
  
  border_color_ = {255,255,255,255};
  fill_color_ = {255,255,255,255};
  solid_ = 1;
  centered_ = 0;
  speed_ = 0;
  speed_rot_ = 0.0f;
}*/


/*
void Rect::init(SDL_Rect* r,
                int32_t t, int8_t e, uint8_t c,
                RBM::Transform2* tr,
                SDL_Color* fc, SDL_Color* bc,
                uint8_t s, int32_t speed,
                float speedr){
                  
  Entity::init(t,e,tr,*r);
  
  border_color_ = *bc;
  fill_color_ = *fc;
  solid_ = s;
  centered_ = c;
  speed_ = speed;
  speed_rot_ = speedr;
  
}*/
/*
void Rect::horizontalMovement(int w){
  
  transform_.position.x -= speed_;
  if(transform_.position.x - transform_.scale.x < 0){
    transform_.position.x = (float)w;
  }
}

void Rect::changeColor(){
  border_color_.r = rand()%256;
  border_color_.g = rand()%256;
  border_color_.b = rand()%256; 
}

void Rect::rotate(){
  transform_.rotation += speed_rot_;
}
*/
void Rect::draw(SDL_Renderer* ren){
  
  SDL_SetRenderDrawColor(ren,
                         fill_color_.r, 
                         fill_color_.g,
                         fill_color_.b,
                         fill_color_.a);
                         
  SDL_RenderFillRect(ren, &dst_rect_);                       
//  SDL_RenderDrawRect(ren, &dst_rect_);
}
/*
void Rect::drawT(SDL_Renderer* renderer){
  
  if(enabled_){
    
    RBM::Vec2 point_to_draw[5];
    
    RBM::Vec3 rect_points[4];
    RBM::Vec3 rect_points_c[4];
    
    rect_points[0]={0.0f,
                    0.0f, 1.0f};
    rect_points[1]={(float)dst_rect_.w, 
                    0.0f, 1.0f};
    rect_points[2]={(float)dst_rect_.w, 
                    (float)dst_rect_.h, 1.0f};
    rect_points[3]={0.0f,              
                   (float)dst_rect_.h, 1.0f};

    rect_points_c[0]={0.0f - (float)dst_rect_.w/2,
                      0.0f - (float)dst_rect_.h/2, 1.0f};
    rect_points_c[1]={0.0f + (float)dst_rect_.w/2,
                      0.0f - (float)dst_rect_.h/2, 1.0f};
    rect_points_c[2]={0.0f + (float)dst_rect_.w/2,
                      0.0f + (float)dst_rect_.h/2, 1.0f};
    rect_points_c[3]={0.0f - (float)dst_rect_.w/2,
                      0.0f + (float)dst_rect_.h/2, 1.0f};

    RBM::Mat3 tr = RBM::M3Identity();
    
    tr = RBM::M3Product(RBM::M3Scale(transform_.scale.x, 
                                     transform_.scale.y),tr);
    tr = RBM::M3Product(RBM::M3Rotation(transform_.rotation),tr);    
    tr = RBM::M3Product(RBM::M3Translate(transform_.position.x, 
                                         transform_.position.y),tr);
    
    
    if(centered_){
      for(int i=0; i< 4; ++i){
        RBM::Vec3 aux;
        aux = RBM::M3ProductVec3(tr, rect_points_c[i]);
        //RBM::PrintV3(&aux);
        point_to_draw[i].x = aux.x;
        point_to_draw[i].y = aux.y;
      }
    }
    else{
      for(int i=0; i< 4; ++i){
        RBM::Vec3 aux;
        aux = RBM::M3ProductVec3(tr, rect_points[i]);
        point_to_draw[i].x = aux.x;
        point_to_draw[i].y = aux.y;
      }
    }
    point_to_draw[4] = point_to_draw[0];
    
    for(int i = 0; i<4; ++i){
      
      SDL_SetRenderDrawColor(renderer,border_color_.r,border_color_.g,
                      border_color_.b,border_color_.a);
      
      SDL_RenderDrawLine(renderer,(int)point_to_draw[i].x,
                                  (int)point_to_draw[i].y,
                                  (int)point_to_draw[i+1].x,
                                  (int)point_to_draw[i+1].y); 
    }
    // tengo que revisarlo
    //dst_rect_.x = (int)point_to_draw[0].x;
    //dst_rect_.y = (int)point_to_draw[0].y;
  }
}*/