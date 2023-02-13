#include <Framework/world.h>

World::World(){
}

World::~World(){
}

// Geometry
int World::initGeometries(const int value){

  if (NULL != worldGeometries.get()) {

    return kError_ArrayAlreadyCreated;

  }

  if (0 >= value)
    return kError_ValueIsZero;

  worldGeometries.alloc(value);
  return kError_OK;

}

void World::loadAllGeometries()
{

  loadGeometry("../assets/Models/SM_Building_00.obj");
  loadGeometry("../assets/Models/SM_Building_01.obj");
  loadGeometry("../assets/Models/SM_Building_02.obj");
  loadGeometry("../assets/Models/SM_Building_03.obj");
  loadGeometry("../assets/Models/SM_Building_04.obj");
  loadGeometry("../assets/Models/SM_Building_05.obj");
  loadGeometry("../assets/Models/SM_Road_00.obj");
  loadGeometry("../assets/Models/SM_Road_01.obj");
  loadGeometry("../assets/Models/SM_Road_02.obj");
  loadGeometry("../assets/Models/SM_Road_03.obj");
  loadGeometry("../assets/Models/SM_Road_04.obj");
  loadGeometry("../assets/Models/SM_TrafficL_00.obj");
  loadGeometry("../assets/Models/SM_TrafficL_01.obj");
  loadGeometry("../assets/Models/SM_TrafficS_00.obj");
  loadGeometry("../assets/Models/SM_TrafficS_01.obj");
  loadGeometry("../assets/Models/SM_TrafficS_02.obj");
  loadGeometry("../assets/Models/SM_TrafficS_03.obj");
  loadGeometry("../assets/Models/SM_TrafficS_04.obj");
  loadGeometry("../assets/Models/SM_TrafficS_05.obj");
  loadGeometry("../assets/Models/SM_TrafficS_06.obj");

}

int World::loadGeometry(const char* filepath) {

  int i = 0;
  bool indexFound = false;

  if (NULL == worldGeometries.get())
    return kError_EmptyArray;

  int size = worldGeometries.size();

  if (worldGeometries[size - 1] != NULL)
    return kError_ArrayIsFull;

  EDK3::scoped_array<char> errorLog;
  EDK3::scoped_array<EDK3::ref_ptr<EDK3::Geometry>> geometryReferece;
  LoadObj(filepath, &geometryReferece, &errorLog);  

  do{

    if(worldGeometries[i] == NULL) {
      worldGeometries[i] = geometryReferece[0];
      indexFound = true;
      return kError_OK;
    }

    ++i;

  } while (i < size && !indexFound);

}

// Material
void World::initMaterial(const char* vertexShader, const char* fragmentShader){

  if (NULL != worldMaterial.get())       
    worldMaterial->load_shaders_from_filepaths(vertexShader, fragmentShader);

}

// Lights
int World::initLights()
{

  worldDirectionalLight.alloc();

  for(int i = 0; i < kNPointLights; ++i){
    
    worldPointLights[i].init();

  }

  for (int i = 0; i < kNSpotLights; ++i) {

    worldSpotLights[i].init();

  }

  return kError_OK;

}

int World::initDirectionalLight(EDK3::ref_ptr<DirectionalLight> directional_light, const float x, const float y, const float z)
{

  if (NULL == directional_light.get())
    directional_light.alloc();

  directional_light->init();
  directional_light->set_position(x, y, z);
  return kError_OK;

}

// Texture
void World::loadTexture(const char* path, EDK3::ref_ptr<Mollete::Texture>* texture)
{
  EDK3::Texture::Load(path, (EDK3::ref_ptr<EDK3::Texture>*)texture);
  texture->get()->generateMipmaps();
}

// Material
void World::initAllMaterials(const int nMatSettings)
{

  if(NULL == worldMaterial.get())
    worldMaterial.alloc();

  if (NULL == worldMatSettings.get()) {

    worldMatSettings.alloc(nMatSettings);

    for (int i = 0; i < nMatSettings; ++i) {

      worldMatSettings[i].alloc();

    }
  }
}

void World::initMatSettings(EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> material,
                            const float r, const float g, const float b, const float a,
                            EDK3::ref_ptr<Mollete::Texture> texture, unsigned int nPointLights,
                            unsigned int nSpotLights, float* camPos, float shininess, 
                            int* fog, float* fog_d){
  
  material->set_color(r, g, b, a);
  material->set_texture(texture.get());
  material->d_lights = worldDirectionalLight.get();
  material->p_lights = worldPointLights;
  material->s_lights = worldSpotLights;
  material->n_d_lights = 1;
  material->n_p_lights = nPointLights;
  material->n_s_lights = nSpotLights;
  material->set_cam_pos(camPos);
  material->set_shininess(shininess);

  material->set_fog_active_data(fog);
  material->set_fog_density(fog_d);

}

EDK3::ref_ptr<Buildingmaterial> World::getMaterial()
{
  return worldMaterial;
}

EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> World::getMatSettings(const int index)
{
  return worldMatSettings[index];
}

EDK3::ref_ptr<DirectionalLight> World::getDirectionalLight(){
  return worldDirectionalLight;
}
        
void World::generateCity(EDK3::ref_ptr<EDK3::Node> node, const int width, const int height){
  
  int totalElements = width * height;
  int totalBuildings = (width - 1) * (height - 1);
  int totalTLigths = 1;
  EDK3::scoped_array<EDK3::ref_ptr<EDK3::Drawable>> buildings_;
  EDK3::scoped_array<EDK3::ref_ptr<EDK3::Drawable>> roads_;
  EDK3::scoped_array<EDK3::ref_ptr<EDK3::Drawable>> t_lights;

  buildings_.alloc(totalElements);
  roads_.alloc(totalElements);
  t_lights.alloc(totalTLigths);

  for(int i = 0; i < totalElements; ++i){
            
    roads_[i].alloc();
    node->addChild(roads_[i].get());
    roads_[i]->set_geometry(worldGeometries[6].get());
    roads_[i]->set_material(worldMaterial.get());
    roads_[i]->set_material_settings(worldMatSettings[2].get());
    roads_[i]->set_scale(4.f, 1.f, 4.f);

  }

  for(int i = 0; i < totalBuildings; ++i){

    buildings_[i].alloc();
    node->addChild(buildings_[i].get());
    buildings_[i]->set_geometry(worldGeometries[rand()%2].get());
    buildings_[i]->set_material(worldMaterial.get());
    buildings_[i]->set_material_settings(worldMatSettings[rand()%2].get());

  }

  for (int i = 0; i < totalTLigths; ++i) {

    t_lights[i].alloc();
    node->addChild(t_lights[i].get());
    t_lights[i]->set_geometry(worldGeometries[ 11].get());
    t_lights[i]->set_material(worldMaterial.get());
    t_lights[i]->set_material_settings(worldMatSettings[3].get());
    t_lights[i]->set_scale(0.75f, 0.75f, 0.75f);
    t_lights[i]->set_position(3800.f, 0.0f, 0.0f);
  }
  
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j){
            
      roads_[i*width+j]->set_position(j*4000.f, 0.f, i*4000.f);

    }
  }

  int middleWidth = (width - 1) / 2;
  int middleHeight = (height - 1) / 2;
  float multiplier = 1.f;
  float currentScaleY = 0.0f;
  float currentScaleX = 0.0f;

  for (int i = 0; i < height - 1; ++i) {

    if (i < middleHeight) {
      currentScaleY = multiplier * (i + 1);
    }
    else {
      currentScaleY = ((height - 1) - i) * multiplier;
    }

    for (int j = 0; j < width - 1; ++j) {

      buildings_[i*(width - 1) + j]->set_position(j * 4000.f + 4000.f, 0.f, i * 4000.f);

      if (j < middleWidth) {
        currentScaleX = multiplier * (j + 1);
      }
      else {
        currentScaleX = ((width - 1) - j) * multiplier;
      }

      if (currentScaleX > currentScaleY) {
        buildings_[i*(width - 1) + j]->set_scale(1.0f, currentScaleY, 1.0f);
      }
      else {
        buildings_[i*(width - 1) + j]->set_scale(1.0f, currentScaleX, 1.0f);
      }
    }
  }

}