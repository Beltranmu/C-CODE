/**
 * Antekeland 2020
 * Author: Ricardo Beltrán Muriel
 * Mail: beltranmu@esat-alumni.com
 * University Development @ESAT
 */

#include "sprite.h"

Sprite::Sprite(){
  texture_ = nullptr;
  dst_rect_.x = 0;
  dst_rect_.y = 0;
  dst_rect_.w = 1;
  dst_rect_.h = 1;
  
  snip_rect_.x = 0;
  snip_rect_.y = 0;
  snip_rect_.w = 1;
  snip_rect_.h = 1;
}

Sprite::~Sprite(){
}

void Sprite::initSprite(Texture& t, 
                        const SDL_Rect* position,
                        const SDL_Rect* snip){
  
  texture_ = &t;
  dst_rect_ = *position;
  snip_rect_ = *snip;
  
}

int Sprite::width(){
  return dst_rect_.w;
}
int Sprite::height(){
  return dst_rect_.h;
}

void Sprite::set_texture(const Texture& tex){

  *texture_ = tex;

}

void Sprite::draw(SDL_Renderer* render){
  if(texture_!= nullptr){
    SDL_RenderCopy(render,texture_->texture_,
                 &snip_rect_, &dst_rect_);
  }
}