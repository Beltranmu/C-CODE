
/************************************************
 * Transform Tree structs
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __TRASNFORMTREE_H__
#define __TRASNFORMTREE_H__ 1

#include <stdint.h>
#include <vector>
#include <glm.hpp>


namespace AWK {

  //! TTComponent struct.
  /*! Store all the information for each entity in the transform tree. */
  struct TTComponent {
    glm::mat4 localMatrix; /*!<Local matrix of an entity. */
    glm::mat4 wolrdMatrix; /*!<World matrix of an entity. */
    int32_t indexParent; /*!<Id of the parent of an entity. -1 if they don't have parent.*/
  };

  //! LvlStructure struct.
  /*! Store the information of each level of the scene */
  struct LvlStructure {
    uint32_t numberOfEntities;  /*!< How many entities are on a level. */
    int32_t entityStart;        /*!< Where in the array of TTComponet the level of the entity starts. */
    int32_t parentStart;        /*!< Where in the array of TTComponet the level of the parent of the entity starts.*/
  };

  //! TransformTree struct.
  /*! Information for a transform tree. */
  struct TransformTree {
    std::vector<LvlStructure> numberEntitiesLvL; /*!< Vector to store the information of each level of the hierarchy. */
    std::vector<TTComponent> AAA;               /*!< Vector to store the information of each entity needed in the TransformTreeSystem to work properly. */

  };
}

#endif // !__TRASNFORMTREE_H__