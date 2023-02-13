#include <geometry/triangler.h>
#include<EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>


Triangler::Triangler(){
}

bool Triangler::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  switch (a){
  case EDK3::A_POSITION: // position
    gpu->enableVertexAttribute(vertex_buffer.get(),
      where_to_bind_attribute, EDK3::T_FLOAT_3);
    break;

  case EDK3::A_NORMAL: //normal
    gpu->enableVertexAttribute(vertex_buffer.get(),
      where_to_bind_attribute, EDK3::T_FLOAT_3, true, 
      size_points);
    break;

  default:
    return false;
    break;
  }
  
  return true;
 
  
}

void Triangler::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  gpu->drawElements(EDK3::dev::GPUManager::kDrawMode_Triangles,
    kNTriangles, element_buffer.get(), EDK3::T_UINT);
}

void Triangler::Generate() {

  //hecho de las dos formas para que no se me olviden
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);
  gpu->newBuffer(&element_buffer);

  Vector3 t_points[3] = { {-1.0f, -0.5f, 0.0f },
                          {1.0f, -0.5f, 0.0f},
                          {0.0f, 1.0f, 0.0f}};

  float t_normal[] = {0.0f, 0.0f, 1.0f,
                      0.0f, 0.0f, 1.0f,
                      0.0f, 0.0f, 1.0f };

  size_points = sizeof(t_points);

  unsigned int vertex_order[] = {0, 1, 2,
                                 2, 1, 0};

  kNTriangles = sizeof(vertex_order) / sizeof(unsigned int);
  //Vertex
  vertex_buffer->init(size_points + sizeof(t_normal) );
  vertex_buffer->uploadData((void*)t_points, size_points);
  vertex_buffer->uploadData((void*)t_normal, sizeof(t_normal), size_points);

  //Elements
  element_buffer->init(sizeof(vertex_order));
  element_buffer->uploadData((void*)vertex_order, sizeof(vertex_order));
}


Triangler::~Triangler(){
}
