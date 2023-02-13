#ifndef INCLUDE_SPORTLIGHT_H_
#define INCLUDE_SPORTLIGHT_H_ 1

#include <EDK3/node.h>
//#include <vector_3.h>
//#include <vector_4.h>

class SpotLight : public  EDK3::Node{
public:
  SpotLight();
  virtual ~SpotLight();
  void init();

  float position[3]; // =  camera pos / farola pos
  float direction[3]; // =  camera forward / farola forward
  float ambient[3];
  float diffuse[3];
  float specular[3];

  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
  

protected:

private:

};


#endif