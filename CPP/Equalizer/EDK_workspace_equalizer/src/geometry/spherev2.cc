#include <geometry/spherev2.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>


const int kNCirclePoints = 30;
const int kNSpherePoints = kNCirclePoints * kNCirclePoints;

Sphere2::Sphere2(){
}

bool Sphere2::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


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

void Sphere2::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  gpu->drawElements(EDK3::dev::GPUManager::kDrawMode_Triangles,
    kNTriangles, element_buffer.get(), EDK3::T_UINT);
}

void Sphere2::Generate(){

  //hecho de las dos formas para que no se me olviden
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);
  gpu->newBuffer(&element_buffer);

  float g_points[kNSpherePoints * 3]; 
  //float g_normal[kNSpherePoints * 3]; 
  float *worker = g_points;

  float angle1 = (3.141592f) / (kNCirclePoints -1)  ;
  float angle = ((3.141592f) / (kNCirclePoints -1)) *2.0f  ;
  
  unsigned int vertex_order[(kNSpherePoints-1) * 6];


  for (int i = 0; i < kNCirclePoints; ++i) {

    float radius = sinf(angle1 * i);
    for (int j = 0; j < kNCirclePoints; ++j) {

      *worker = cosf(angle* /* 2.0f */ j) * radius;
      ++worker;
      *worker = sinf(angle* /* 2.0f */ j) * radius;
      ++worker;
      *worker = cosf(angle1 *i);
      ++worker;
     /*
      if (i < (kNCirclePoints - 1)) {
          vertex_order[i * 30 + j * 6 + 0] = i;
          vertex_order[i * 30 + j * 6 + 3] = i;
          vertex_order[i * 30 + j * 6 + 4] = i + 30;

        if ((i + 1) % kNCirclePoints == 0) {
          vertex_order[i * 30 + j * 6 + 1] = i + 1;
          vertex_order[i * 30 + j * 6 + 2] = i - 29;
          vertex_order[i * 30 + j * 6 + 5] = i + 1;
        }
        else {
          vertex_order[i * 30 + j * 6 + 1] = i + 31;
          vertex_order[i * 30 + j * 6 + 2] = i + 1;
          vertex_order[i * 30 + j * 6 + 5] = i + 31;
        }

      }*/

    }
  }

  for (int i = 0; i < kNSpherePoints; ++i) {

    //printf("(%f,%f,%f)\n", g_points[i * 3], g_points[i * 3 + 1], g_points[i * 3 + 2]);
   
    if (i%kNCirclePoints == 29) {
      printf("\n");
    }
  }
  

  //0 -> kNSpherePoints
  for (int i = 0; i < kNSpherePoints; ++i) {

    if (i < (kNSpherePoints - kNCirclePoints) && i%kNCirclePoints !=29) {

      vertex_order[i * 6 + 0] = i;
      vertex_order[i * 6 + 3] = i;
      vertex_order[i * 6 + 4] = i + kNCirclePoints;

      if ((i + 1) % kNCirclePoints == 0) {
       vertex_order[i * 6 + 1] = i + 1;
       vertex_order[i * 6 + 2] = i - kNCirclePoints -1;
       vertex_order[i * 6 + 5] = i + 1;  
      }
      else {
       vertex_order[i * 6 + 1] = i + kNCirclePoints + 1 ;
       vertex_order[i * 6 + 2] = i + 1;
       vertex_order[i * 6 + 5] = i + kNCirclePoints + 1;
      }
      /*printf("T1 : (%d,%d,%d)\n", vertex_order[i * 6 + 0],
        vertex_order[i * 6 + 1],
        vertex_order[i * 6 + 2]);
      printf("T2 : (%d,%d,%d)\n", vertex_order[i * 6 + 3],
        vertex_order[i * 6 + 4],
        vertex_order[i * 6 + 5]);*/
    }

   

   // printf("(%f,%f,%f)\n", g_normal[i].x, g_normal[i].y, g_normal[i].z);

    if (i%kNCirclePoints == 29) {
      printf("\n");
    }
  }
 

  size_points = sizeof(g_points);

  kNTriangles = sizeof(vertex_order) / sizeof(unsigned int);
  //Vertex
  vertex_buffer->init(size_points + sizeof(g_points) );
  vertex_buffer->uploadData((void*)g_points, size_points);
  vertex_buffer->uploadData((void*)g_points, sizeof(g_points), size_points);

  //Elements
  element_buffer->init(sizeof(vertex_order));
  element_buffer->uploadData((void*)vertex_order, sizeof(vertex_order));
}


Sphere2::~Sphere2(){
}
