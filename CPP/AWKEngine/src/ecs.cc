 
 #include "ecs.h" 
 #include "engine.h"
 #include "internals.h"

namespace AWK {

  ECSData::ECSData() {
    auto&& ecsTransaciton = transaction();
    LvlStructure rootLvL;
    rootLvL.entityStart = 0;
    rootLvL.numberOfEntities = 1;
    rootLvL.parentStart = 0;
    transformTree.numberEntitiesLvL.push_back(rootLvL);
  }

  uint32_t ECSData::Transaction::createEntity(uint64_t mask)
  {
    std::shared_ptr<EntityHandler> new_entity = std::make_shared<EntityHandler>();
    uint32_t newEntityID = (uint32_t)_ecs->entitiesMap[mask].size();
    uint32_t newUserEntityID = (uint32_t)_ecs->entities.size();
    new_entity->componentMask = mask;
    new_entity->id = newUserEntityID;
    new_entity->mapID = newEntityID;
    uint64_t aux_mask = mask;

    _ecs->entitiesMap[mask].push_back(new_entity);
    _ecs->entities.push_back(new_entity);

    //Add components
    for (uint64_t componentID = 0; componentID < Component::ComponentType::MAX; componentID++) {

      if ((aux_mask & 1) == 1) {

        switch (componentID) {

        case 1: addRenderComponent(newUserEntityID, false); break;
        case 2: addTRSComponent(newUserEntityID, false); break;
        case 3: addLocalTransformComponent(newUserEntityID, false); break;
        case 4: addWorldTransformComponent(newUserEntityID, false); break;
        case 5: addNodeComponent(newUserEntityID, false); break;
        case 6: addCameraComponent(newUserEntityID, false); break;
        case 7: addVPComponent(newUserEntityID, false); break;
        case 8: addSunComponent(newUserEntityID, false); break;
        case 9: addFlyCameraComponent(newUserEntityID, false); break;
        case 10: addLightComponent(newUserEntityID, false); break;

        }
      }
      aux_mask >>= 1;
    }

    return newUserEntityID;
  }

  bool ECSData::Transaction::entityHasComponent(const uint32_t entity, const Component::ComponentType ct) {

    if (_ecs->entities.size() == 0) {
      return false;
    }
    return _ecs->entities[entity]->components[ct] != 0;
  }


  RenderComponent* ECSData::Transaction::addRenderComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::RENDER;

    if (!entityHasComponent(entity, ct)) {
      RenderComponent newComponent;
      newComponent.materialID = _ecs->defaultMaterial;
      newComponent.meshID = _ecs->defaultObj;
      _ecs->renderComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->renderComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_RENDER;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID = 
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->renderComponents[(uint32_t)_ecs->renderComponents.size() - 1];
  }

  RenderComponent* ECSData::Transaction::getRenderComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::RENDER;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->renderComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeRenderComponent(const uint32_t entity) {
    const Component::ComponentType ct = Component::ComponentType::RENDER;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_RENDER;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t) _ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t) _ecs->renderComponents.size() - 1;

    _ecs->renderComponents[newComponentID] = _ecs->renderComponents.back();
    _ecs->renderComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  TRSComponent* ECSData::Transaction::addTRSComponent(const uint32_t entity, bool refreshEntityMap)
  {
    const Component::ComponentType ct = Component::ComponentType::TRS;

    if (!entityHasComponent(entity, ct)) {
      TRSComponent newComponent;
      newComponent.translation = glm::vec3(0.0f, 0.0f, 0.0f);
      newComponent.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
      newComponent.scale = glm::vec3(1.0f, 1.0f, 1.0f);
      _ecs->trsComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->trsComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_TRS;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID  =
       (uint32_t) _ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return  &_ecs->trsComponents[(uint32_t)_ecs->trsComponents.size() - 1];;
  }

  TRSComponent* ECSData::Transaction::getTRSComponent(const uint32_t entity) {
    const Component::ComponentType ct = Component::ComponentType::TRS;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->trsComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeTRSComponent(const uint32_t entity){
    const Component::ComponentType ct = Component::ComponentType::TRS;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_TRS;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->trsComponents.size() - 1;

    _ecs->trsComponents[newComponentID] = _ecs->trsComponents.back();
    _ecs->trsComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  LocalTransformComponent* ECSData::Transaction::addLocalTransformComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::LOCAL_TRANSFORM;

    if (!entityHasComponent(entity, ct)) {

      LocalTransformComponent newComponent;
      newComponent.localMatrix = glm::mat4(1.0f);
      _ecs->localTransformComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->localTransformComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_LOCAL_TRANSFORM;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->localTransformComponents[(uint32_t)_ecs->localTransformComponents.size() - 1];;
  }

  LocalTransformComponent* ECSData::Transaction::getLocalTransformComponent(const uint32_t entity) {
    const Component::ComponentType ct = Component::ComponentType::LOCAL_TRANSFORM;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->localTransformComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeLocalTransformComponent(const uint32_t entity){
    const Component::ComponentType ct = Component::ComponentType::LOCAL_TRANSFORM;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_LOCAL_TRANSFORM;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->localTransformComponents.size() - 1;
    //RenderComponent oldRenderComponent = _ecs->localTransformComponents[newComponentID];

    _ecs->localTransformComponents[newComponentID] = _ecs->localTransformComponents.back();
    _ecs->localTransformComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }

  }


  WorldTransformComponent* ECSData::Transaction::addWorldTransformComponent(const uint32_t entity, bool refreshEntityMap)
  {
    const Component::ComponentType ct = Component::ComponentType::WORLD_TRANSFORM;

    if (!entityHasComponent(entity, ct)) {

      WorldTransformComponent newComponent;
      newComponent.worldMatrix = glm::mat4(1.0f);
      _ecs->worldTransformComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->worldTransformComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_WORLD_TRANSFORM;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->worldTransformComponents[(uint32_t)_ecs->worldTransformComponents.size() - 1];;
  }

  WorldTransformComponent* ECSData::Transaction::getWorldTransformComponent(const uint32_t entity)
  {

    const Component::ComponentType ct = Component::ComponentType::WORLD_TRANSFORM;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->worldTransformComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeWorldTransformComponent(const uint32_t entity){

    const Component::ComponentType ct = Component::ComponentType::WORLD_TRANSFORM;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_WORLD_TRANSFORM;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->worldTransformComponents.size() - 1;

    _ecs->worldTransformComponents[newComponentID] = _ecs->worldTransformComponents.back();
    _ecs->worldTransformComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  NodeComponent* ECSData::Transaction::addNodeComponent(const uint32_t entity, bool refreshEntityMap)
  {
    const Component::ComponentType ct = Component::ComponentType::NODE;

    if (!entityHasComponent(entity, ct)) {

      NodeComponent newComponent;
      newComponent.parent = -1;
      newComponent.indexparent = -1;
      newComponent.index = _ecs->transformTree.numberEntitiesLvL[0].numberOfEntities++;
      newComponent.lvl = 0;
      _ecs->nodeComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->nodeComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_NODE;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->nodeComponents[(uint32_t)_ecs->nodeComponents.size() - 1];;
  }

  NodeComponent* ECSData::Transaction::getNodeComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::NODE;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->nodeComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeNodeComponent(const uint32_t entity){

    const Component::ComponentType ct = Component::ComponentType::NODE;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_NODE;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->nodeComponents.size() - 1;
    

    _ecs->nodeComponents[newComponentID] = _ecs->nodeComponents.back();
    _ecs->nodeComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  CameraComponent* ECSData::Transaction::addCameraComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::CAMERA;

    if (!entityHasComponent(entity, ct)) {

      CameraComponent newComponent;
  
      _ecs->cameraComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->cameraComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_CAMERA;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->cameraComponents[(uint32_t)_ecs->cameraComponents.size() - 1];;
  }

  CameraComponent* ECSData::Transaction::getCameraComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::CAMERA;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->cameraComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeCameraComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::CAMERA;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }


    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_CAMERA;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->cameraComponents.size() - 1;


    _ecs->cameraComponents[newComponentID] = _ecs->cameraComponents.back();
    _ecs->cameraComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  ViewProjectionComponent* ECSData::Transaction::addVPComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::VIEWPROJECTION;

    if (!entityHasComponent(entity, ct)) {

      ViewProjectionComponent newComponent;
      newComponent.viewMatrix = glm::mat4(1.0f);
      newComponent.proyectionMatrix = glm::mat4(1.0f);
      _ecs->VPComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->VPComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_VIEWPROJECTION;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->VPComponents[(uint32_t)_ecs->VPComponents.size() - 1];;
  }

  ViewProjectionComponent* ECSData::Transaction::getVPComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::VIEWPROJECTION;

    if (!entityHasComponent(entity, ct)) { return nullptr; }
    return &_ecs->VPComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeVPComponent(const uint32_t entity){

    const Component::ComponentType ct = Component::ComponentType::VIEWPROJECTION;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }

    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_VIEWPROJECTION;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->VPComponents.size() - 1;


    _ecs->VPComponents[newComponentID] = _ecs->VPComponents.back();
    _ecs->VPComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  LightComponent* ECSData::Transaction::addLightComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::LIGHT;

    if (!entityHasComponent(entity, ct)) {

      LightComponent newComponent;
      newComponent.lightType = LightComponent::Type::Directional;
      newComponent.r = 1.f;
      newComponent.g = 1.f;
      newComponent.b = 1.f;
      newComponent.lightWidth = 0.5f;
      newComponent.intensity = 100.0f;
      _ecs->lightComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->lightComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_LIGHT;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->lightComponents[(uint32_t)_ecs->lightComponents.size() - 1];;
  }

  LightComponent* ECSData::Transaction::getLightComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::LIGHT;

    if (!entityHasComponent(entity, ct)) { return nullptr; }

    return &_ecs->lightComponents[_ecs->entities[entity]->components[ct]];
  }
  
  void ECSData::Transaction::removeLightComponent(const uint32_t entity){

    const Component::ComponentType ct = Component::ComponentType::LIGHT;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }

    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_LIGHT;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->lightComponents.size() - 1;


    _ecs->lightComponents[newComponentID] = _ecs->lightComponents.back();
    _ecs->lightComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  FlyCameraMovementComponent* ECSData::Transaction::addFlyCameraComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::FLY_CAMERA;

    if (!entityHasComponent(entity, ct)) {

      FlyCameraMovementComponent newComponent;

      _ecs->flyCameraComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->flyCameraComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_FLY_CAMERA;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->flyCameraComponents[(uint32_t)_ecs->flyCameraComponents.size() - 1];
  }

  FlyCameraMovementComponent* ECSData::Transaction::getFlyCameraComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::FLY_CAMERA;

    if (!entityHasComponent(entity, ct)) { return nullptr; }

    return &_ecs->flyCameraComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeFlyCameraComponent(const uint32_t entity) {

    const Component::ComponentType ct = Component::ComponentType::FLY_CAMERA;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }

    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_FLY_CAMERA;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->flyCameraComponents.size() - 1;


    _ecs->flyCameraComponents[newComponentID] = _ecs->flyCameraComponents.back();
    _ecs->flyCameraComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }



  SunComponent* ECSData::Transaction::addSunComponent(const uint32_t entity, bool refreshEntityMap) {

    const Component::ComponentType ct = Component::ComponentType::SUN;

    if (!entityHasComponent(entity, ct)) {

      SunComponent newComponent;

      _ecs->sunComponents.push_back(newComponent);
      _ecs->entities[entity]->components[ct] = (uint32_t)_ecs->sunComponents.size() - 1;
    }

    if (refreshEntityMap) {

      EntityHandler swapEntity;

      uint64_t previousMask = _ecs->entities[entity]->componentMask;
      uint32_t entityMapID = _ecs->entities[entity]->mapID;
      uint32_t entityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

      if (_ecs->entitiesMap[previousMask].size() != 1) {

        _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[entityVectorIDToSwap];
        _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
        _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

      }

      _ecs->entitiesMap[previousMask].pop_back();

      _ecs->entities[entity]->componentMask |= Component::AddComponentType::ADD_SUN;
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;

    }

    return &_ecs->sunComponents[(uint32_t)_ecs->sunComponents.size() - 1];
  }

  SunComponent* ECSData::Transaction::getSunComponent(const uint32_t entity)
  {
    const Component::ComponentType ct = Component::ComponentType::SUN;

    if (!entityHasComponent(entity, ct)) { return nullptr; }

    return &_ecs->sunComponents[_ecs->entities[entity]->components[ct]];
  }

  void ECSData::Transaction::removeSunComponent(const uint32_t entity) {

    const Component::ComponentType ct = Component::ComponentType::SUN;
    // Entity Hasn't got that component
    if (!entityHasComponent(entity, ct)) {
      return;
    }

    EntityHandler swapEntity;

    uint64_t previousMask = _ecs->entities[entity]->componentMask;
    uint32_t entityMapID = _ecs->entities[entity]->mapID;
    uint32_t lastEntityVectorIDToSwap = _ecs->entitiesMap[previousMask].back()->id;

    if (_ecs->entitiesMap[previousMask].size() != 1) {

      _ecs->entitiesMap[previousMask][entityMapID] = _ecs->entities[lastEntityVectorIDToSwap];
      _ecs->entitiesMap[previousMask][entityMapID]->mapID = entityMapID;
      _ecs->entitiesMap[previousMask].back() = _ecs->entities[entity];

    }

    _ecs->entitiesMap[previousMask].pop_back();

    _ecs->entities[entity]->componentMask &= ~Component::AddComponentType::ADD_SUN;
    if (_ecs->entities[entity]->componentMask != 0) {
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].push_back(_ecs->entities[entity]);
      _ecs->entitiesMap[_ecs->entities[entity]->componentMask].back()->mapID =
        (uint32_t)_ecs->entitiesMap[_ecs->entities[entity]->componentMask].size() - 1;
    }


    //Erase Component
    uint32_t newComponentID = _ecs->entities[entity]->components[ct];
    _ecs->entities[entity]->components[ct] = 0;
    uint32_t old = (uint32_t)_ecs->sunComponents.size() - 1;


    _ecs->sunComponents[newComponentID] = _ecs->sunComponents.back();
    _ecs->sunComponents.pop_back();
    bool changedDone = false;
    for (int i = 0; i < _ecs->entities.size() && !changedDone; ++i) {
      if (_ecs->entities[i]->components[ct] == old) {
        changedDone = true;
        _ecs->entities[i]->components[ct] = newComponentID;

      }
    }
  }


  void ECSData::Transaction::setActiveCamera(const uint32_t camera) {

  }

  void ECSData::Transaction::addChild(const uint32_t parentEntity, const uint32_t childEntity) {

    NodeComponent* parentNode = getNodeComponent(parentEntity);
    NodeComponent* childNode = getNodeComponent(childEntity);

    if (_ecs->transformTree.numberEntitiesLvL.size() == (parentNode->lvl + 1)) {
      LvlStructure newLvL;
      newLvL.entityStart = 0;
      newLvL.numberOfEntities = 0;
      newLvL.parentStart = 0;
      _ecs->transformTree.numberEntitiesLvL.push_back(newLvL);
    }

    childNode->lvl = parentNode->lvl + 1;
    childNode->index = _ecs->transformTree.numberEntitiesLvL[childNode->lvl].numberOfEntities++;

    childNode->indexparent = parentNode->index;
    childNode->parent = parentEntity;
  }

}