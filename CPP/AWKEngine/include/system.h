
/************************************************
 * System Class
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__ 1

#include <stdint.h>
#include <component.h>

namespace AWK {

  class Engine;
  class DisplayList;
  struct EntityHandler;
  struct Camera;
  
  //! ComponentState Enum.
  /*! Tell the presence of a component for a system. */
  enum ComponentState {
    DONT_CARE = 0,  /*!< Dont care, That system does not care if the entity has or not that component. */  
    PRESENT,        /*!< Present, The entity has to have that component to enter in the system. */  
    MISSING         /*!< Present, The entity must not have that component to enter in the system. */ 
  };

  //! SystemType Enum.
  /*! Indicate the type of the system. */
  enum SystemType {
    RENDER_SYSTEM = 0,              /*!< Render type. */  
    TRANSFORM_SYSTEM,               /*!< Transform type. */  
    TRANSFORM_TREE_SYSTEM,          /*!< Transform tree type. */  
    TRANSFORM_TREE_TO_ECS_SYSTEM,   /*!< Transform tree to ECS type. */
    CAMERA_SYSTEM,
    LIGHT_SYSTEM,
    BOUNDING_BOX_SYSTEM,
    MAX /*!< Tell how many systems are in total. */  
  };


  //! System struct.
  /*! Is the base archetype for the rest of the systems. */
	struct System {

    SystemType type;     /*!< Has the type of the system. */  

    ComponentState readComponents[Component::MAX];  /*!< Array of Component State for the read components of each system. Store if that component will be use or not for reading. */  
    ComponentState writeComponents[Component::MAX]; /*!< Array of Component State for the write components of each system. Store if that component will be use or not for writing.*/  

    uint64_t presentmask;  /*!< The mask of the system for the present component. */  
    uint64_t missingmask;  /*!< The mask of the system for the missing component. */  

    System();
    virtual void init();

    //! Make the functionality of each system.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \param entity. ID of the entity you want to work with.
    \sa begin(Engine* engine), end(Engine* engine).
    */
    virtual void execute(Engine* engine, const uint32_t entity) = 0;

    //! Make functionality only once before start all the execution of the system.
     /*!
     \param engine. A reference of the engine necessary for make the work.
     \sa execute(Engine* engine, const uint32_t entity), end(Engine* engine).
     */
    virtual void begin(Engine* engine) {}

    //! Make functionality only once after end all the execution of the system.
     /*!
     \param engine. A reference of the engine necessary for make the work.
     \sa execute(Engine* engine, const uint32_t entity), begin(Engine* engine).
     */
    virtual void end(Engine* engine) {}
    virtual ~System() {}
  
	};


  //! Render System struct.
  /*! System that create all the command for the render. */
  struct RenderSystem : public System {

    RenderSystem();
    virtual ~RenderSystem();
    
    //! Create a clear command.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \sa execute(Engine* engine, const uint32_t entity), end(Engine* engine).
    */
    virtual void begin(Engine* engine) override;
    virtual void beginShadow(Engine* engine, uint32_t lightIndex) ;


    //! Create the draw command for an entity.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \param entity. ID of the entity you want to work with.
    \sa begin(Engine* engine), end(Engine* engine).
    */
    virtual void execute(Engine* engine, const uint32_t entity);
    virtual void executeShadow(Engine* engine, const uint32_t entity);

    //! Submit the display list with all the render commands.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \sa execute(Engine* engine, const uint32_t entity), begin(Engine* engine).
    */
    virtual void end(Engine* engine) override;

    DisplayList* dl; /*!< Display list to create and submit all the commands */
    Camera* activeCamera;
    uint32_t activateCamera;
  };

  //! Transform System struct.
  /*! Calculate all the local matrix for the component using its TRS component.*/
  struct TransformSystem : public System {

    TransformSystem();

    //! Create the local matrix for each entity.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \param entity. ID of the entity you want to work with.
    \sa begin(Engine* engine), end(Engine* engine).
    */
    virtual void execute(Engine* engine, const uint32_t entity);
  };


  //! Transform Tree struct.
  /*! Make all the calculus to for the scene graph.*/
  struct TransformTreeSystem : public System {

    TransformTreeSystem();

    uint32_t numberElements; /*!< Number of total elements */

    //! Calculate the number of total elements in the transform tree and where are the segments .
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \sa execute(Engine* engine, const uint32_t entity), end(Engine* engine).
    */
    virtual void begin(Engine* engine) override;

    //! Fill all the transform tree with the local matrix of each entity and where is their parent in the Transform tree.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \param entity. ID of the entity you want to work with.
    \sa begin(Engine* engine), end(Engine* engine).
    */
    virtual void execute(Engine* engine, const uint32_t entity);

    //! Calculate all the world matrix of each entity in the transform tree.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \sa execute(Engine* engine, const uint32_t entity), begin(Engine* engine).
    */
    virtual void end(Engine* engine) override;

  };

  //! Transform Tree to ECS struct.
  /*! Fill all the world matrix of each entity with the Transform tree information.*/
  struct TransformTreeToECSSystem : public System {

    TransformTreeToECSSystem();

    //! Fill all the information of each entity with its World matrix from the transform tree.
    /*!
    \param engine. A reference of the engine necessary for make the work.
    \param entity. ID of the entity you want to work with.
    \sa begin(Engine* engine), end(Engine* engine).
    */
    virtual void execute(Engine* engine, const uint32_t entity);

  };

  struct CameraSystem : public System{
    CameraSystem();

    virtual void execute(Engine* engine, const uint32_t entity);
  };

  struct LightSystem : public System {

    LightSystem();

    uint32_t currentLight;
    
    virtual void begin(Engine* engine) override;
    virtual void execute(Engine* engine, const uint32_t entity);

  };

  struct BoundingBoxSystem : public System {

    BoundingBoxSystem();

    virtual void begin(Engine* engine) override;
    virtual void execute(Engine* engine, const uint32_t entity);
    float radius;

  };

};
 
#endif // !__SYSTEM__H__