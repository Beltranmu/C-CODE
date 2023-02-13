#ifndef INCLUDE_POINTLIGHT_H_
#define INCLUDE_POINTLIGHT_H_ 1

#include <EDK3/node.h>
//#include <vector_3.h>
//#include <vector_4.h>

class PointLight : public  EDK3::Node{
public:
  PointLight();
  virtual ~PointLight();
  void init();

  float position[3];
  float ambient[3];
  float diffuse[3];
  float specular[3];

  float constant;
  float linear;
  float quadratic;

  

protected:

private:

};


#endif