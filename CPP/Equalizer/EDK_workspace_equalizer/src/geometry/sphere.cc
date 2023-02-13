#include <geometry/sphere.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>


const int kNCirclePoints = 30;
const int kNSpherePoints = kNCirclePoints * kNCirclePoints;

Sphere::Sphere(){
}

bool Sphere::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


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

void Sphere::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  gpu->drawElements(EDK3::dev::GPUManager::kDrawMode_Triangles,
    kNTriangles, element_buffer.get(), EDK3::T_UINT);
}

void Sphere::Generate(){

  //hecho de las dos formas para que no se me olviden
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);
  gpu->newBuffer(&element_buffer);

  Vector3 g_points[kNSpherePoints]; 
  Vector3 g_normal[kNSpherePoints]; 
  float angle = (3.141592f) / (kNCirclePoints -1)  ;
  
  unsigned int vertex_order[(kNSpherePoints-1) * 6];



  //0 -> kNSpherePoints
  for (int i = 0; i < kNSpherePoints; ++i) {
    int new_i = i % kNCirclePoints;
    float radius = sinf(angle * i / kNCirclePoints);
    g_points[i].x = cosf(angle * 2.0f * new_i) * radius;
    g_points[i].y = cosf(angle * (int)(i / kNCirclePoints));
    g_points[i].z = sinf(angle * 2.0f * new_i) * radius;



    printf("%d  ", i);
    printf("(%06f,%06f,%06f)\n", g_points[i].x, g_points[i].y, g_points[i].z);
    if (i < (kNSpherePoints - kNCirclePoints)) {

      vertex_order[i * 6 + 0] = i;
      vertex_order[i * 6 + 3] = i;
      vertex_order[i * 6 + 4] = i + 30 ;

      if ((i + 1) % kNCirclePoints == 0) {
       vertex_order[i * 6 + 1] = i + 1;
       vertex_order[i * 6 + 2] = i - 29;
       vertex_order[i * 6 + 5] = i + 1;  
      }
      else {
       vertex_order[i * 6 + 1] = i + 31;
       vertex_order[i * 6 + 2] = i + 1;
       vertex_order[i * 6 + 5] = i + 31;
      }
      /*printf("T1 : (%d,%d,%d)\n", vertex_order[i * 6 + 0],
        vertex_order[i * 6 + 1],
        vertex_order[i * 6 + 2]);
      printf("T2 : (%d,%d,%d)\n", vertex_order[i * 6 + 3],
        vertex_order[i * 6 + 4],
        vertex_order[i * 6 + 5]);*/
    }

   
    g_normal[i] = g_points[i];
   // printf("(%f,%f,%f)\n", g_normal[i].x, g_normal[i].y, g_normal[i].z);

    if (i%kNCirclePoints == 29) {
      printf("\n");
    }
  }
 

  size_points = sizeof(g_points);

  kNTriangles = sizeof(vertex_order) / sizeof(unsigned int);
  //Vertex
  vertex_buffer->init(size_points + sizeof(g_normal) );
  vertex_buffer->uploadData((void*)g_points, size_points);
  vertex_buffer->uploadData((void*)g_normal, sizeof(g_normal), size_points);

  //Elements
  element_buffer->init(sizeof(vertex_order));
  element_buffer->uploadData((void*)vertex_order, sizeof(vertex_order));
}


Sphere::~Sphere(){
}
