// plane.cc
#include "plane.h"

void Plane::init(const unsigned int g, unsigned char p){

  gas_ = g;
  doors_ = p;
  locked_ = 1;
  gear_ = 1;

}

void Plane::openDoors(){

  if(gear_){ locked_ = 1; }

}

void Plane::closeDoors(){

  locked_ = 0;

}

void Plane::pullGear(){

  gear_ = 0;

}

void Plane::lowerGear(){

  gear_ = 1;

}