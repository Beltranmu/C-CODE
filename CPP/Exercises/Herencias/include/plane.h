// plane.h
#ifndef __PLANE_H__
#define __PLANE_H__ 1

#include "vehicle.h"

class Plane : public Vehicle {

  public:

  // Methods
  void init(const unsigned int g, unsigned char p);
  void openDoors();
  void closeDoors();
  void pullGear();
  void lowerGear();

  // Attributes
  unsigned char gear_;

};

#endif // __PLANE_H__