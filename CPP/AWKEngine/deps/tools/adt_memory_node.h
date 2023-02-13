// adt_memory_node.h
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__ 1

#include "abgs_platform_types.h"


class MemoryNode
{
public:

  MemoryNode();
  MemoryNode(MemoryNode& copynode);
  MemoryNode& operator=(const MemoryNode& other);
  ~MemoryNode();

  /**
   * @brief Creates an empty MemoryNode.
   * @return Returns the new MemoryNode.
  */
  void init();


  /**
    * @brief Gets the node data.
      @return data information, if there is no node, return NULL.
    */
  void *data();

  /**
    * @brief Sets the MemoryNode data a new value
    * @param *src that contains the value for the node data
    * @param bytes determines the size of the *src
    * @return - kErrorCode_MemoryNode_Null when node or scr are NULL.  
              - kErrorCode_ZeroSize when bytes == 0.
              - kErrorCode_Ok Everething was done correctly.
    */
  s16 setData (void *src, u16 bytes);


  /**
    * @brief Gets the node next pointer.
    * @return the next pointer, if there is no node, return NULL.
  */
  MemoryNode* next();

  /**
    * @brief Sets the node next pointer.
    * @param *next_node the new direction for the pointer.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 setNext (MemoryNode *next_node);

  /**
    * @brief Gets the node previous pointer.
    * @return the previous pointer, if there is no node, return NULL.
  */
  MemoryNode* previous();


  /**
    * @brief Sets the node previos pointer.
    * @param *preivous_node the new direction for the pointer.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 setPrevious(MemoryNode *preivous_node);


  /**
    * @brief Returns the size of the data of the MemoryNode.
    * @return size of the data of MemoryNode.
    */
  u16 size();

  /**
    * @brief Resets the content of memory node and free his data.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
              - kErrorCode_Data_Null when data's node is NULL.
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 reset();

  /**
    * @brief Resets the content of memory node without freeing the data.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 softReset ();

  /**
    * @brief Reset the node and frees the memory node including both data and node. 
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
              - kErrorCode_Data_Null when data's node is NULL.
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 free ();

  /**
    * @brief Frees only the memory node.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.    
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 softFree ();


  /**
    * @brief Sets all memory bytes received from parameters to a new value.
    * @param value will be the new value node->data value.
    * @return - kErrorCode_MemoryNode_Null when the param node is NULL.
              - kErrorCode_Ok Everething was done correctly.
  */
  s16 memSet (u8 value);

  /**
    * @brief Copies the content from one node (src) into node.
    * @param *src will be the source Node to copy data from.
    * @param bytes determines the length of src.
    * @return - kErrorCode_MemoryNode_Null when the params node or src are NULL.
              - kErrorCode_ZeroSize  when the param bytes is == 0.
              - KErrorCode_No_Memory when this function try to alloc new memory but there is no memory available.
              - kErrorCode_Ok Everething was done correctly.
    */
  s16 memCopy (void *src, u16 bytes);

  /**
    * @brief Concatenates two memory nodes into the first one.
    * @param *src will be the second node to concatenate.
    * @param bytes determines the length of src.
    * @return - kErrorCode_MemoryNode_Null when the params node or src are NULL.
              - kErrorCode_Data_Null when data's node is NULL.
              - kErrorCode_ZeroSize  when the param bytes or the size of the first node are == 0.
              - KErrorCode_No_Memory when this function try to alloc new memory but there is no memory available.
              - kErrorCode_Ok Everething was done correctly.
    */
  s16 memConcat (void *src, u16 bytes);

  /**
    * @brief Masks the node with an AND mask.
    * @param mask the value of the mask.
    * @return - kErrorCode_MemoryNode_Null when the params node is NULL.
              - kErrorCode_Data_Null when data's node is NULL.
              - kErrorCode_ZeroSize  when the size of the first node is == 0.
              - kErrorCode_Ok Everething was done correctly.
    */
  s16 memMask (u8 mask);

  /**
    * @brief Prints all the node information.
    * @param *node to be printed.
    */
  void print ();


  /**
   * @brief Creates an empty MemoryNode.
   * @return Returns the new MemoryNode.
   */
  //MemoryNode* MEMNODE_create();

  /**
   * @brief Creates an empty MemoryNode into a given pointer.
   * @return kErrorCode_MemoryNode_Null or kErrorCode_Ok.
   */
  //s16 MEMNODE_createFromRef(MemoryNode **node);

  /**
   * @brief Creates a MemoryNode without memory allocation.
   * @return kErrorCode_MemoryNode_Null or kErrorCode_Ok.
   */
  //s16 MEMNODE_createLite(MemoryNode *node);




  protected:

  /**
  * @var *data_ will contain the information.
  * @var size_ determines the size of the node.
  * @var *next_ pointer to the next node.
  * @var *previous_ pointer to the previous node.
  */
  void *data_;
  u16 size_;
  MemoryNode *next_;
  MemoryNode *previous_;

private:

};


#endif // __ADT_MEMORY_NODE_H__