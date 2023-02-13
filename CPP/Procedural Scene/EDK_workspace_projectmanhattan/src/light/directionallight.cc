#include <light/directionallight.h>

DirectionalLight::DirectionalLight() {
  direction[0] = 0.0f;
  direction[1] = 0.0f;
  direction[2] = 0.0f;

  ambient[0] = 0.0f;
  ambient[1] = 0.0f;
  ambient[2] = 0.0f;

  diffuse[0] = 0.0f;
  diffuse[1] = 0.0f;
  diffuse[2] = 0.0f;

  specular[0] = 0.0f;
  specular[1] = 0.0f;
  specular[2] = 0.0f;
}

void DirectionalLight::init(){
  direction[2] = -1.0f;
  ambient[0] = 1.0f;
  ambient[1] = 1.0f;
  ambient[2] = 1.0f;

  diffuse[0] = 1.0f;
  diffuse[1] = 1.0f;
  diffuse[2] = 1.0f;

  specular[0] = 1.0f;
  specular[1] = 1.0f;
  specular[2] = 1.0f;
}

DirectionalLight::~DirectionalLight(){

}
