
/************************************************
 * Component Class
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/
#ifndef __COMPONENT_H__
#define __COMPONENT_H__ 1

#include <stdint.h>
#include <glm.hpp>

namespace AWK {
  struct EntityHandler;


  //! Component Struct.
  /*! Base Archetype for the rest of the Components.*/
	struct Component
  {
      Component() { }
      //! ComponentType Enum.
      /*! Different types that a Component may be. */
      enum ComponentType {
        INVALID = 0,      /*!< Enum value Invalid, invalid component. */  
        RENDER ,          /*!< Enum value Render, render component type. */  
        TRS,              /*!< Enum value TRS, translation rotate and scale. */  
        LOCAL_TRANSFORM,  /*!< Enum value Local_transform, local transform component */  
        WORLD_TRANSFORM,  /*!< Enum value World_transform, world transform component */  
        NODE,             /*!< Enum value Node, node component type */
        CAMERA,
        VIEWPROJECTION,
        SUN,
        FLY_CAMERA,
        LIGHT,
        MAX               /*!< Enum value Max, know how many component the engine has and where is the limit */  
      };

      //! AddComponentType Enum.
      /*! Different mask that the user will use to create a new entity  */
      enum AddComponentType {
        ADD_RENDER = 1 << 1,          /*!< Render Mask. Only to add a Render Component */
        ADD_TRS = 1 << 2,             /*!< TRS Mask. Only to add a TRS Component */
        ADD_LOCAL_TRANSFORM = 1 << 3, /*!< Local Transform Mask. Only to add a Local Transform Component */
        ADD_WORLD_TRANSFORM = 1 << 4, /*!< World Transform Mask. Only to add a World Transform Component */
        ADD_NODE = 1 << 5,            /*!< Node Mask. Only to add a Node Component */
        TRS_AND_TRANSFORM = ADD_TRS | ADD_LOCAL_TRANSFORM | ADD_WORLD_TRANSFORM, /*!< TRS and Transform Mask. To add TRS, Local and world transform component */
        ADD_CAMERA = 1 << 6 | 1 <<7 | TRS_AND_TRANSFORM,
        ADD_VIEWPROJECTION = 1 <<7,
        ADD_SUN = 1 << 8,
        ADD_FLY_CAMERA = 1 << 9,
        ADD_LIGHT = 1 << 10 | TRS_AND_TRANSFORM,
        
        
        ADD_ALL = 0xffffffff /*!< All Add possible components for a entity */
      };
	};

  //! RenderComponent Struct.
  /*! RenderComponent store the material ID and mesh ID for the render  */
  struct RenderComponent : public Component {
    uint32_t meshID;          /*!< ID of the mesh to render. */
    uint32_t materialID;      /*!< ID of the material that the entity will have. */
  };

  //! TRSComponent Struct.
  /*! TRSComponent store the translation rotation and scale for an entity  */
  struct TRSComponent : public Component{
    glm::vec3 translation;    /*!< translation of the entity. */
    glm::vec3 rotation;       /*!< rotation of the entity. */
    glm::vec3 scale;          /*!< scale of the entity. */
  };

  //! LocalTransformComponent Struct.
  /*! LocalTransformComponent used to store the local transform of and entity */
  struct LocalTransformComponent : public Component {
    glm::mat4 localMatrix; /*!< Matrix that represent the Local matrix of a entity. */
  };

  //! WorldTransformComponent Struct.
  /*! WorldTransformComponent used to store the world transform of and entity  */
  struct WorldTransformComponent : public Component {
    glm::mat4 worldMatrix; /*!< Matrix that represent the world matrix of a entity. */
  };

 
  //! NodeComponent Struct.
  /*! NodeComponent store the information needed to represent the scene as a tree in an hierarchical way.  */
  struct NodeComponent : public Component {
    int32_t parent = -1;      /*!< Index of the parent in the vector of entities of the engine-1 if the entity does not have a parent.*/  
    uint32_t lvl = 0;         /*!< level in the hierarchy of the transform tree. LvL 0 for the ones without parent.*/  
    uint32_t index = 0 ;      /*!< Index inside the level of the hierarchy of the node.*/  
    int32_t indexparent = -1; /*!< Index inside the parent level of the hierarchy of the parent node. -1 if the entity does not have a parent.*/
  };

  struct ViewProjectionComponent : public Component {
    glm::mat4 viewMatrix;
    glm::mat4 proyectionMatrix;

  };


  struct CameraComponent: public Component{
    int32_t width, height;
    float zNear;
    float zFar;
    float FOV;
    float aspectRatio;

  };

  struct LightComponent : public Component{

    typedef enum {
      Directional,
      Spot,
      Point
    } Type; 
        
    Type lightType;
    float r, g, b;  // Exceed range to represent intensity
    float cutOff;
    glm::vec3 direction;
    float lightWidth;
    float intensity;
            
  };

  struct FlyCameraMovementComponent : public Component {
    float pitch; // -90 -> 90
    float yaw;   // 0 -> 360
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    float lSpeed = 1.0f;;
    float rSpeed = 0.04f;;
  };

  struct SunComponent : public Component {
    glm::vec3 lookPosition;
  };

 
}

#endif // !__COMPONENT_H__