// vechile.h
#ifndef __VEHICLE_H__
#define __VEHICLE_H__ 1

class Vehicle{

  public:
  void init(const unsigned int g, unsigned char p);
  void init(const unsigned int g);
  void refuelGas(const unsigned int g);
  void consumeGas(const unsigned int g);
  void toggleLock(const unsigned char l);

  // Attributes
  float gas_;
  unsigned char doors_;
  unsigned char locked_;

};

#endif //   __VECHILE_H__