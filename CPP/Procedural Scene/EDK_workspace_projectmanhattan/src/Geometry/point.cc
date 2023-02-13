#include <Geometry/point.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/opengl.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>
#include <stdio.h>

Point::Point(){
}

bool Point::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  switch (a){
    case EDK3::A_POSITION: // position_c
      gpu->enableVertexAttribute(vertex_buffer.get(),
        where_to_bind_attribute, EDK3::T_FLOAT_3);
      break;

    default:
      return false;
    break;
  }
  
  return true;
 
  
}

void Point::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  glDrawArrays(GL_POINTS, 0, 50);

}

void Point::Generate() {
    
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);

  /*float* points;
  points = (float*)malloc(sizeof(float) * intensity);

  float* speed;
  speed = (float*)malloc(sizeof(float) * ((float)(intensity * 0.5f)));*/

  for(int i = 0; i < 50; ++i){

    /**(points + i * 2) = (-100 + rand() % 201) / 100.f;
    *(points + i * 2 + 1) = (-100 + rand() % 201) / 100.f;
    *(speed + i) = minRainForce + rand() % maxRainForce;*/

    rain[i * 2] = (-100 + rand() % 201) / 100.f;
    rain[i * 2 + 1] = (-100 + rand() % 201) / 100.f;

  }

  for (int i = 0; i < 50; ++i) {

    printf("%f %f\n", rain[i * 2], rain[i * 2 + 1]);

  }
    
  //Points
  int size = sizeof(rain);
  vertex_buffer->init(size);
  vertex_buffer->uploadData((void*)rain, size, 0);
}

Point::~Point(){
}
