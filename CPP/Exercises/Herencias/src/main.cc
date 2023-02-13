#include <stdio.h>
#include "vehicle.h"
#include "car.h"
#include "motorbike.h"
#include "plane.h"

int main(){

  Car my_car;
  Plane my_plane;
  Motorbike my_moto;

  my_car.init(200, 5);
  my_plane.init(5000, 17);
  my_moto.init(50);

  printf("Car: %f %d\nPlane: %f %d\nMoto: %f %d\n",
        my_car.gas_, my_car.doors_,
        my_plane.gas_, my_plane.doors_,
        my_moto.gas_, my_moto.doors_);

  unsigned char c;
  scanf("%c", &c);

  return 0;
}