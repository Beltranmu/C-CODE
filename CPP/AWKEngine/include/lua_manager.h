
/************************************************
 * Lua Manager Structs
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __LUA_MANAGER_H__
#define __LUA_MANAGER_H__ 1

#include <iostream>
#include "lua.hpp"
#include "engine.h"

namespace AWK {

  class GPUState;
  class Engine;
  struct ECSData;
  //! LuaBase struct.
  /*! Is the base archetype that our Lua strucs will have*/
  struct LuaBase {

    LuaBase();
    virtual ~LuaBase();

    protected:
      lua_State* L = nullptr; /*!< State of Lua. */

  };

  //! LuaLoadShader struct.
  /*! It will load all the shaders of the engine. Those shaders are written in shader.lua*/
  struct LuaLoadShader : public LuaBase {

    LuaLoadShader(Engine::GPUState* gpuState);
    ~LuaLoadShader();

    Engine::GPUState* _GPUState; /*!< GPUState of the engine, to load and save the shaders on the engine. */

    //! Load a new shader.
    /*!
      \param State of lua that will have all the params in the stack.
    */
    static int loadShader(lua_State* L);

  };


  //! LuaCreateScene struct.
  /*! It will create the scene at the beginning of the execution. All the instruction are in scene.lua
    All the function must be called from Lua.*/
  struct LuaCreateScene : public LuaBase {

    LuaCreateScene();
    ~LuaCreateScene();

    Engine* engine; /*!< Ref to the engine. Needed to store and create all the new geometries.*/
    ECSData* ECS;
    void init(Engine* engine, ECSData* ECS);
    void update(float time);

    //! Create a new entity.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int createEntityL(lua_State* L);

    //! Load a new obj and set this obj to an entity.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int loadObj(lua_State* L);

    //! Set and already created obj to an entity.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setObj(lua_State* L);

    //! Set the Position of an entity.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setEntityPosition(lua_State* L);

    //! Set the Rotation of an entity. Angles in degrees.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setEntityRotation(lua_State* L);

    //! Set the Rotation of an entity. Angles in radians.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setEntityRotationRad(lua_State* L);

    //! Set the Scale of an entity. 
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setEntityScale(lua_State* L);

    //! Set the Transform (Position, Rotation (radians), Scale ) of an entity. 
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int setEntityTransform(lua_State* L);

    //! Add a new child to an entity.
    /*!
    \param State of lua that will have all the params in the stack.
    */
    static int addChild(lua_State* L);

    static int setMaterial(lua_State* L);
    static int createMaterial(lua_State* L);
    static int createTexture(lua_State* L);
    static int setColor(lua_State* L);
    static int setTexture(lua_State* L);

  };
 
  struct LuaComposer : public LuaBase{
    LuaComposer(Engine* engine);
    ~LuaComposer();
    Engine* engine;
    
    static int renderScene(lua_State* L);
    static int displayScene(lua_State* L);
    static int blurPP(lua_State* L);
    static int renderShadowScene(lua_State* L);
    
  };


}

#endif // !__LUA_MANAGER_H__