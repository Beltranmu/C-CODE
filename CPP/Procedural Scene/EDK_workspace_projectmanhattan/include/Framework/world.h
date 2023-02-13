#ifndef __WORLD_H__
#define __WORLD_H__ 1

#include <EDK3/geometry.h>
#include <Material/Buildingmaterial.h>
#include <EDK3/materialsettings.h>
#include <Mollete/texture.h>
#include <Framework/props.h>
#include <Light/pointligh.h>
#include <Light/directionallight.h>
#include <Light/sportlight.h>
#include <Geometry/point.h>

class World{

public:
  World();
  ~World();

  enum kErrorCode{

    kError_EmptyArray = 0,
    kError_OK,
    kError_ArrayAlreadyCreated,
    kError_ValueIsZero,
    kError_ArrayIsFull,
    kError_InvalidValue,
    kError_NotNull,
    kError_Null

  };

  // Constants
  static const int kNPointLights = 2;
  static const int kNSpotLights = 3;
  static const int kNTextures = 5;

  // Geometry
  int initGeometries(const int value);
  void loadAllGeometries();
  int loadGeometry(const char* filepath);

  // Material
  void initMaterial(const char* vertexShader, const char* fragmentShader);

  // Material Settings
  int initLights();  
  int initDirectionalLight(EDK3::ref_ptr<DirectionalLight> directional_light, const float x, const float y, const float z);  
  void loadTexture(const char* path, EDK3::ref_ptr<Mollete::Texture>* texture);
  void initAllMaterials(const int nMatSettings);
  void initMatSettings(EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> material,
                       const float r, const float g, const float b, const float a,
                       EDK3::ref_ptr<Mollete::Texture> texture, unsigned int nPointLights,
                       unsigned int nSpotLights, float* camPos, float shininess,
                       int* fog, float* fog_d);

  EDK3::ref_ptr<Buildingmaterial> getMaterial();  
  EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> getMatSettings(const int index);
  EDK3::ref_ptr<DirectionalLight> getDirectionalLight();

  // Create City
  void generateCity(EDK3::ref_ptr<EDK3::Node> node, const int width, const int height);
  
  EDK3::ref_ptr<Mollete::Texture> worldTextures[kNTextures];
  PointLight worldPointLights[kNPointLights];
  SpotLight worldSpotLights[kNSpotLights];
  
private:
  
  // Geometry
  EDK3::scoped_array<EDK3::ref_ptr<EDK3::Geometry>> worldGeometries;
  EDK3::ref_ptr<Point> rain;

  // Material
  EDK3::ref_ptr<Buildingmaterial> worldMaterial;

  // Material Settings
  EDK3::scoped_array<EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat>> worldMatSettings;
  
  // Props  
  EDK3::scoped_array<EDK3::ref_ptr<Props>> buildings;
  EDK3::scoped_array<EDK3::ref_ptr<Props>> roads;
  //EDK3::scoped_array<EDK3::ref_ptr<Props>> t_lights;

  // Lights
  EDK3::ref_ptr<DirectionalLight> worldDirectionalLight;

};

#endif // !__WORLD_H__