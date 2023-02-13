/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Generic Rect Class definition
#ifndef __RECT_H__
#define __RECT_H__ 1

#include "entity.h"
#include "SDL.h"

struct Transform {

  JBA::Vector2 position;
  float rotation;
  JBA::Vector2 scale;

};

class Rect : public Entity {

  public:

    Rect();
    ~Rect();

    void init(const float w, const float h);
    void setTransform(const JBA::Vector2 pos, const float rot, const JBA::Vector2 sca);
    void setSpeed(const uint8_t s);
    void setSolid(const uint8_t s);
    void setFillColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    void setBorderColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    void draw(SDL_Renderer* renderer);

    SDL_Rect info_;
    Transform transform_;
    uint8_t speed_;
    uint8_t border_color_[4];  // RGBA
    uint8_t fill_color_[4];    // RGBA
    uint8_t solid_;

};

#endif // __RECT_H__