#include <geometry/icosaedro.h>
#include<EDK3/ref_ptr.h>
#include <EDK3/dev/buffer.h>
#include <EDK3/dev/gpumanager.h>
#include <vector_3.h>


Icosaedro::Icosaedro(){
}

bool Icosaedro::bindAttribute(const EDK3::Attribute a, unsigned int where_to_bind_attribute)const {


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

void Icosaedro::render()const{

  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();

  gpu->drawElements(EDK3::dev::GPUManager::kDrawMode_Triangles,
    kNTriangles, element_buffer.get(), EDK3::T_UINT);
}

void Icosaedro::Generate() {

  //hecho de las dos formas para que no se me olviden
  EDK3::dev::GPUManager* gpu = EDK3::dev::GPUManager::Instance();
  EDK3::dev::GPUManager::Instance()->newBuffer(&vertex_buffer);
  gpu->newBuffer(&element_buffer);

  Vector3 g_points[12] = {
  { 0.000000f, -1.000000f,  0.000000f},
  { 0.723600f, -0.447215f,  0.525720f},
  {-0.276385f, -0.447215f,  0.850640f},
  {-0.894425f, -0.447215f,  0.000000f},
  {-0.276385f, -0.447215f, -0.850640f},
  { 0.723600f, -0.447215f, -0.525720f},
  { 0.276385f,  0.447215f,  0.850640f},
  {-0.723600f,  0.447215f,  0.525720f},
  {-0.723600f,  0.447215f, -0.525720f},
  { 0.276385f,  0.447215f, -0.850640f},
  { 0.894425f,  0.447215f,  0.000000f},
  { 0.000000f,  1.000000f,  0.000000f} };


  Vector3 g_normals[] = {
  {  0.1876f, - 0.7947f,   0.5774f},
  {  0.6071f, - 0.7947f,   0.0000f},
  {- 0.4911f, - 0.7947f,   0.3568f},
  {- 0.4911f, - 0.7947f, - 0.3568f},
  {  0.1876f, - 0.7947f, - 0.5774f},
  {  0.9822f, - 0.1876f,   0.0000f},
  {  0.3035f, - 0.1876f,   0.9342f},
  {- 0.7946f, - 0.1876f,   0.5774f},
  {- 0.7946f, - 0.1876f, - 0.5774f},
  {  0.3035f, - 0.1876f, - 0.9342f},
  {  0.7946f,   0.1876f,   0.5774f},
  {- 0.3035f,   0.1876f,   0.9342f},
  {- 0.9822f,   0.1876f,   0.0000f},
  {- 0.3035f,   0.1876f, - 0.9342f},
  {  0.7946f,   0.1876f, - 0.5774f},
  {  0.4911f,   0.7947f,   0.3568f},
  {- 0.1876f,   0.7947f,   0.5774f},
  {- 0.6071f,   0.7947f,   0.0000f},
  {- 0.1876f,   0.7947f, - 0.5774f},
  {  0.4911f,   0.7947f, - 0.3568f}, };

  size_points = sizeof(g_points);

  unsigned int vertex_order[] = { 0,1,2,
                                  0,2,3,
                                  0,3,4,
                                  0,4,5,
                                  0,5,1,

                                  7,2,6,
                                  2,1,6,
                                  6,1,10,
                                  1,5,10,
                                  10,5,9,
                                  5,4,9,
                                  9,4,8,
                                  4,3,8,
                                  8,3,7,
                                  3,2,7,

                                  11,6,10,
                                  11,10,9,
                                  11,9,8,
                                  11,8,7,
                                  11,7,6};

  kNTriangles = sizeof(vertex_order) / sizeof(unsigned int);


  //Vertex
  vertex_buffer->init(sizeof(g_points) + sizeof(g_normals) );
  vertex_buffer->uploadData((void*)g_points, sizeof(g_points));
  vertex_buffer->uploadData((void*)g_normals, sizeof(g_normals), sizeof(g_points));

  //Elements
  element_buffer->init(sizeof(vertex_order));
  element_buffer->uploadData((void*)vertex_order, sizeof(vertex_order));
}


Icosaedro::~Icosaedro(){
}
