// adt_memory_node.h
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "ABGS_MemoryManager/abgs_platform_types.h"

/**
 * @struct Memory Node Struct.
 * @var *data_ will contain the information.
 * @var size_ determines the size of the node.
 * @var *ops_ pointer to MemoryNode functions.
 */
typedef struct memory_node_s {
  void *data_;
  u16 size_;
  struct memory_node_ops_s *ops_;
} MemoryNode;

// Memory Node's API Declarations
/**
 * @struct MemoryNode_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct memory_node_ops_s {

  /**
   * @brief Gets the node data.
   * @param *node to extract the data content.
   * @return data information.
   */
  void*(*data) (MemoryNode *node);

  /**
   * @brief Sets the MemoryNode data a new value
   * @param *node that contains the data
   * @param *src that contains the value for the node data
   * @param bytes determines the size of the *src
   * @return kErrorCode_Memory, kErrorCode_ZeroSize or kErrorCode_Ok.
   */
  s16(*setData) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Returns the size of the MemoryNode
   * @param *node that contains the data
   * @return size of the MemoryNode
   */
  u16(*size) (MemoryNode *node);

  /**
   * @brief Resets the content of memory node.
   * @param *node will receive a MemoryNode to reset.
   * @return kErrorCode_Memory, kErrorCode_Data or kErrorCode_Ok.
  */
  s16(*reset) (MemoryNode *node);

  /**
   * @brief Resets the content of memory node without freeing the data.
   * @param *node will receive a MemoryNode to reset.
   * @return kErrorCode_Memory or kErrorCode_Ok.
  */
  s16(*softReset) (MemoryNode *node);

  /**
   * @brief Frees the memory node including both data and node.
   * @param *node will receive a MemoryNode to be freed.
   * @return kErrorCode_Memory or kErrorCode_Ok.
  */
  s16(*free) (MemoryNode *node);

  /**
   * @brief Frees only the memory node.
   * @param *node will receive a MemoryNode to be freed.
   * @return kErrorCode_Memory, kError_Data or kErrorCode_Ok.
  */
  s16(*softFree) (MemoryNode *node);


  /**
   * @brief Sets all memory bytes received from parameters to a new value.
   * @param *node will receive a MemoryNode to set its data.
   * @param value will be the new value node->data value.
   * @return kErrorCode_Memory or kErrorCode_Ok.
  */
  s16(*memSet) (MemoryNode *node, u8 value);

  /**
   * @brief Copies the content from src into node.
   * @param *node will receive a MemoryNode to copy data content from src.
   * @param *src will be the source Node to copy data from.
   * @param bytes determines the length of src.
   * @return kErrorCode_Memory, kErrorCode_ZeroSize or kErrorCode_Ok.
   */
  s16(*memCopy) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Concatenates two memory nodes into one.
   * @param *node will be the first node to concatenate.
   * @param *src will be the second node to concatenate.
   * @param bytes determines the length of src.
   * @return kErrorCode_Memory, kErrorCode_Data or kErrorCode_Ok.
   */
  s16(*memConcat) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Masks the node with an AND mask.
   * @param *node that will get a mask applied.
   * @param mask the value of the mask.
   * @return kErrorCode_Memory, kErrorCode_Data, kErrorCode_ZeroSize or kErrorCode_Ok.
   */
  s16(*memMask) (MemoryNode *node, u8 mask);

  /**
   * @brief Prints all the node information.
   * @param *node that be printed.
   */
  void(*print) (MemoryNode *node);
};

/**
 * @brief Creates an empty MemoryNode.
 * @return Returns the new MemoryNode.
 */
MemoryNode* MEMNODE_create();

/**
 * @brief Creates an empty MemoryNode into a given pointer.
 * @return kErrorCode_Memory or kErrorCode_Ok.
 */
s16 MEMNODE_createFromRef(MemoryNode **node);

/**
 * @brief Creates a MemoryNode without memory allocation.
 * @return kErrorCode_Memory or kErrorCode_Ok.
 */
s16 MEMNODE_createLite(MemoryNode *node);

#endif // __ADT_MEMORY_NODE_H__
