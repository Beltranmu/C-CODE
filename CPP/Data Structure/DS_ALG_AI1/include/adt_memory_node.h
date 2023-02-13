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
  struct MemoryNode *next_;
  struct MemoryNode *previous_;
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
   * @return data information, if there is no node, return NULL.
   */
  void*(*data) (MemoryNode *node);

  /**
   * @brief Sets the MemoryNode data a new value
   * @param *node that contains the data
   * @param *src that contains the value for the node data
   * @param bytes determines the size of the *src
   * @return - kErrorCode_MemoryNode_Null when node or scr are NULL.  
             - kErrorCode_ZeroSize when bytes == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16(*setData) (MemoryNode *node, void *src, u16 bytes);


  /** 
   * @brief Gets the node next pointer.
   * @param *node that has the pointer.
   * @return the next pointer, if there is no node, return NULL.
  */
  MemoryNode* (*next)(MemoryNode *node);

  /**
   * @brief Sets the node next pointer.
   * @param *node that has the pointer.
   * @param next_nodethe new direction for the pointer.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*setNext)(MemoryNode *node, MemoryNode *next_node);

  /**
   * @brief Gets the node previous pointer.
   * @param *node that has the pointer.
   * @return the previous pointer, if there is no node, return NULL.
  */
  MemoryNode* (*previous)(MemoryNode *node);


  /**
   * @brief Sets the node previos pointer.
   * @param *node that has the pointer.
   * @param preivous_node the new direction for the pointer.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*setPrevious)(MemoryNode *node, MemoryNode *preivous_node);


  /**
   * @brief @brief Returns the size of the data of the MemoryNode.
   * @param *node that contains the data.
   * @return size of the data of MemoryNode.
   */
  u16(*size) (MemoryNode *node);

  /**
   * @brief Resets the content of memory node and free his data.
   * @param *node will receive a MemoryNode to reset.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
             - kErrorCode_Data_Null when data's node is NULL.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*reset) (MemoryNode *node);

  /**
   * @brief Resets the content of memory node without freeing the data.
   * @param *node will receive a MemoryNode to reset.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*softReset) (MemoryNode *node);

  /**
   * @brief Reset the node and frees the memory node including both data and node. 
   * @param *node will receive a MemoryNode to be freed.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
             - kErrorCode_Data_Null when data's node is NULL.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*free) (MemoryNode *node);

  /**
   * @brief Frees only the memory node.
   * @param *node will receive a MemoryNode to be freed.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*softFree) (MemoryNode *node);


  /**
   * @brief Sets all memory bytes received from parameters to a new value.
   * @param *node will receive a MemoryNode to set its data.
   * @param value will be the new value node->data value.
   * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16(*memSet) (MemoryNode *node, u8 value);

  /**
   * @brief Copies the content from one node (src) into node.
   * @param *node will receive a MemoryNode to copy data content from src.
   * @param *src will be the source Node to copy data from.
   * @param bytes determines the length of src.
   * @return - kErrorCode_MemoryNode_Null when the params node or src are NULL.
             - kErrorCode_ZeroSize  when the param bytes is == 0.
             - KErrorCode_No_Memory when this function try to alloc new memory but there is no memory available.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16(*memCopy) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Concatenates two datas, the outcome will be kept in the node.
   * @param *node have the first data and is where the result is going to be saved.
   * @param *src  the second data to concatenate.
   * @param bytes determines the length of src.
   * @return - kErrorCode_MemoryNode_Null when the params node or src are NULL.
             - kErrorCode_Data_Null when data's node is NULL.
             - kErrorCode_ZeroSize  when the param bytes or the size of the first node are == 0.
             - KErrorCode_No_Memory when this function try to alloc new memory but there is no memory available.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16(*memConcat) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Masks the node with an AND mask.
   * @param *node that will get a mask applied.
   * @param mask the value of the mask.
   * @return - kErrorCode_MemoryNode_Null when the params node is NULL.
             - kErrorCode_Data_Null when data's node is NULL.
             - kErrorCode_ZeroSize  when the size of the first node is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16(*memMask) (MemoryNode *node, u8 mask);

  /**
   * @brief Prints all the node information.
   * @param *node to be printed.
   */
  void(*print) (MemoryNode *node);

  /**
 * @brief Prints all the node information with interger format.
 * @param *node to be printed.
 */
  void(*printI) (MemoryNode *node);
};

/**
 * @brief Creates an empty MemoryNode.
 * @return Returns the new MemoryNode, return NULL if there is no free memory.
 */
MemoryNode* MEMNODE_create();

/**
 * @brief Creates an empty MemoryNode into a given pointer.
 * @param **node node to copy the information.
 * @return - kErrorCode_MemoryNode_Null if the param node is NULL or if there is no            memory avalaible for the new node
           - kErrorCode_Ok Everething was done correctly.
 */
s16 MEMNODE_createFromRef(MemoryNode **node);

/**
 * @brief Creates a MemoryNode without memory allocation.
 * @param *node node to copy the information.
 * @return - kErrorCode_MemoryNode_Null if the param node is NULL
           - kErrorCode_Ok Everething was done correctly.
     
 */
s16 MEMNODE_createLite(MemoryNode *node);

#endif // __ADT_MEMORY_NODE_H__
