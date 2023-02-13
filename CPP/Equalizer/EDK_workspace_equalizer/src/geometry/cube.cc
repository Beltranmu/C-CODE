#include <geometry/cube.h>
#include<EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>


Cube::Cube(){
}

bool Cube::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


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

void Cube::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  gpu->drawElements(EDK3::dev::GPUManager::kDrawMode_Triangles,
    kNTriangles, element_buffer.get(), EDK3::T_UINT);
}

void Cube::Generate() {

  //hecho de las dos formas para que no se me olviden
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);
  gpu->newBuffer(&element_buffer);

  float c_positions[] = {
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  
   1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
  
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  
  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f

  };


  float c_normals[] = {
   0.0f, 0.0f, 1.0f,
   0.0f, 0.0f, 1.0f,
   0.0f, 0.0f, 1.0f,
   0.0f, 0.0f, 1.0f,

   1.0f, 0.0f, 0.0f,
   1.0f, 0.0f, 0.0f,
   1.0f, 0.0f, 0.0f,
   1.0f, 0.0f, 0.0f,

   0.0f, 0.0f, -1.0f,
   0.0f, 0.0f, -1.0f,
   0.0f, 0.0f, -1.0f,
   0.0f, 0.0f, -1.0f,

  -1.0f, 0.0f, 0.0f,
  -1.0f, 0.0f, 0.0f,
  -1.0f, 0.0f, 0.0f,
  -1.0f, 0.0f, 0.0f,

   0.0f, 1.0f, 0.0f,
   0.0f, 1.0f, 0.0f,
   0.0f, 1.0f, 0.0f,
   0.0f, 1.0f, 0.0f,

   0.0f, -1.0f, 0.0f,
   0.0f, -1.0f, 0.0f,
   0.0f, -1.0f, 0.0f,
   0.0f, -1.0f, 0.0f

  };
  unsigned int indices[] = { 0,1,2, 2,1,3,
                            4,5,6,  4,6,7,
                            9,8,11, 11,8,10,
                            13,12,15, 13,15,14,
                            16,17,18, 16,18,19,
                            21,20,23, 21,23,22
  };


  kNTriangles = sizeof(indices) / sizeof(unsigned int);


  //Vertex
  vertex_buffer->init(sizeof(c_positions) + sizeof(c_normals) );
  vertex_buffer->uploadData((void*)c_positions, sizeof(c_positions));
  vertex_buffer->uploadData((void*)c_normals, sizeof(c_normals), sizeof(c_positions));

  //Elements
  element_buffer->init(sizeof(indices));
  element_buffer->uploadData((void*)indices, sizeof(indices));
}

void Cube::Update(float deformation, float deformation2){

 //deformation /= 10.0f;
  if (deformation > 1.57f) {
    deformation = 1.57f;
  }

  if (deformation2 > 1.57f) {
    deformation2 = 1.57f;
  }
  float deformation_cos = cosf(deformation) * 2.0f;
  float deformation_sin = sinf(deformation) * 2.0f;

  float deformation_cos2 = cosf(deformation) * 2.0f;
  float deformation_sin2 = sinf(deformation) * 2.0f;


  float c_positions[] = {
    -1.0f, -1.0f + deformation_sin,  1.0f + deformation_sin,
     1.0f, -1.0f + deformation_sin,  1.0f + deformation_sin,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

     1.0f + deformation_sin , -1.0f + deformation_sin,  1.0f,//
     1.0f + deformation_sin , -1.0f + deformation_sin, -1.0f,//
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,

    -1.0f, -1.0f + deformation_sin, -1.0f - deformation_sin,
     1.0f, -1.0f + deformation_sin, -1.0f - deformation_sin,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,

    -1.0f - deformation_sin , -1.0f + deformation_sin ,  1.0f,//
    -1.0f - deformation_sin , -1.0f + deformation_sin , -1.0f,//
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,

    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f

  };



  float c_normals[] = {
  0.0f, deformation_sin * 0.5f, deformation_cos * 0.5f, //
  0.0f, deformation_sin * 0.5f, deformation_cos * 0.5f, //
  0.0f, deformation_sin * 0.5f, deformation_cos * 0.5f, //
  0.0f, deformation_sin * 0.5f, deformation_cos * 0.5f, //
                    
  deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
  deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
  deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
  deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
                    
  0.0f, deformation_sin * 0.5f, -deformation_cos * 0.5f,//
  0.0f, deformation_sin * 0.5f, -deformation_cos * 0.5f,//
  0.0f, deformation_sin * 0.5f, -deformation_cos * 0.5f,//
  0.0f, deformation_sin * 0.5f, -deformation_cos * 0.5f,//
                    
 -deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
 -deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
 -deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //
 -deformation_cos * 0.5f, deformation_sin * 0.5f, 0.0f, //

  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,

  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f

  };
  
  vertex_buffer->uploadData((void*)c_positions, sizeof(c_positions));
  vertex_buffer->uploadData((void*)c_normals, sizeof(c_normals), sizeof(c_positions));

}


Cube::~Cube(){
}
