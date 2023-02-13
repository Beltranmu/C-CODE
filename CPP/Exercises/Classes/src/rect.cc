/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Generic Rect Class implementation

#include "rect.h"

Rect::Rect() {

  transform_.position = {0.0f, 0.0f};
  transform_.rotation = 0.0f;
  transform_.scale = {1.0f, 1.0f};

  info_.w = 0.0f;
  info_.h = 0.0f;

  for(int i = 0; i < 4; ++i) {

    border_color_[i] = 0xFF;
    fill_color_[i] = 0xFF;

  }

  solid_ = 0;

}

Rect::~Rect(){  
}

void Rect::init(const float w, const float h){

  info_.x = 0;
  info_.y = 0;
  info_.w = w;
  info_.h = h;

}

void Rect::setTransform(const JBA::Vector2 pos, const float rot, const JBA::Vector2 sca){

  transform_.position = pos;
  transform_.rotation = rot;
  transform_.scale = sca;

}

void Rect::setSpeed(const uint8_t s){

  speed_ = s;

}

void Rect::setSolid(const uint8_t s) {

  solid_ = s;

}

void Rect::setFillColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {

  fill_color_[0] = r;
  fill_color_[1] = g;
  fill_color_[2] = b;
  fill_color_[3] = a;

}

void Rect::setBorderColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {

  border_color_[0] = r;
  border_color_[1] = g;
  border_color_[2] = b;
  border_color_[3] = a;

}

void Rect::draw(SDL_Renderer* renderer) {

  if(!solid_){

    SDL_Point points_to_draw[5];

    SDL_Point points[5] = {{info_.x, info_.y},
                           {info_.x + info_.w, info_.y},
                           {info_.x + info_.w, info_.y + info_.h},
                           {info_.x, info_.y + info_.h},
                           {info_.x, info_.y}};

    JBA::Matrix3 tr = JBA::M3Identity();
    tr = JBA::M3Multiply(JBA::M3Scale(transform_.scale.x, transform_.scale.y), tr);

    tr = JBA::M3Multiply(JBA::M3Rotate(transform_.rotation), tr);

    tr = JBA::M3Multiply(JBA::M3Translate(transform_.position.x, transform_.position.y), tr);

    for(int i = 0; i < 5; ++i){

      JBA::Vector3 aux_vec;

      aux_vec = JBA::M3MultiplyVector3(tr, points[i]);

      points_to_draw[i].x = aux_vec.x;
      points_to_draw[i].y = aux_vec.y;

    }
    
    SDL_SetRenderDrawColor(renderer, border_color_[0],
                          border_color_[1], border_color_[2],
                          border_color_[3]);                       
    
    SDL_RenderDrawLines(renderer, points_to_draw, 5);

  }else{

    SDL_Rect r = {transform_.position.x, transform_.position.y, info_.w, info_.h};

    SDL_SetRenderDrawColor(renderer, fill_color_[0],
                          fill_color_[1], fill_color_[2],
                          fill_color_[3]);
    SDL_RenderFillRect(renderer, &r);

  }

}
