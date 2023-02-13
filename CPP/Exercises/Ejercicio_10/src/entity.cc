/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Generic Entity Class implementation

#include "entity.h"

uint8_t Entity::id_index_ = 0;

Entity::Entity() {

  tag_ = 0;
  enabled_ = 0;
  id_ = Entity::id_index_;
  ++Entity::id_index_;

}

Entity::~Entity();

void Entity::init() {

  tag_ = 0;
  enabled_ = 0;
  
}

void Entity::setEnable(){

  enabled_ = 1;

}

uint8_t get_id(){
  return id_;
}