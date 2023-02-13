/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Generic Entity Class definition
#ifndef __ENTITY_H__
#define __ENTITY_H__ 1

#include "JBAmath.h"
#include "SDL.h"

#include <stdint.h>

class Entity {

  public:
    Entity();
    ~Entity();

    void init();
    void setEnable();
    uint8_t get_id();

    int32_t tag_;
    uint8_t enabled_;
    static uint8_t id_index_;

  private:
    uint8_t id_;

};

#endif //__ENTITY_H__
