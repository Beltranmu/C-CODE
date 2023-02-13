#ifndef INCLUDE_DIRECTIONALLIGHT_H_
#define INCLUDE_DIRECTIONALLIGHT_H_ 1

#include <EDK3/node.h>
//#include <vector_3.h>
//#include <vector_4.h>

class DirectionalLight : public  EDK3::Node{
public:
  DirectionalLight();
  virtual ~DirectionalLight();
  void init();

  float direction[3];
  float ambient[3];
  float diffuse[3];
  float specular[3];

protected:

  
private:



};




#endif