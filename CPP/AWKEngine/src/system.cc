
#include <system.h>
#include <engine.h>
#include "internals.h"
#include <displayList.h>
#include <gtx/transform.hpp>
#include <ecs.h>

namespace AWK{

  System::System(){
    presentmask = 0;
    missingmask = 0;
  }

  void System::init() {
    for (int i = 0; i < Component::MAX; ++i) {
      if (readComponents[i] == ComponentState::PRESENT
        || writeComponents[i] == ComponentState::PRESENT) {
        presentmask += (uint64_t) 1 << i;
      }
      if (readComponents[i] == ComponentState::MISSING ||
        writeComponents[i] == ComponentState::MISSING) {
        missingmask += (uint64_t) 1 << i;
      }
    }
  }
  
  RenderSystem::RenderSystem() {

    dl = new DisplayList();
    readComponents[Component::RENDER] = ComponentState::PRESENT;
    readComponents[Component::WORLD_TRANSFORM] = ComponentState::PRESENT;
    type = RENDER_SYSTEM;
    activeCamera = nullptr;
  }
  
  void RenderSystem::begin(Engine* engine) {

    auto&& ECSTransaction = engine->_entityData->transaction();
    ViewProjectionComponent* vpComp = ECSTransaction.getVPComponent(engine->_entityData->camera);

    const glm::mat4 viewMatrix = vpComp->viewMatrix;
    const glm::mat4 projectionMatrix = vpComp->proyectionMatrix;
    dl->clear(engine->bgColor[0], engine->bgColor[1], engine->bgColor[2], engine->bgColor[3]).setCamera(viewMatrix, projectionMatrix);
    if(engine->onlyOnce){
      dl->drawWater(viewMatrix, projectionMatrix);
     // engine->onlyOnce = false;
    }
  }

  void RenderSystem::beginShadow(Engine* engine, uint32_t lightIndex) {

    
    auto&& ECSTransaction = engine->_entityData->transaction();
    uint32_t entity = engine->_entityData->lights[lightIndex];
    ViewProjectionComponent* vpComp = ECSTransaction.getVPComponent(engine->_entityData->lights[lightIndex]);

    const glm::mat4 viewMatrix = vpComp->viewMatrix;
    const glm::mat4 projectionMatrix = vpComp->proyectionMatrix;
    dl->clear(engine->bgColor[0], engine->bgColor[1], engine->bgColor[2], engine->bgColor[3]).setCamera(viewMatrix, projectionMatrix);
    engine->_GPUState->frameData.shadows.MVL[lightIndex] = viewMatrix;
    engine->_GPUState->frameData.shadows.MPL[lightIndex] = projectionMatrix;

    LightComponent* light = ECSTransaction.getLightComponent(entity);
    LocalTransformComponent* transform = ECSTransaction.getLocalTransformComponent(entity);
    glm::mat4 m = transform->localMatrix;

    engine->_GPUState->frameData.lights.colorAndIntensity[lightIndex] = glm::vec4(light->r, light->g, light->b, light->intensity);
    engine->_GPUState->frameData.lights.directionAndCutoff[lightIndex] = glm::vec4(light->direction, light->cutOff);
    engine->_GPUState->frameData.lights.positionAndType[lightIndex] = glm::vec4(m[3].x, m[3].y, m[3].z, light->lightType);
    engine->_GPUState->frameData.lights.lightProperties[lightIndex] = glm::vec4(light->lightWidth, engine->_GPUState->minBias, engine->_GPUState->maxBias, 0.f);


  }
  
  void RenderSystem::execute(Engine* engine, const uint32_t entity){

    auto&& ECSTransaction = engine->_entityData->transaction();
    RenderComponent* render = ECSTransaction.getRenderComponent(entity);
    WorldTransformComponent* tr = ECSTransaction.getWorldTransformComponent(entity);

    dl->drawGeometry(render->meshID, tr->worldMatrix, render->materialID);
  }

  void RenderSystem::executeShadow(Engine* engine, const uint32_t entity) {

    auto&& ECSTransaction = engine->_entityData->transaction();
    RenderComponent* render = ECSTransaction.getRenderComponent(entity);
    WorldTransformComponent* tr = ECSTransaction.getWorldTransformComponent(entity);

    dl->drawGeometry(render->meshID, tr->worldMatrix, engine->_entityData->defaultMaterial);
  }
  
  void RenderSystem::end(Engine* engine){
    dl->displayShadows();
    engine->renderSubmit(std::move(*dl), engine->textureColor, engine->textureDepth);
  }

  RenderSystem::~RenderSystem() { delete dl; }

  TransformSystem::TransformSystem() {

    writeComponents[Component::LOCAL_TRANSFORM] = ComponentState::PRESENT;
    readComponents[Component::TRS] = ComponentState::PRESENT;
    type = TRANSFORM_SYSTEM;

  }

  void TransformSystem::execute(Engine* engine, const uint32_t entity) {

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* lrs = ECSTransaction.getTRSComponent(entity);
    LocalTransformComponent* tr = ECSTransaction.getLocalTransformComponent(entity);
    glm::mat4 model;

    model = glm::translate(lrs->translation);
    model *= glm::rotate(lrs->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model *= glm::rotate(lrs->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model *= glm::rotate(lrs->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model *= glm::scale(lrs->scale);
    tr->localMatrix = model;

  }

  TransformTreeSystem::TransformTreeSystem()  {
    readComponents[Component::NODE] = ComponentState::PRESENT;
    readComponents[Component::LOCAL_TRANSFORM] = ComponentState::PRESENT;
    type = TRANSFORM_TREE_SYSTEM;
    numberElements = 0;
  }

  void TransformTreeSystem::begin(Engine* engine){
    numberElements = 0;
    for (int i = 0; i < engine->_entityData->transformTree.numberEntitiesLvL.size(); ++i) {
      engine->_entityData->transformTree.numberEntitiesLvL[i].entityStart = numberElements;
      numberElements += engine->_entityData->transformTree.numberEntitiesLvL[i].numberOfEntities;
      if (i == 0) {
        engine->_entityData->transformTree.numberEntitiesLvL[i].parentStart = -1;
      }
      else {
        engine->_entityData->transformTree.numberEntitiesLvL[i].parentStart = engine->_entityData->transformTree.numberEntitiesLvL[i-1].entityStart;
      }

      //Create the vector
    } 
    engine->_entityData->transformTree.AAA.resize(numberElements);
  }

  void TransformTreeSystem::execute(Engine* engine, const uint32_t entity){

      auto&& ECSTransaction = engine->_entityData->transaction();
      NodeComponent* node = ECSTransaction.getNodeComponent(entity);
      LocalTransformComponent* local_tr = ECSTransaction.getLocalTransformComponent(entity);
      // Primer pasada

      int32_t entityID = engine->_entityData->transformTree.numberEntitiesLvL[node->lvl].entityStart + node->index;
      int32_t parentID = engine->_entityData->transformTree.numberEntitiesLvL[node->lvl].parentStart + node->indexparent;

      engine->_entityData->transformTree.AAA[entityID].localMatrix = local_tr->localMatrix;
      engine->_entityData->transformTree.AAA[entityID].indexParent = parentID;  
  }

  void TransformTreeSystem::end(Engine* engine){
    //engine->transformTree.AAA.clear();

    for (unsigned int i = 0; i < numberElements; ++i) {
      if (engine->_entityData->transformTree.AAA[i].indexParent < 0) {
        engine->_entityData->transformTree.AAA[i].wolrdMatrix = engine->_entityData->transformTree.AAA[i].localMatrix;
      }
      else {
        int32_t indexParent = engine->_entityData->transformTree.AAA[i].indexParent;
        engine->_entityData->transformTree.AAA[i].wolrdMatrix =
        engine->_entityData->transformTree.AAA[indexParent].wolrdMatrix *
        engine->_entityData->transformTree.AAA[i].localMatrix;
      }
    }
  }

  TransformTreeToECSSystem::TransformTreeToECSSystem(){
    writeComponents[Component::WORLD_TRANSFORM] = ComponentState::PRESENT;
    readComponents[Component::NODE] = ComponentState::PRESENT;
    type = TRANSFORM_TREE_TO_ECS_SYSTEM;
  }

  void TransformTreeToECSSystem::execute(Engine* engine, const uint32_t entity){

    auto&& ECSTransaction = engine->_entityData->transaction();
    NodeComponent* node = ECSTransaction.getNodeComponent(entity);
    WorldTransformComponent* world_tr = ECSTransaction.getWorldTransformComponent(entity);
    
    int32_t entityID = engine->_entityData->transformTree.numberEntitiesLvL[node->lvl].entityStart + node->index;
    
    world_tr->worldMatrix = engine->_entityData->transformTree.AAA[entityID].wolrdMatrix;
  
  }
    

  CameraSystem::CameraSystem(){

    readComponents[Component::TRS] = ComponentState::PRESENT;
    readComponents[Component::CAMERA] = ComponentState::PRESENT;
    writeComponents[Component::VIEWPROJECTION] = ComponentState::PRESENT;
    type = CAMERA_SYSTEM;
  }

  void CameraSystem::execute(Engine* engine, const uint32_t entity) {

    auto&& ECSTransaction = engine->_entityData->transaction();
    CameraComponent* camera = ECSTransaction.getCameraComponent(entity);
    LocalTransformComponent* transform = ECSTransaction.getLocalTransformComponent(entity);
    TRSComponent* trsComp = ECSTransaction.getTRSComponent(entity);
    ViewProjectionComponent* vpComp = ECSTransaction.getVPComponent(entity);


    vpComp->viewMatrix = glm::inverse(transform->localMatrix);
    vpComp->proyectionMatrix = glm::perspective(camera->FOV, camera->aspectRatio, camera->zNear, camera->zFar);

    //engine->printM4(vpComp->viewMatrix ,"viewMatrix");

    //Calculate  forward, right and up for the camera
    FlyCameraMovementComponent* flyComp = ECSTransaction.getFlyCameraComponent(entity);
    if(nullptr != flyComp){
      float yaw = flyComp->yaw;
      float pitch = flyComp->pitch;

      flyComp->forward.z = cos(yaw) * cos(pitch);
      flyComp->forward.y = sin(pitch);
      flyComp->forward.x = sin(yaw) * cos(pitch);

      flyComp->right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), flyComp->forward));
      flyComp->up = glm::cross(flyComp->forward, flyComp->right); 
      glm::vec3 camPos = {transform->localMatrix[3].x, transform->localMatrix[3].y, transform->localMatrix[3].z };


      SunComponent* sunComp = ECSTransaction.getSunComponent(entity);
      glm::vec3 target = camPos + flyComp->forward;
      if(nullptr != sunComp){
        target = sunComp->lookPosition;
      }
      vpComp->viewMatrix = glm::lookAt(camPos, target, flyComp->up);
      //(engine->printM4(vpComp->viewMatrix ,"viewMatrix2");

  

    }
  }

  LightSystem::LightSystem() {
      
    readComponents[Component::LIGHT] = ComponentState::PRESENT;
    readComponents[Component::WORLD_TRANSFORM] = ComponentState::PRESENT;

    currentLight = 0;

    type = LIGHT_SYSTEM;

  }

  void LightSystem::begin(Engine* engine) {
    currentLight = 0;
      /*auto&& ECSTransaction = engine->_entityData->transaction();
    for(int i = 0; i < engine->_GPUState->currentActivatedLights; ++i){
      uint32_t entity = engine->_entityData->lights[i];
      LightComponent* light = ECSTransaction.getLightComponent(entity);
      LocalTransformComponent* transform = ECSTransaction.getLocalTransformComponent(entity);
      glm::mat4 m = transform->localMatrix;

      engine->_GPUState->frameData.lights.colorAndIntensity[currentLight] = glm::vec4(light->r, light->g, light->b, light->intensity);
      engine->_GPUState->frameData.lights.directionAndCutoff[currentLight] = glm::vec4(light->direction, light->cutOff);
      engine->_GPUState->frameData.lights.positionAndType[currentLight] = glm::vec4(m[3].x, m[3].y, m[3].z, light->lightType);
      engine->_GPUState->frameData.lights.lightProperties[currentLight] = glm::vec4(light->lightWidth, engine->_GPUState->minBias, engine->_GPUState->maxBias, 0.f);

    }*/
    
    //++currentLight;

  }

  void LightSystem::execute(Engine* engine, const uint32_t entity) {

   /* auto&& ECSTransaction = engine->_entityData->transaction();
    LightComponent* light = ECSTransaction.getLightComponent(entity);
    LocalTransformComponent* transform = ECSTransaction.getLocalTransformComponent(entity);
    glm::mat4 m = transform->localMatrix;

    engine->_GPUState->frameData.lights.colorAndIntensity[currentLight] = glm::vec4(light->r, light->g, light->b, light->intensity);
    engine->_GPUState->frameData.lights.directionAndCutoff[currentLight] = glm::vec4(light->direction, light->cutOff);
    engine->_GPUState->frameData.lights.positionAndType[currentLight] =  glm::vec4(m[3].x, m[3].y, m[3].z, light->lightType);
    engine->_GPUState->frameData.lights.lightProperties[currentLight] =  glm::vec4(light->lightWidth, engine->_GPUState->minBias, engine->_GPUState->maxBias, 0.f);

      ++currentLight;*/
  }

  BoundingBoxSystem::BoundingBoxSystem() {

    readComponents[Component::LIGHT] = ComponentState::PRESENT;
    readComponents[Component::WORLD_TRANSFORM] = ComponentState::PRESENT;
    writeComponents[Component::CAMERA] = ComponentState::PRESENT;

    type = BOUNDING_BOX_SYSTEM;

  }

  void BoundingBoxSystem::begin(Engine* engine) {

    engine->boundingBoxScene[0] = {engine->sceneSize, engine->sceneSize, engine->sceneSize};
    engine->boundingBoxScene[1] = {engine->sceneSize, engine->sceneSize, -engine->sceneSize};
    engine->boundingBoxScene[2] = {engine->sceneSize, -engine->sceneSize, engine->sceneSize};
    engine->boundingBoxScene[3] = {engine->sceneSize, -engine->sceneSize, -engine->sceneSize};
    engine->boundingBoxScene[4] = {-engine->sceneSize, engine->sceneSize, engine->sceneSize};
    engine->boundingBoxScene[5] = {-engine->sceneSize, engine->sceneSize, -engine->sceneSize};
    engine->boundingBoxScene[6] = {-engine->sceneSize, -engine->sceneSize, engine->sceneSize};
    engine->boundingBoxScene[7] = {-engine->sceneSize, -engine->sceneSize, -engine->sceneSize};

    radius = sqrt((engine->sceneSize * engine->sceneSize) * 2);

    //printf("%f", radius);
  }

  float minDistance(const glm::vec3  v1, const glm::vec3 v2){
    
    float min = glm::abs(v1.x - v2.x);
    float aux_min = glm::abs(v1.y - v2.y);
    if(aux_min < min){
      min = aux_min;
    }
    aux_min = glm::abs(v1.z - v2.z);
    if (aux_min < min) {
      min = aux_min;
    }
    return min;
  }

  void BoundingBoxSystem::execute(Engine* engine, const uint32_t entity) {

    auto&& ECSTransaction = engine->_entityData->transaction();
    CameraComponent* cameraComponent = ECSTransaction.getCameraComponent(entity);
    LocalTransformComponent* wtc = ECSTransaction.getLocalTransformComponent(entity);

    glm::vec3 lightPos = glm::vec3(wtc->localMatrix[3].x, wtc->localMatrix[3].y, wtc->localMatrix[3].z);
    float nearest = glm::distance(lightPos, engine->boundingBoxScene[0]);
    float furthest = glm::distance(lightPos, engine->boundingBoxScene[0]);

    float sceneS = radius;// 30 -> 42.42f

    bool inside = -sceneS < lightPos.x;
    inside = inside && lightPos.x < sceneS;
    inside = inside && -sceneS < lightPos.y;
    inside = inside && lightPos.y < sceneS;
    inside = inside && -sceneS < lightPos.z;
    inside = inside && lightPos.z < sceneS;
    float distanceCenter = glm::length(lightPos);

    nearest = distanceCenter - radius;
    furthest = distanceCenter + radius;
    
    /*for(int i = 0; i < 8; ++i) {
        
      float tmpDistance = glm::distance(lightPos, engine->boundingBoxScene[i]);
      float tmpDistance1 = glm::distance(lightPos, engine->boundingBoxScene[i]);

      if(tmpDistance < nearest)
        nearest = tmpDistance;

      if (tmpDistance1 > furthest)
        furthest = tmpDistance1;

    }*/
    
    cameraComponent->zNear = nearest;

    if(inside)
      cameraComponent->zNear = 0.1f;
    
    cameraComponent->zFar = furthest;

  }

}
