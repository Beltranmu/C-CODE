// vehicle.cc
#include "vehicle.h"

void Vehicle::init(const unsigned int g, unsigned char p){

  gas_ = g;
  doors_ = p;
  locked_ = 0;

}

void Vehicle::init(const unsigned int g){

  gas_ = g;
  doors_ = 0;
  locked_ = 0;

}

void Vehicle::refuelGas(const unsigned int g){

  gas_ += g;

}

void Vehicle::consumeGas(const unsigned int g){

  gas_ -= g;

}

void Vehicle::toggleLock(const unsigned char l){

  locked_ = l;

}