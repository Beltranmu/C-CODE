
/************************************************
 * ECS
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __ECS_H__
#define __ECS_H__ 1

#include <stdint.h>
#include <mutex>
#include <vector>
#include <map>
#include "component.h"
#include "transformtree.h"
#include "camera.h"

namespace AWK {

  struct ECSData {

    private:
      std::mutex mutex;

    public:
      ECSData();

      struct Transaction {

        Transaction(ECSData* ecs) : _ecs(ecs) {
          
          _ecs->mutex.lock();

        }

        ~Transaction() {

          _ecs->mutex.unlock();

        }

        ECSData* _ecs;
        uint32_t createEntity(uint64_t mask = Component::AddComponentType::ADD_ALL);
    
        RenderComponent* addRenderComponent(const uint32_t entity, bool refreshEntityMap = true);
        RenderComponent* getRenderComponent(const uint32_t entity);
        void removeRenderComponent(const uint32_t entity);
        
        TRSComponent* addTRSComponent(const uint32_t entity, bool refreshEntityMap = true);
        TRSComponent* getTRSComponent(const uint32_t entity);
        void removeTRSComponent(const uint32_t entity);

        LocalTransformComponent* addLocalTransformComponent(const uint32_t entity, bool refreshEntityMap = true);
        LocalTransformComponent* getLocalTransformComponent(const uint32_t entity);
        void removeLocalTransformComponent(const uint32_t entity);

        WorldTransformComponent* addWorldTransformComponent(const uint32_t entity, bool refreshEntityMap = true);
        WorldTransformComponent* getWorldTransformComponent(const uint32_t entity);
        void removeWorldTransformComponent(const uint32_t entity);

        NodeComponent* addNodeComponent(const uint32_t entity, bool refreshEntityMap = true);
        NodeComponent* getNodeComponent(const uint32_t entity);
        void removeNodeComponent(const uint32_t entity);

        CameraComponent* addCameraComponent(const uint32_t entity, bool refreshEntityMap = true);
        CameraComponent* getCameraComponent(const uint32_t entity);
        void removeCameraComponent(const uint32_t entity);

        ViewProjectionComponent* addVPComponent(const uint32_t entity, bool refreshEntityMap = true);
        ViewProjectionComponent* getVPComponent(const uint32_t entity);
        void removeVPComponent(const uint32_t entity);

        LightComponent* addLightComponent(const uint32_t entity, bool refreshEntityMap = true);
        LightComponent* getLightComponent(const uint32_t entity);
        void removeLightComponent(const uint32_t entity);

        FlyCameraMovementComponent* addFlyCameraComponent(const uint32_t entity, bool refreshEntityMap = true);
        FlyCameraMovementComponent* getFlyCameraComponent(const uint32_t entity);
        void removeFlyCameraComponent(const uint32_t entity);

        SunComponent* addSunComponent(const uint32_t entity, bool refreshEntityMap = true);
        SunComponent* getSunComponent(const uint32_t entity);
        void removeSunComponent(const uint32_t entity);

        void setActiveCamera(const uint32_t camera);
        void addChild(const uint32_t parentEntity, const uint32_t childEntity);

        //void removeEntity(const uint32_t entity);

        private:
          bool entityHasComponent(const uint32_t entity, const Component::ComponentType ct);
      };

    Transaction transaction() { return Transaction(this); }

    //ECS
    std::vector<std::shared_ptr<EntityHandler>> entities;
    std::map<uint64_t, std::vector<std::shared_ptr<EntityHandler>>> entitiesMap;
    std::vector<RenderComponent> renderComponents;
    std::vector<TRSComponent> trsComponents;
    std::vector<LocalTransformComponent> localTransformComponents;
    std::vector<WorldTransformComponent> worldTransformComponents;
    std::vector<NodeComponent> nodeComponents;
    std::vector<CameraComponent> cameraComponents;
    std::vector<ViewProjectionComponent> VPComponents;
    std::vector<LightComponent> lightComponents;
    std::vector<FlyCameraMovementComponent> flyCameraComponents;
    std::vector<SunComponent> sunComponents;
    uint32_t numberActivatedSystems;

    TransformTree transformTree;
    uint32_t camera;
    uint32_t lights[4];

    uint32_t defaultObj;
    uint32_t defaultMaterial;
    
  };

}

#endif // !__ECS_H__