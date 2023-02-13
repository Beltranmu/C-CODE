#include <light/sportlight.h>

SpotLight::SpotLight() {

  position[0] = 0.0f;
  position[1] = 0.0f;
  position[2] = 0.0f;

  direction[0] = -1.0f;
  direction[0] = 0.0f;
  direction[0] = 0.0f;

  ambient[0] = 0.0f;
  ambient[1] = 0.0f;
  ambient[2] = 0.0f;

  diffuse[0] = 0.0f;
  diffuse[1] = 0.0f;
  diffuse[2] = 0.0f;

  specular[0] = 0.0f;
  specular[1] = 0.0f;
  specular[2] = 0.0f;

  constant = 0.0f;
  linear = 0.0f;
  quadratic = 0.0f;

  cutOff = 0.0f;
  outerCutOff = 0.0f;

}

void SpotLight::init(){
 
  ambient[0] = 0.35f;
  ambient[1] = 0.35f;
  ambient[2] = 0.35f;

  diffuse[0] = 0.35f;
  diffuse[1] = 0.35f;
  diffuse[2] = 0.35f;

  specular[0] = 0.35f;
  specular[1] = 0.35f;
  specular[2] = 0.35f;

  constant = 1.f;
  linear = 1.f;
  cutOff = 1.206f;
  outerCutOff = 0.809;
}

SpotLight::~SpotLight(){

}
