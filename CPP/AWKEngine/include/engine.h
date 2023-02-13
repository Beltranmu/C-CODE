
/**
 * Engine Class
 * Developed by Javier BA & Ricardo BM
 * Graphic Engine Development
 * ESAT - 2022 */

#ifndef __ENGINE_H__
#define __ENGINE_H__ 1

#include <functional>
#include <material.h>
#include <component.h>
#include <system.h>
#include <memory>
#include <vector>
#include <transformtree.h>
#include <texture.h>

#include <list>

namespace AWK {

  class DisplayList;
  class Window;
  struct ECSData;
  struct EntityHandler;
  struct ECS;

  class Engine {

    public:

      Engine();
      ~Engine();
       
      void init(int argc, char* argv[]); // Essential for the engine

      uint32_t loadObj(const char* path);
      uint32_t createTexture(const char* path, GLint minMagFilter = GL_NEAREST);
      uint32_t createTexture(uint32_t width, uint32_t height, Texture::Type textureType, GLint minMagFilter);
      void setTexturesFBO(uint32_t cT, uint32_t dT);
      void showTextures(uint32_t blitTextureID);
      uint32_t createGeometry(float* vertex_attrib, uint32_t* vertex_indices,  uint32_t size_geometry);

      /** MATERIAL */
      uint32_t createMaterial(AWK::Material::MaterialParameters newMat);
      void changeMaterialColor(uint32_t mat, AWK::Material::RGBA color);
      void changeMaterialTexture(uint32_t mat, uint32_t texture);

      /** ECS */
      void enableSystem(SystemType type);

      std::vector<std::unique_ptr<System>> systems;

      uint32_t blitTextureID, testTextureID;      
      uint32_t textureColor, textureDepth;

      double getTime();     

      // Render Loop - Gets Logic Function from the outside
      void startRenderLoop(std::function<void()> &&logicFunc);
      void submit(DisplayList&& list);
      void renderSubmit(DisplayList&& list , uint32_t colorTexture, uint32_t depthTexture);
      void render();
      void blitTexture();
      void cameraMovement();

      void update();
      void End();
      void printM4(const glm::mat4 m, const char* name);

      /** SCENE */

      /** COMPOSER */
      uint32_t requestTexture(uint32_t width, uint32_t height, Texture::Type type);
      std::list<uint32_t> composerTextureList;

      struct Data;

      struct GPUState;
      struct Material;
      uint32_t root;
      uint32_t currentCamera;
      std::vector<uint32_t> shadowMapTextures;
   
      ECSData* _entityData = nullptr;
      float bgColor[4] = { 0.4f, 0.7f, 0.0f, 1.0f };

      GPUState* _GPUState;
      float sceneSize;
      glm::vec3 boundingBoxScene[8];
      bool onlyOnce;

  private:
    Data* _data = nullptr;
    static uint32_t next_mat_id;
    static uint32_t next_texture_id;
  
    //bool entityHasComponent(const uint32_t entity, const Component::ComponentType ct);
 
  };
}


#endif // !__ENGINE_H__