#include <light/pointligh.h>

PointLight::PointLight() {

  position[0] = 0.0f;
  position[1] = 0.0f;
  position[2] = 0.0f;

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
}

void PointLight::init(){
 
  ambient[0] = 0.5f;
  ambient[1] = 0.5f;
  ambient[2] = 0.5f;

  diffuse[0] = 0.5f;
  diffuse[1] = 0.5f;
  diffuse[2] = 0.5f;

  specular[0] = 0.5f;
  specular[1] = 0.5f;
  specular[2] = 0.5f;

  constant = 1.f;
  linear = 1.f;
}

PointLight::~PointLight(){

}
