#include <lua_manager.h>
#include "internals.h"
#include "ecs.h"
#include "system.h"
#include <algorithm>

namespace AWK {

  /* --- Lua Base --- */
  LuaBase::LuaBase() {

    L = luaL_newstate();
    luaL_openlibs(L);

  }

  LuaBase::~LuaBase() {

    lua_close(L);

  }

  /* --- LuaLoadShaders --- */
  LuaLoadShader::LuaLoadShader(Engine::GPUState* gpuState) {

    _GPUState = gpuState;
    lua_pushlightuserdata(L, _GPUState);
    lua_pushcclosure(L, loadShader,1);
    lua_setglobal(L, "loadShader");

    int err = luaL_dofile(L, "../../data/lua/shader.lua");

  }

  LuaLoadShader::~LuaLoadShader() {}

  int LuaLoadShader::loadShader(lua_State* L) {


    int args = lua_gettop(L);

    if(args != 3)
      return luaL_error(L, "Expected 3 arguments, got %d", args);

    const char* name = luaL_checkstring(L, 1);
    const char* vertexShader = luaL_checkstring(L, 2);
    const char* fragmentShader = luaL_checkstring(L, 3);
        
    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto gpuState = reinterpret_cast<Engine::GPUState*>(raw_ptr);
    /* --------------------------------------------------------- */
    

    /*-----------SHADERS------------*/
    printf("------ SHADER: %s ------\n\n", name);
    GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &vertexShader, nullptr);
    glCompileShader(shaderVertex);

    GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &fragmentShader, nullptr);
    glCompileShader(shaderFragment);

    char msg[255];
    glGetShaderInfoLog(shaderVertex, 255, NULL, msg);
    printf("Vertex:\n\n %s\n\n", msg);
    glGetShaderInfoLog(shaderFragment, 255, NULL, msg);
    printf("Fragment:\n\n %s\n\n", msg);

    /*----------PROGRAM--------*/
    GLuint program = glCreateProgram();
    glAttachShader(program, shaderVertex);
    glAttachShader(program, shaderFragment);
    glLinkProgram(program);
    glUseProgram(program);

    gpuState->_programsArray.push_back(program);
    return 0;
  }

  /* --- LuaCreateScene --- */
  LuaCreateScene::LuaCreateScene() {


  }

  void LuaCreateScene::init(Engine* engine, ECSData* ECS){
    this->engine = engine;
    this->ECS = ECS;
    // Upload functions to lua
    lua_pushlightuserdata(L, engine);
    lua_pushlightuserdata(L, this->ECS);
    lua_pushcclosure(L, createEntityL, 2);
    lua_setglobal(L, "createEntity");

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, createTexture, 1);
    lua_setglobal(L, "createTexture");



    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, createMaterial, 1);
    lua_setglobal(L, "createMaterial");

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, setColor, 1);
    lua_setglobal(L, "setColor");

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, setTexture, 1);
    lua_setglobal(L, "setTexture");
    //! end Upload Functions

    // Register Values
    lua_newtable(L);
    lua_pushinteger(L, Component::AddComponentType::ADD_RENDER);
    lua_setfield(L, -2, "RENDER");
    lua_pushinteger(L, Component::AddComponentType::ADD_TRS);
    lua_setfield(L, -2, "TRS");
    lua_pushinteger(L, Component::AddComponentType::ADD_LOCAL_TRANSFORM);
    lua_setfield(L, -2, "LOCAL_TRANSFORM");
    lua_pushinteger(L, Component::AddComponentType::ADD_WORLD_TRANSFORM);
    lua_setfield(L, -2, "WORLD_TRANSFORM");
    lua_pushinteger(L, Component::AddComponentType::ADD_NODE);
    lua_setfield(L, -2, "NODE");
    lua_pushinteger(L, Component::AddComponentType::ADD_CAMERA);
    lua_setfield(L, -2, "CAMERA");
    lua_pushinteger(L, Component::AddComponentType::ADD_VIEWPROJECTION);
    lua_setfield(L, -2, "VP");
    lua_pushinteger(L, Component::AddComponentType::ADD_LIGHT);
    lua_setfield(L, -2, "LIGHT");
    lua_pushinteger(L, Component::AddComponentType::TRS_AND_TRANSFORM);
    lua_setfield(L, -2, "TRS_AND_TRANSFORM");

    lua_setglobal(L, "ComponentType");
    // !Register Values

    // Register Values TextureType
    lua_newtable(L);
    lua_pushinteger(L, Texture::Type::RGBA);
    lua_setfield(L, -2, "RGBA");
    lua_pushinteger(L, Texture::Type::RGB);
    lua_setfield(L, -2, "RGB");
    lua_pushinteger(L, Texture::Type::RG);
    lua_setfield(L, -2, "RG");
    lua_pushinteger(L, Texture::Type::R);
    lua_setfield(L, -2, "R");
    lua_pushinteger(L, Texture::Type::DEPTH);
    lua_setfield(L, -2, "DEPTH");
    lua_setglobal(L, "TextureType");


    // !Register Values TextureType

    


    // Register MaterialTypes
     
    lua_newtable(L);
    lua_pushinteger(L, Material::MaterialParameters::MaterialType::MaterialType_BlinnPhong);
    lua_setfield(L, -2, "BlinnPhong"); 
    lua_pushinteger(L, Material::MaterialParameters::MaterialType::MaterialType_TextureColor);
    lua_setfield(L, -2, "TextureColor");
    lua_pushinteger(L, Material::MaterialParameters::MaterialType::MaterialType_Atmosphere);
    lua_setfield(L, -2, "Atmosphere");
    lua_setglobal(L, "MaterialType");
    // !Register MaterialTypes




    int err1 = luaL_dofile(L, "../../data/lua/material.lua");
    int err = luaL_dofile(L, "../../data/lua/scene.lua");
    

  }

  void LuaCreateScene::update(float time){
    int luaType  = lua_getglobal(L, "update");

    if(luaType == LUA_TFUNCTION){
      lua_pushnumber(L, time);

      if (lua_pcall(L, 1, 0, 0) != 0) {
        luaL_error(L, "error running function 'update': %s",
          lua_tostring(L, -1));
      }
    }else{
      lua_pop(L, 1);
    }
  }

  LuaCreateScene::~LuaCreateScene(){}

  int LuaCreateScene::createEntityL(lua_State* L){

    int args = lua_gettop(L);
    uint64_t entityMask = Component::AddComponentType::ADD_ALL;

    if (args == 1)
      entityMask = (uint64_t)lua_tointeger(L, 1);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);
    raw_ptr = lua_touserdata(L, lua_upvalueindex(2));
    auto ECS = reinterpret_cast<ECSData*>(raw_ptr);

    // Create OBJ 
    lua_newtable(L);
    auto&& ECSTransaction = ECS->transaction();

    uint32_t tmpID = ECSTransaction.createEntity(entityMask);
    lua_pushinteger(L, tmpID);
    lua_setfield(L, -2, "id");

    if(luaL_newmetatable(L, "MetaEntity")){

      lua_newtable(L);

      // Set Position
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setEntityPosition, 1);
      lua_setfield(L, -2, "setPosition");

      // Set Rotation
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setEntityRotation, 1);
      lua_setfield(L, -2, "setRotation");

      // Set Rotation in Radians
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setEntityRotationRad, 1);
      lua_setfield(L, -2, "setRotationInRadians");

      // Set Scale
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setEntityScale, 1);
      lua_setfield(L, -2, "setScale");

      // Set Transform
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setEntityTransform, 1);
      lua_setfield(L, -2, "setTransform");

      // Load OBJ
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, loadObj, 1);
      lua_setfield(L, -2, "loadObj");

      // Set OBJ
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setObj, 1);
      lua_setfield(L, -2, "setObj");

      // Add Child
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, addChild, 1);
      lua_setfield(L, -2, "addChild");

      // Add Child
      lua_pushlightuserdata(L, engine);
      lua_pushcclosure(L, setMaterial, 1);
      lua_setfield(L, -2, "setMaterial");



      // Add more functions
      lua_setfield(L, -2, "__index");

    }

    lua_setmetatable(L, -2);
    
    return 1;
  }

  int LuaCreateScene::loadObj(lua_State* L) {

    const uint32_t n_args = 2;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);    

    lua_getfield(L, 1, "id");

    const uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    const char* objPath = lua_tostring(L, 2);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);
    auto&& ECSTransaction = engine->_entityData->transaction();
    RenderComponent* render = ECSTransaction.getRenderComponent(entityID);

    char originalPath[255];
    sprintf(originalPath, "../../data/models/%s", objPath);

    render->meshID = engine->loadObj(originalPath);
    lua_pushinteger(L, render->meshID);
    return 1;
  }

  int LuaCreateScene::setObj(lua_State* L) {

    const uint32_t n_args = 2;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);
    
    lua_getfield(L, 1, "id");

    const uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    const uint32_t meshID = (uint32_t)lua_tointeger(L, 2);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    RenderComponent* render = ECSTransaction.getRenderComponent(entityID);
    render->meshID = meshID;

    return 0;
  }

  int LuaCreateScene::setEntityPosition(lua_State* L){

    const uint32_t n_args = 4;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args,  args);

    lua_getfield(L, 1, "id");

    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ECSTransaction.getTRSComponent(entityID);
    trs->translation.x = x;
    trs->translation.y = y;
    trs->translation.z = z;

    return 0;
  }

  int LuaCreateScene::setEntityRotation(lua_State* L) {

    const uint32_t n_args = 4;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    lua_getfield(L, 1, "id");

    float piConversor = 3.1415f / 180.f;

    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    float x = (float)lua_tonumber(L, 2) * piConversor;
    float y = (float)lua_tonumber(L, 3) * piConversor;
    float z = (float)lua_tonumber(L, 4) * piConversor;

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ECSTransaction.getTRSComponent(entityID);
    trs->rotation.x = x;
    trs->rotation.y = y;
    trs->rotation.z = z;

    return 0;
  }

  int LuaCreateScene::setEntityRotationRad(lua_State* L) {

    const uint32_t n_args = 4;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    lua_getfield(L, 1, "id");

    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ECSTransaction.getTRSComponent(entityID);
    trs->rotation.x = x;
    trs->rotation.y = y;
    trs->rotation.z = z;

    return 0;
  }

  int LuaCreateScene::setEntityScale(lua_State* L) {

    const uint32_t n_args = 4;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    lua_getfield(L, 1, "id");

    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ECSTransaction.getTRSComponent(entityID);
    trs->scale.x = x;
    trs->scale.y = y;
    trs->scale.z = z;

    return 0;
  }

  int LuaCreateScene::setEntityTransform(lua_State* L) {

    const uint32_t n_args = 10;
    int args = lua_gettop(L);
    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    lua_getfield(L, 1, "id");

    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    float tx = (float)lua_tonumber(L, 2);
    float ty = (float)lua_tonumber(L, 3);
    float tz = (float)lua_tonumber(L, 4);
    
    float rx = (float)lua_tonumber(L, 5);
    float ry = (float)lua_tonumber(L, 6);
    float rz = (float)lua_tonumber(L, 7);
    
    float sx = (float)lua_tonumber(L, 8);
    float sy = (float)lua_tonumber(L, 9);
    float sz = (float)lua_tonumber(L, 10);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ECSTransaction.getTRSComponent(entityID);
    trs->translation.x = tx;
    trs->translation.y = ty;
    trs->translation.z = tz;
    trs->rotation.x = rx;
    trs->rotation.y = ry;
    trs->rotation.z = rz;
    trs->scale.x = sx;
    trs->scale.y = sy;
    trs->scale.z = sz;

   

    return 0;
  }

  int LuaCreateScene::addChild(lua_State* L){

    const uint32_t n_args = 2;
    int args = lua_gettop(L);

    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    lua_getfield(L, 1, "id");
    uint32_t parentID = (uint32_t)lua_tointeger(L, -1);

    lua_getfield(L, 2, "id");
    uint32_t childID = (uint32_t)lua_tointeger(L, -1);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    auto engine = reinterpret_cast<Engine*>(raw_ptr);

    auto&& ECSTransaction = engine->_entityData->transaction();
    ECSTransaction.addChild(parentID, childID);

    return 0;
  }
  
  int LuaCreateScene::setMaterial(lua_State* L) {
    const uint32_t n_args = 2;
    int args = lua_gettop(L);

    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);


    lua_getfield(L, 1, "id");
    uint32_t entityID = (uint32_t)lua_tointeger(L, -1);
    uint32_t matID = (uint32_t)lua_tointeger(L, 2);

    auto&& ecsTransaction = engine->_entityData->transaction();

    RenderComponent* render = ecsTransaction.getRenderComponent(entityID);
    render->materialID = matID;
    return 0;

  }

  int LuaCreateScene::createTexture(lua_State*L){
    const uint32_t n_args = 1;
    int args = lua_gettop(L);
    if (args != n_args )
      return luaL_error(L, "Expected %d arguments, got %d or %d ", n_args, args, 1);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);
    
    const char* texturePath = lua_tostring(L, 1);

    lua_pushinteger(L, engine->createTexture(texturePath));
    

    return 1;
  }

  int LuaCreateScene::createMaterial(lua_State* L) {
    const uint32_t n_args = 1;
    int args = lua_gettop(L);
    if(args != n_args){
      return luaL_error(L, "Expected %d, arguments, got %d", n_args, args);
    }
    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);
    int materiaType = lua_tointeger(L, 1);


    AWK::Material::MaterialParameters new_mat;
    new_mat.data.textureColor.color.r = 1.0f;
    new_mat.data.textureColor.color.g = 1.0f;
    new_mat.data.textureColor.color.b = 1.0f;
    new_mat.data.textureColor.color.a = 1.0f;
    new_mat.data.textureColor.texture = 0;
    new_mat.type = (Material::MaterialParameters::MaterialType)materiaType;
    uint32_t newMat = engine->createMaterial(new_mat);
    
    lua_pushinteger(L, newMat);
    return 1;

  }


  int LuaCreateScene::setColor(lua_State* L) {
    const uint32_t n_args = 5;
    int args = lua_gettop(L);
    if (args != n_args) {
      return luaL_error(L, "Expected %d, arguments, got %d", n_args, args);
    }
    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);

   

    uint32_t materialID = (uint32_t)lua_tonumber(L, 1);
    AWK::Material::RGBA color;
    color.r = (float)lua_tonumber(L, 2);
    color.g = (float)lua_tonumber(L, 3);
    color.b = (float)lua_tonumber(L, 4);
    color.a = (float)lua_tonumber(L, 5);
    
    engine->changeMaterialColor(materialID,color);
    return 0;
  }

  int LuaCreateScene::setTexture(lua_State* L) {
    const uint32_t n_args = 2;
    int args = lua_gettop(L);
    if (args != n_args) {
      return luaL_error(L, "Expected %d, arguments, got %d", n_args, args);
    }
    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);

    uint32_t materialID = (uint32_t)lua_tonumber(L, 1);
    uint32_t textureID = (uint32_t)lua_tonumber(L, 2);
    engine->changeMaterialTexture(materialID, textureID);


    return 1;
  }

  /* --- Lua Composer --- */
  LuaComposer::LuaComposer(Engine* engine) {

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, renderScene, 1);
    lua_setglobal(L, "renderScene");

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, displayScene, 1);
    lua_setglobal(L, "displayScene");

    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, renderShadowScene, 1);
    lua_setglobal(L, "renderShadowScene"); 
    
    lua_pushlightuserdata(L, engine);
    lua_pushcclosure(L, blurPP, 1);
    lua_setglobal(L, "blur");

    int err = luaL_dofile(L, "../../data/lua/composer.lua");
    
  }

  LuaComposer::~LuaComposer(){

  }

  int LuaComposer::renderScene(lua_State* L) {

    const uint32_t n_args = 1;
    int args = lua_gettop(L);

    if (args != n_args && args != 0)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);
    uint32_t colorTexture = 0;
    uint32_t depthTexture = 0;
    {
      auto&& ECSTransaction = engine->_entityData->transaction();

      CameraComponent* camera = ECSTransaction.getCameraComponent(engine->_entityData->camera);

      // Create Color & Depth Textures
      colorTexture = engine->requestTexture(camera->width, camera->height, Texture::Type::RGBA);
      depthTexture = engine->requestTexture(camera->width, camera->height, Texture::Type::DEPTH);
    }

    std::list<uint32_t>::iterator it = engine->composerTextureList.begin();
    uint32_t cnt = 0;

    while (cnt < colorTexture && it != engine->composerTextureList.end()) {

      ++cnt;
      ++it;

    }
    
    engine->textureColor = *it;

    cnt = 0;
    it = engine->composerTextureList.begin();
    while (cnt < depthTexture && it != engine->composerTextureList.end()) {

      ++cnt;
      ++it;

    }

    engine->textureDepth = *it;

    // RenderSystem Call
    uint64_t map_mask = 0;

    std::unique_ptr<RenderSystem> _renderSystem = std::make_unique<RenderSystem>();
    _renderSystem->init();
    _renderSystem->activateCamera = engine->_entityData->camera;
    
    _renderSystem->begin(engine);

    for (auto& x : engine->_entityData->entitiesMap) {

      map_mask = x.first;

      if ((_renderSystem->presentmask & map_mask) == _renderSystem->presentmask &&
          (_renderSystem->missingmask & map_mask) == 0) {

        for (int j = 0; j < engine->_entityData->entitiesMap[map_mask].size(); ++j) {

          _renderSystem->execute(engine, engine->_entityData->entitiesMap[map_mask][j]->id);

        }

      }
    }

    _renderSystem->end(engine);

    lua_pushinteger(L, colorTexture);

    return 1;
  }

  int LuaComposer::displayScene(lua_State* L) {

    const uint32_t n_args = 1;
    int args = lua_gettop(L);

    if (args != n_args)
      return luaL_error(L, "Expected %d arguments, got %d", n_args, args);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);
    uint32_t sceneTexture = (uint32_t)lua_tointeger(L, 1);

    std::list<uint32_t>::iterator it = engine->composerTextureList.begin();
    uint32_t cnt = 0;

    while(cnt < sceneTexture && it != engine->composerTextureList.end()) {          
      
      ++cnt;
      ++it;

    }  
    
    engine->blitTextureID = *it;

    return 0;

  }

  int LuaComposer::blurPP(lua_State* L){

    const uint32_t nArgs = 3;
    int args = lua_gettop(L);
    if (args != nArgs && args != 0)
      return luaL_error(L, "Expected %d arguments, got %d", nArgs, args);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);

    uint32_t texture = (uint32_t)lua_tointeger(L, 1);
    uint32_t nRepetitions= (uint32_t)lua_tointeger(L, 2);
    const char* mode = lua_tostring(L, 3);

    uint32_t bMode = Material::MaterialParameters::MaterialType::MaterialType_Blur;
    auto&& ECSTransaction = engine->_entityData->transaction();

    CameraComponent* camera = ECSTransaction.getCameraComponent(engine->_entityData->camera);
    uint32_t colorTexture = engine->requestTexture(camera->width, camera->height, Texture::Type::RGBA);
    uint32_t depthTexture = engine->requestTexture(camera->width, camera->height, Texture::Type::DEPTH);

    std::list<uint32_t>::iterator it = engine->composerTextureList.begin();
    uint32_t cnt = 0;

    while (cnt < colorTexture && it != engine->composerTextureList.end()) {

      ++cnt;
      ++it;

    }

    uint32_t realCT = *it;

    cnt = 0;
    it = engine->composerTextureList.begin();
    while (cnt < depthTexture && it != engine->composerTextureList.end()) {

      ++cnt;
      ++it;

    }
    uint32_t realDT = *it;

    cnt = 0;
    it = engine->composerTextureList.begin();
    while (cnt < texture && it != engine->composerTextureList.end()) {

      ++cnt;
      ++it;

    }
    uint32_t sceneTexture = *it;


    uint32_t imode = 0;
    if (0 == strcmp("Both", mode)) {
      imode = 2;
    }
    else {

      if (0 == strcmp("Vertical", mode)) {
        imode = 1;
      }
     
    }

    DisplayList dl;

    uint32_t data[4];

    data[0] = nRepetitions;
    data[1] = nRepetitions;
    data[2] = imode;
    data[3] = sceneTexture;
  
    dl.clear(engine->bgColor[0], engine->bgColor[1], engine->bgColor[2], engine->bgColor[3]);
    dl.blur(data);

    engine->renderSubmit(std::move(dl), realCT, realDT);
    //Crear la DL un hacer un submit


    lua_pushinteger(L, colorTexture);
    return 1;
  }

  int LuaComposer::renderShadowScene(lua_State* L){
    const uint32_t nArgs = 1;
    int args = lua_gettop(L);

    if (args != nArgs && args != 0)
      return luaL_error(L, "Expected %d arguments, got %d", nArgs, args);

    void* raw_ptr = lua_touserdata(L, lua_upvalueindex(1));
    Engine* engine = reinterpret_cast<Engine*>(raw_ptr);

    // Create Color & Depth Textures
    uint32_t colorTexture = 0;
      uint32_t depthTexture = 0;
    
    for(int i = 0; i < engine->_GPUState->currentActivatedShadows; ++i){
      
      {

        auto&& ECSTransaction = engine->_entityData->transaction();
        CameraComponent*cc = ECSTransaction.getCameraComponent(engine->_entityData->lights[i]);
  
         colorTexture = engine->requestTexture(cc->width, cc->height, Texture::Type::RGBA);
         depthTexture = engine->requestTexture(cc->width, cc->height, Texture::Type::DEPTH);
      }

      std::list<uint32_t>::iterator it = engine->composerTextureList.begin();
      uint32_t cnt = 0;

      while (cnt < colorTexture && it != engine->composerTextureList.end()) {

        ++cnt;
        ++it;

      }

      engine->textureColor = *it;
      engine->testTextureID = *it;
      engine->_GPUState->_lightShadowTexture[i] = *it;

      cnt = 0;
      it = engine->composerTextureList.begin();
      while (cnt < depthTexture && it != engine->composerTextureList.end()) {

        ++cnt;
        ++it;

      }

      engine->textureDepth = *it;
      engine->_GPUState->_shadowTexture[i] = *it;

      // RenderSystem Call
      uint64_t map_mask = 0;
    
      std::unique_ptr<RenderSystem> _renderSystem = std::make_unique<RenderSystem>();
      _renderSystem->init();


      _renderSystem->activateCamera = engine->_entityData->lights[i];
      _renderSystem->beginShadow(engine,i);

      for (auto& x : engine->_entityData->entitiesMap) {

        map_mask = x.first;

        if ((_renderSystem->presentmask & map_mask) == _renderSystem->presentmask &&
          (_renderSystem->missingmask & map_mask) == 0) {

          for (int j = 0; j < engine->_entityData->entitiesMap[map_mask].size(); ++j) {

            _renderSystem->executeShadow(engine, engine->_entityData->entitiesMap[map_mask][j]->id);

          }

        }
      }

      _renderSystem->end(engine);

      engine->shadowMapTextures.push_back(depthTexture);

    }
    return 0;
  }
}