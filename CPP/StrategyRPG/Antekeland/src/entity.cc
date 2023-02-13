/**
 * Antekeland 2020
 * Author: Ricardo Beltrán Muriel
 * Mail: beltranmu@esat-alumni.com
 * University Development @ESAT
 */

  
#include "entity.h"

int32_t Entity::next_id_ = 0;

Entity::Entity(){
  
  tag_ = 0;
  enabled_ = 1;
  id_ = next_id_;
  ++next_id_;
  transform_.position = {0.0f, 0.0f};
  transform_.rotation= 0.0f;
  transform_.scale = {1.0f, 1.0f};
  dst_rect_ = {0,0,1,1};
}

Entity::Entity(const Entity& e){
  
  tag_ = e.tag_;
  enabled_ = e.enabled_;
  id_ = next_id_;
  ++next_id_;
  transform_.position = e.transform_.position;
  transform_.rotation= e.transform_.rotation;
  transform_.scale = e.transform_.scale;
  dst_rect_ = e.dst_rect_;
}

Entity::~Entity(){
}

void Entity::init(){
  
  tag_ = 0;
  enabled_ = 1;
  id_ = next_id_;
  ++next_id_;
  transform_.position = {0.0f, 0.0f};
  transform_.rotation= 0.0f;
  transform_.scale = {1.0f, 1.0f};
  dst_rect_ = {0,0,1,1};
}
/*
void Entity::init(const int32_t t, const int8_t e, 
                  const RBM::Transform2* tr, 
                  const SDL_Rect r){
  tag_ = t;
  enabled_ = e;
  id_ = next_id_;
  ++next_id_;
  
  transform_.position = tr->position;
  transform_.rotation = tr->rotation;
  transform_.scale = tr->scale;
  dst_rect_ = r;
}
*/
// Setters Getters

void Entity::set_position(RBM::Vec2 v){
  transform_.position = v;
}
RBM::Vec2 Entity::position(){
  return transform_.position;
}


void Entity::set_rotation(float r){
  transform_.rotation = r;
}
float Entity::rotation(){
  return transform_.rotation;
}


void Entity::set_scale(RBM::Vec2 v){
  transform_.scale = v;
}
RBM::Vec2 Entity::scale(){
  return transform_.scale;
}


int32_t Entity::id(){
  return id_;
}

